#pragma once

#include "SoloBase.h"
#include "SoloRenderer.h"


namespace solo
{
    class Effect final
    {
    public:
        static auto create(const std::string& vsSrc, const std::string& fsSrc) -> sptr<Effect>;

        ~Effect();
        SL_NONCOPYABLE(Effect)

        auto getHandle() const -> ProgramHandle;

        void apply();

    private:
        Effect(const std::string& vsSrc, const std::string& fsSrc);

        Renderer* renderer;
        ProgramHandle handle;
    };

    inline auto Effect::getHandle() const -> ProgramHandle
    {
        return handle;
    }
}
