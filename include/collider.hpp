#ifndef COLLIDER_H_
#define COLLIDER_H_

#include <functional>
#include "rigidobject.hpp"

namespace CacoEngine
{
    template<typename T>
    using ColliderCallback = std::function<bool(T)>; //  bool(*)(T, T);

    // Wrapper for collision check and resolution routines
    template<typename T>
    class Collider
    {
    protected:
        ColliderCallback<T> Callback;

        std::unique_ptr<RigidObject2D> Object;
    public:

        virtual bool CollidesWith(T object)
        {
            return true;
        }

        void SetCallback(ColliderCallback<T> callback)
        {
            this->Callback = callback;
        }

        Collider(std::unique_ptr<RigidObject2D> object, ColliderCallback<T> callback = nullptr) : Object(std::move(object)), Callback(callback)
        {
        }

        virtual ~Collider()
        {
        }
    };

    class RigidCircleCollider : public Collider<RigidCircle>
    {
    protected:
        std::unique_ptr<RigidCircle> mCircle;
    
    public:

        bool CollidesWith(RigidCircle circle) override
        {
            return (*this->mCircle).CollidesWith(circle);
        }

        RigidCircleCollider(std::unique_ptr<RigidCircle> circle, std::unique_ptr<RigidObject2D> object)
            : Collider<RigidCircle>(std::move(circle)), mCircle(std::move(circle))
        {
            this->SetCallback([this](RigidCircle circle) {
                if (this->CollidesWith(circle))
                    (*this->Object).SetFillColor(Colors[(int)Color::Red]);

                return true;
            });
        }

        virtual ~RigidCircleCollider() {}
    };
};

#endif // COLLIDER_H_
