#pragma once

#include "SoloNode.h"
#include "SoloBitFlags.h"
#include "SoloRenderQueue.h"


namespace solo
{
    struct RenderContext;

    struct ComponentTypeId
    {
        template <typename T>
        static uint32_t get()
        {
            static const auto id = counter++;
            return id;
        }

    private:
        static uint32_t counter;
    };


    class Component
    {
    public:
        SL_NONCOPYABLE(Component)
        virtual ~Component() {}

        virtual uint32_t getTypeId() = 0;

        virtual void init() {}
        virtual void update() {}
        virtual void terminate() {}
        virtual void render(RenderContext& context) {}
        virtual void onAfterCameraRender() {}

        uint32_t getRenderQueue() const;
        void setRenderQueue(uint32_t queue);

        Node getNode() const;

        BitFlags& getTags();

    protected:
        explicit Component(const Node& node) : node(node)
        {
            tags.set(1);
        }

        Node node;
        BitFlags tags;
        uint32_t renderQueue = KnownRenderQueues::NotRendered;
    };

    inline uint32_t Component::getRenderQueue() const
    {
        return renderQueue;
    }

    inline void Component::setRenderQueue(uint32_t queue)
    {
        renderQueue = queue;
    }

    inline Node Component::getNode() const
    {
        return node;
    }

    inline BitFlags& Component::getTags()
    {
        return tags;
    }


    template <class T>
    class ComponentBase: public Component
    {
    public:
        static uint32_t getId();

        explicit ComponentBase(const Node& node): Component(node) {}

        virtual uint32_t getTypeId() override;
    };

    template <class T>
    inline uint32_t ComponentBase<T>::getId()
    {
        return ComponentTypeId::get<T>();
    }

    template <class T>
    inline uint32_t ComponentBase<T>::getTypeId()
    {
        return getId();
    }
}
