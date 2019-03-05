#ifndef APPLICATION_HPP
#define APPLICATION_HPP


#include "shake/core/diagnostics/running_average_counter.hpp"
#include "shake/core/diagnostics/stopwatch.hpp"
#include "shake/core/process_manager.hpp"
#include "shake/core/types/macro_property.hpp"

#include "shake/hid/window.hpp"

#include "shake/content/content_manager.hpp"


namespace shake {


class Application
{
public:
    Application
    (
        const size_t screen_width           = 2560,
        const size_t screen_height          = 1440,
        const std::string& application_name = "Shake Engine"
    );

    ~Application();

    void run();

    void update();
    void start_frame();
    void end_frame();

private:
    using FpsCounter = RunningAverageCounter< float, 120 >;

private:
    PROPERTY_RW( Window,            window              )
    PROPERTY_RW( content::ContentManager,    content_manager     )

    PROPERTY_R( ProcessManager, process_manager     )
    
    PROPERTY_R( Stopwatch,      frame_time_counter  )
    PROPERTY_R( float,          current_frame_time  )

    PROPERTY_R( FpsCounter,     fps_counter         )
    PROPERTY_R( float,          current_fps         )
};

} // namespace shake

#endif // APPLICATION_HPP
