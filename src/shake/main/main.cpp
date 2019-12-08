#include "application.hpp"

#include "shake/graphics/gl/gl.hpp"


//----------------------------------------------------------------
int main(int argc, char* argb[])
{
    shake::Application application { };
    application.run();
    return EXIT_SUCCESS;
}
