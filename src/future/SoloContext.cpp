#include "SoloContext.h"

using namespace solo;


Context::Context(bool multithreaded):
    multithreaded(multithreaded)
{
    if (multithreaded)
    {
        // todo create rendering thread
    }

    // todo command buffer initialization
    // todo kick render thread (if any) to initialize
    waitFrame(); // or smth like that
    // todo init other stuff
}


Context::~Context()
{
}


void Context::waitFrame()
{
}


void Context::swap()
{
}
