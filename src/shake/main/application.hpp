#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <optional>
#include <variant>

#include "shake/core/diagnostics/running_average_counter.hpp"
#include "shake/core/diagnostics/stopwatch.hpp"
#include "shake/core/process_manager.hpp"
#include "shake/core/macros/macro_property.hpp"

#include "shake/hid/window.hpp"

#include "shake/content/content_manager.hpp"

#include "shake/core/std/optional_callback.hpp"

namespace shake {

//----------------------------------------------------------------
class Application
{
public:
    using InitCallback      = std::function<void()>;
    using UpdateCallback    = std::function<void(double)>;
    using DrawCallback      = std::function<void()>;
    using DestroyCallback   = std::function<void()>;

public:
    Application
    (
        const InitCallback&     init_callback,
        const UpdateCallback&   update_callback,
        const DrawCallback&     draw_callback,
        const DestroyCallback&  destroy_callback,
        const size_t            screen_width        = 2560,
        const size_t            screen_height       = 1440,
        const std::string&      application_name    = "Shake Engine"
    );

    ~Application();

    void set_target_fps( const std::uint8_t target_fps );
    void set_max_updates_per_frame( const std::uint8_t max_updates_per_frame );

    inline double get_total_lag_time()
    {
        return m_total_run_time - m_total_simulation_time;
    }

    inline void close()
    {
        m_window.set_should_close();
    }

    void run();
    void init();
    void update( const double dt );
    void draw();
    void destroy();

public:
    PROPERTY_RW( InitCallback,      init_callback     )
    PROPERTY_RW( UpdateCallback,    update_callback   )
    PROPERTY_RW( DrawCallback,      draw_callback     )
    PROPERTY_RW( DestroyCallback,   destroy_callback  )

private:
    using FpsCounter = RunningAverageCounter< double, 120 >;

private:

    Stopwatch m_application_timer;
    Stopwatch m_frame_timer;

    PROPERTY_R( std::uint8_t,   target_fps              )
    PROPERTY_R( double,         update_period_ms        )

    PROPERTY_R( std::uint8_t,   max_updates_per_frame   )
    PROPERTY_R( double,         max_accumulated_residual_time_ms    )

    PROPERTY_R( double,         total_run_time )
    PROPERTY_R( double,         total_simulation_time )

    PROPERTY_RW( Window,            window              )
    PROPERTY_RW( content::ContentManager,    content_manager     )
    
    PROPERTY_R( FpsCounter,     fps_counter         )
    PROPERTY_R( float,          current_fps         )
};

} // namespace shake

#endif // APPLICATION_HPP
