#include "application.hpp"

#include <vector>
#include <memory>

#include <GLFW/glfw3.h>

#include "shake/content/content_manager.hpp"
#include "shake/core/math/math.hpp"
#include "shake/core/process_manager.hpp"
#include "shake/core/types/lexical_cast.hpp"
#include "shake/graphics/camera.hpp"
#include "shake/graphics/context.hpp"

#include "shake/graphics/draw.hpp"
#include "shake/graphics/draw_text.hpp"
#include "shake/graphics/gl.hpp"
#include "shake/graphics/geometry/geometry_3d.hpp"
#include "shake/graphics/geometry/voxel_grid.hpp"
#include "shake/graphics/material/uniform.hpp"
#include "shake/hid/keyboard.hpp"
#include "shake/hid/mouse.hpp"
#include "shake/io/file_json.hpp"



namespace shake {

namespace { // anonymous

    glm::vec3 lightPos(2.0f, 1.75f, 1.5f);

    glm::vec2 sprite_pos ( 0.f, 0.f );

    const io::Path game_content_directory { "C:/Users/Berry/Documents/shake3/shake_test_game/content/game1/" };

} // namespace anonymous

//----------------------------------------------------------------
Application::Application
(
    const size_t screen_width,
    const size_t screen_height,
    const std::string& application_name
)
    : m_window  { screen_width, screen_height, application_name }
    , m_frame_time_counter { }
    , m_fps_counter { }
    , m_process_manager { }
    , m_content_manager { }
{
    // graphics
    graphics::gl::init( m_window.get_glfw_gl_load_proc() );
    content::init_gl( m_window.get_glfw_gl_load_proc() );
    graphics::set_current_screen_size( { screen_width, screen_height } );

    // hid
    Keyboard::get_instance().init( m_window );
    Mouse::get_instance().init( m_window );

    // content
    m_content_manager.init( m_window.get_glfw_gl_load_proc() );
    m_content_manager.host_content_directory( game_content_directory );

    // processes 
    m_process_manager
        .add_process( []( float ) { glfwPollEvents(); } )
        .add_process( []( float ) { Keyboard::get_instance().update(); } )
        .add_process( []( float ) { Mouse::get_instance().update(); } )
    ;
}

//----------------------------------------------------------------
Application::~Application()
{
    // make sure to destroy all content that's still in memory
    // before destructing the correpsonding contexts
    m_content_manager.destroy();
}

//----------------------------------------------------------------
void Application::update()
{
    m_current_frame_time = m_frame_time_counter.get_and_reset();

    m_fps_counter.add_sample( m_current_frame_time );
    m_current_fps = static_cast<uint64_t>( std::floor( 1000.f / m_fps_counter.get_running_average() ) );

    m_process_manager.update( m_current_frame_time );


    //        
    //auto& camera = graphics::get_current_camera();
    //const auto& keyboard = Keyboard::get_instance();
    //const auto& mouse = Mouse::get_instance();

    //auto translation_velocity = float { 0.01f };
    //if ( keyboard.is_down( Keyboard::Key::W ) ) { camera->get_transform().translate( -math::axis::z * translation_velocity );   }
    //if ( keyboard.is_down( Keyboard::Key::S ) ) { camera->get_transform().translate(  math::axis::z * translation_velocity );   }
    //if ( keyboard.is_down( Keyboard::Key::A ) ) { camera->get_transform().translate( -math::axis::x * translation_velocity );   }
    //if ( keyboard.is_down( Keyboard::Key::D ) ) { camera->get_transform().translate(  math::axis::x * translation_velocity );   }
    //if ( keyboard.is_down( Keyboard::Key::E ) ) { camera->get_transform().translate(  math::axis::y * translation_velocity );   }
    //if ( keyboard.is_down( Keyboard::Key::Q ) ) { camera->get_transform().translate( -math::axis::y * translation_velocity );   }
    //    

    //auto rotation_velocity = float { 0.01f };
    //if( mouse.is_down( Mouse::Key::Left ) )
    //{
    //    // to prevent introducing rotation by pitching and yawing, you do yaw relative to world
    //    camera->get_transform().yaw_world       ( -mouse.get_diff_x() * rotation_velocity );
    //    camera->get_transform().pitch_relative  ( -mouse.get_diff_y() * rotation_velocity );
    //}

}








//----------------------------------------------------------------
void Application::run()
{
    //----------------------------------------------------------------
    // Initialize dependencies
    auto& window = m_window;

    //auto script_system = script_system::ScriptSystem();

    auto camera = std::make_shared<graphics::Camera>( 2560, 1440 );
    graphics::set_current_camera( camera );
    //camera->get_transform().look_in_direction( glm::vec3{ 0.f, 4.f, 0.f } );

    //----------------------------------------------------------------
    // Initialize game processes
    auto process_manager = ProcessManager { };
    process_manager
        .add_process( []                (float   )  { glfwPollEvents(); } )
        .add_process( []                (float   )  { Keyboard::get_instance().update(); } )
        .add_process( []                (float   )  { Mouse::get_instance().update(); } )
        //.add_process( [&script_system]  (float dt)  { script_system.update( dt ); } )
        .add_process( [&camera]         (float dt)  { camera->update( dt ); } );
    ;

    auto running_average_counter = RunningAverageCounter< float, 120 > { };
    auto stopwatch = Stopwatch();

    const auto voxel_content = io::file::json::read( m_content_manager.get_full_path( io::Path( "meshes/cube_map.json" ) ) );
    const auto vertices      = io::file::json::read_as<std::vector<float>>( voxel_content, {"vertices"} );
    const auto skybox_geometry = std::make_shared<graphics::Triangles3D>( vertices );
    const auto skybox_material = m_content_manager.get_or_load<graphics::Material>( io::Path { "materials/default_cube_map_material.json" } );
    const auto render_pack_3d = graphics::RenderPack3D { skybox_geometry, skybox_material };


    const auto default_primitive_3d_shader      = m_content_manager.get_or_load<graphics::Shader>( io::Path { "shaders/default_primitive_3d_shader.glsl" } );
    const auto default_primitive_3d_material    = std::make_shared<graphics::Material>( default_primitive_3d_shader );
    
    const auto axes = std::vector<glm::vec3>{ { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0 } };

    const auto voxel_render_pack = graphics::RenderPack3D 
    { 
        m_content_manager.get_or_load<graphics::VoxelGrid>( io::Path{ "voxel_models/berry.vox" } ),
        m_content_manager.get_or_load<graphics::Material>( io::Path { "materials/default_voxel_material.json" } )
    };

    const auto fez_render_pack = graphics::RenderPack2D 
    { 
        std::make_shared<graphics::Rectangle2D>( 100, 100 ),
        m_content_manager.get_or_load<graphics::Material>( io::Path{ "materials/sprite_material.json" } )
    };

    const auto font = m_content_manager.get_or_load<graphics::Font>( io::Path{ "fonts/open_sans/open_sans.json" } );

    double total_time = 0.0;

    auto voxel_transform = Transform3D{};
    //----------------------------------------------------------------
    // Game Loop
    while ( !window.get_should_close() )
    {
        // Get time delta
        const auto dt = stopwatch.get_and_reset();
        total_time += dt;

        const auto& keyboard = Keyboard::get_instance();
        const auto& mouse = Mouse::get_instance();

        if ( keyboard.get_instance().is_down( Keyboard::Key::Escape ) )
        {
            window.set_should_close();
        }

        auto& camera = graphics::get_current_camera();

        //auto translation_velocity = float { 0.01f * dt };
        //if ( keyboard.is_down( Keyboard::Key::W ) ) { camera->get_transform().translate( -math::axis::z * translation_velocity );   }
        //if ( keyboard.is_down( Keyboard::Key::S ) ) { camera->get_transform().translate(  math::axis::z * translation_velocity );   }
        //if ( keyboard.is_down( Keyboard::Key::A ) ) { camera->get_transform().translate( -math::axis::x * translation_velocity );   }
        //if ( keyboard.is_down( Keyboard::Key::D ) ) { camera->get_transform().translate(  math::axis::x * translation_velocity );   }
        //if ( keyboard.is_down( Keyboard::Key::E ) ) { camera->get_transform().translate(  math::axis::y * translation_velocity );   }
        //if ( keyboard.is_down( Keyboard::Key::Q ) ) { camera->get_transform().translate( -math::axis::y * translation_velocity );   }
        //

        //auto rotation_velocity = float { 0.01f * dt };
        //if( mouse.is_down( Mouse::Key::Left ) )
        //{
        //    // to prevent introducing rotation by pitching and yawing, you do yaw relative to world
        //    camera->get_transform()->yaw_world       ( -mouse.get_diff_x() * rotation_velocity );
        //    camera->get_transform()->pitch_relative  ( -mouse.get_diff_y() * rotation_velocity );
        //}







        auto polygon_mode = graphics::gl::PolygonMode::Fill;
        if ( Keyboard::get_instance().is_down( Keyboard::Key::L ) )
        {
            polygon_mode = graphics::gl::PolygonMode::Line;
        }
        graphics::gl::set_current_polygon_mode( polygon_mode );

        graphics::gl::clear( { graphics::gl::FramebufferBitFlag::Color, graphics::gl::FramebufferBitFlag::Depth } );
        graphics::gl::disable_depth_mask();
        graphics::draw( render_pack_3d, Transform3D { } );
        graphics::gl::enable_depth_mask();

        // Update all game processes!
        process_manager.update( dt );

        for ( const auto i : { 0, 1, 2 } )
        {
            const auto origin = glm::vec3{ 0.0, 0.0, 0.0 };
            auto destination = glm::vec3{ 0.0, 0.0, 0.0 };
            destination[ i ] = 10.0;
            auto vertices = std::vector<glm::vec3>{ origin, destination };
            default_primitive_3d_material->set_uniform( "u_color", std::make_unique<graphics::UniformVec3>( destination ) );
            graphics::draw
            ( 
                graphics::RenderPack3D 
                { 
                    std::make_shared<graphics::Lines3D>( vertices ),
                    default_primitive_3d_material 
                },
                Transform3D{}
            );
        }

        
        voxel_transform.set_translation( glm::vec3( 
            ( sin( total_time / 1000 ) * 10 ), 
            0.0, 
            ( cos( total_time / 1000 ) * 10 )
        ) );

        if ( keyboard.is_down( Keyboard::Key::Y ) ) { voxel_transform.yaw_relative( dt );   }
        if ( keyboard.is_down( Keyboard::Key::U ) ) { voxel_transform.pitch_relative( dt );   }
        if ( keyboard.is_down( Keyboard::Key::I ) ) { voxel_transform.roll_relative( dt );   }
        graphics::draw( voxel_render_pack, voxel_transform );
               
        // Draw FPS
        running_average_counter.add_sample( dt );
        const auto fps = static_cast<uint64_t>( std::floor( 1000.f / running_average_counter.get_running_average() ) );
        graphics::draw( "FPS: " + lexical_cast( fps ), glm::vec2{ 20, 20 }, font );

        // Draw FPS
        const auto camera_position =  camera->get_transform()->get_translation();
        graphics::draw
        ( 
            "Camera Position: " 
            + lexical_cast( camera_position.x ) + ", " 
            + lexical_cast( camera_position.y ) + ", "
            + lexical_cast( camera_position.z ) + ", ", 
            glm::vec2{ 20, 40 }, 
            font 
        );

        graphics::draw( fez_render_pack, Transform2D{} );

        // Swap buffers
        window.swap_buffers();
    }
}

} // namespace shake
