#include "application.hpp"

#include "shake/graphics/gl/gl.hpp"


//----------------------------------------------------------------
void init()
{

}

//----------------------------------------------------------------
void update( double dt )
{

}

//----------------------------------------------------------------
void destroy()
{

}

//----------------------------------------------------------------
int main(int argc, char* argb[])
{
    auto application = shake::Application 
    { 
        init,
        update,
        destroy
    };
    application.run();
}
