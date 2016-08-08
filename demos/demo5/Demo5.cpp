#include "../../include/Solo.h"

using namespace solo;


class Demo final: public DeviceCallback
{
public:
    void onStarted() override final
    {
    }
};


int main()
{
    Device::run(
        DeviceSetup().withMode(DeviceMode::Vulkan).withDimensions(1200, 600).withLogFilePath("demo5.log"),
        std::make_unique<Demo>()
    );
    return 0;
}
