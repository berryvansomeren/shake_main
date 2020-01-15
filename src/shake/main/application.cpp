#include "application.hpp"

#include <vector>
#include <memory>

#include "shake/content/content_manager.hpp"
#include "shake/core/math/math.hpp"
#include "shake/core/process_manager.hpp"
#include "shake/core/type_traits/lexical_cast.hpp"
#include "shake/graphics/camera.hpp"
#include "shake/graphics/context.hpp"

#include "shake/graphics/draw.hpp"
#include "shake/graphics/draw_text.hpp"
#include "shake/graphics/gl/gl.hpp"
#include "shake/graphics/geometry/geometry_3d.hpp"
#include "shake/graphics/material/uniform.hpp"
#include "shake/hid/keyboard.hpp"
#include "shake/hid/mouse.hpp"
#include "shake/io/file_json.hpp"


namespace shake {

namespace { // anonymous

const io::Path game_content_directory { "C:/Users/Berry/Documents/development/shake3/shake_test_game/content/game1/" };

} // namespace anonymous

//----------------------------------------------------------------
Application::Application
(
    const InitCallback& init_callback,
    const UpdateCallback& update_callback,
    const DestroyCallback& destroy_callback,
    const size_t screen_width,
    const size_t screen_height,
    const std::string& application_name
)
    : m_window { screen_width, screen_height, application_name }
    , m_total_run_time { 0.0 }
    , m_total_simulation_time { 0.0 }
    , m_init_callback { init_callback }
    , m_update_callback { update_callback }
    , m_destroy_callback { destroy_callback }
{
    // graphics
    graphics::gl::init( m_window.get_glfw_gl_load_proc() );
    graphics::set_current_screen_size( { screen_width, screen_height } );
    graphics::set_current_camera( std::make_shared<graphics::Camera>( screen_width, screen_height ) );

    // hid
    Keyboard::get_instance().init( m_window );
    Mouse::get_instance().init( m_window );

    // content
    m_content_manager.init();
    m_content_manager.host_content_directory( game_content_directory );

    set_target_fps( 60 );
    set_max_updates_per_frame( 3 );
}

//----------------------------------------------------------------
Application::~Application()
{
    // make sure to destroy all content that's still in memory
    // before destructing the correpsonding contexts
    m_content_manager.destroy();
}

//----------------------------------------------------------------
void Application::set_target_fps( const std::uint8_t target_fps )
{
    m_target_fps        = target_fps;
    m_update_period_ms  = 1000 / static_cast<double>( target_fps );
}

//----------------------------------------------------------------
void Application::set_max_updates_per_frame( const std::uint8_t max_updates_per_frame )
{
    m_max_updates_per_frame = max_updates_per_frame;
    m_max_accumulated_residual_time_ms = max_updates_per_frame * m_update_period_ms;
}

//----------------------------------------------------------------
void Application::run()
{
    const auto default_primitive_2d_shader      = m_content_manager.get_or_load<graphics::Shader>( io::Path { "shaders/default_primitive_2d_shader.glsl" } );
    const auto default_primitive_2d_material    = std::make_shared<graphics::Material>( default_primitive_2d_shader );

    init();

    // We enter an infinite update-render-loop,
    // until someone decides to quit the application.
    m_application_timer.reset();
    m_frame_timer.reset();
    auto accumulated_residual_time_ms = double { 0.0 };

    while ( !m_window.get_should_close() )
    {
        auto did_fixed_update = false;

        // We basically measure how much time has passed since last frame
        // In that sense the rendering and pre_update steps PRODUCE time.
        // We now have to CONSUME that time by running our logic updates and catch up.
        // Consumption happens in uniformly sized steps.
        const auto frame_time = m_frame_timer.get_and_reset();

        // calculate fps
        m_fps_counter.add_sample( frame_time );
        m_current_fps = static_cast<uint64_t>( std::floor( 1000.0 / m_fps_counter.get_running_average() ) );

        // measure how much time we accumulated to simulate
        accumulated_residual_time_ms += frame_time;

        // The behavior of the physics simulation depends on the delta time.
        // If delta time gets too big you get tunneling.
        // if delta time gets too small you get numerical errors.
        // A fixed delta time will make the simulation more stable,
        // deterministic, and easier to debug.
        // However, it takes X amount of computation time,
        // to simulate Y amount of simulation time.
        // If X > Y, you'll get a spiral of death situation.
        // We therefore specify a maximum number of logic updates per frame.
        // As a result, simulation time will slow down with heavy loads,
        // while frame rate is (hopefully) preserved.
        accumulated_residual_time_ms = std::min( accumulated_residual_time_ms, m_max_accumulated_residual_time_ms );
        while ( accumulated_residual_time_ms >= m_update_period_ms )
        {
            update( m_update_period_ms );
            accumulated_residual_time_ms -= m_update_period_ms;
            did_fixed_update = true;
        }

        // If frame rate is super high,
        // and thus delta time is super low,
        // it might happen we get to render again,
        // while no update has happened in between frames.
        // In that case we will simply not update the screen,
        // instead of redrawing the same thing.
        // This enables us to get back to logic updates ASAP.
        // This will however affect our estimate of FPS a little bit.
        if ( !did_fixed_update ) 
        { 
            continue; 
        }

        render();
        
        // TEMPORARY below

        graphics::gl::clear( { graphics::gl::FramebufferBitFlag::Color, graphics::gl::FramebufferBitFlag::Depth } );

        // TODO: render stuff
        default_primitive_2d_material->set_uniform( "u_color", std::make_unique<graphics::UniformVec3>( glm::vec3 { 1.f, 1.f, 0.f } ) );
        const auto geometry = graphics::make_circle_filled_2D( 0.5f );
        graphics::draw
        ( 
            graphics::RenderPack2D { geometry, default_primitive_2d_material },
            Transform2D { }
        );

        m_window.swap_buffers();
        
    }

    destroy();
}

//----------------------------------------------------------------
void Application::init() 
{
    m_init_callback();
}

//----------------------------------------------------------------
void Application::update( const double dt ) 
{
    m_window.poll_events();
    Keyboard::get_instance().update();
    Mouse::get_instance().update();

    m_update_callback( dt );
}

//----------------------------------------------------------------
void Application::render() 
{
    
}

//----------------------------------------------------------------
void Application::destroy()
{
    m_destroy_callback();
}

} // namespace shake
