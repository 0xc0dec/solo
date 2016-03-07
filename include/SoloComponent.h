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
        static int get()
        {
            static const auto id = counter++;
            return id;
        }

    private:
        // TODO Expected to be less than the smallest script component id (which is now 1 000 000 000).
        // No checks so far since it'll hardly ever reach that limit (boldly said, but still).
        static int counter;
    };


    class Component
    {
    public:
        SL_NONCOPYABLE(Component)
        virtual ~Component() {}

        virtual int getTypeId() = 0;

        virtual void init() {}
        virtual void update() {}
        virtual void terminate() {}
        virtual void render(RenderContext& context) {}
        virtual void onAfterCameraRender() {}

        int getRenderQueue() const;
        void setRenderQueue(int queue);

        Node getNode() const;

        BitFlags& getTags();

    protected:
        explicit Component(const Node& node) : node(node)
        {
            tags.set(1);
        }

        Node node;
        BitFlags tags;
        int renderQueue = KnownRenderQueues::NotRendered;
    };

    inline int Component::getRenderQueue() const
    {
        return renderQueue;
    }

    inline void Component::setRenderQueue(int queue)
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
        static int getId();

        explicit ComponentBase(const Node& node): Component(node) {}

        virtual int getTypeId() override;
    };

    template <class T>
    inline int ComponentBase<T>::getId()
    {
        return ComponentTypeId::get<T>();
    }

    template <class T>
    inline int ComponentBase<T>::getTypeId()
    {
        return getId();
    }
}
