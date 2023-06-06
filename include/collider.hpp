#ifndef COLLIDER_H_
#define COLLIDER_H_

#include <functional>
#include "rigidobject.hpp"

namespace CacoEngine
{
    template<typename T>
    using ColliderCallback = std::function<bool(std::shared_ptr<RigidObject2D>)>; //  bool(*)(T, T);

    // Wrapper for collision check and resolution routines
    template<typename T>
    class Collider
    {
    protected:
        ColliderCallback<T> Callback;

        std::vector<std::shared_ptr<RigidObject2D>> Objects;

    public:
        virtual void Handle()
        {
            for (int x = 0; x < this->Objects.size(); x++)
                for (int y = 0; y < this->Objects.size(); y++)
                    if (this->Objects[x]->CollidesWith(*this->Objects[y]))
                        this->Callback(this->Objects[y]);
        }

        void AddObject(std::shared_ptr<RigidObject2D> object)
        {
            this->Objects.push_back(object);
        }

        void SetCallback(ColliderCallback<T> callback)
        {
            this->Callback = callback;
        }

        Collider(std::vector<std::shared_ptr<RigidObject2D>>& objects, ColliderCallback<T> callback = nullptr) : Callback(callback)
        {
            for (int x = 0; x < objects.size(); x++)
                this->Objects.push_back(objects[x]);
        }

        virtual ~Collider()
        {
        }
    };

    class RigidCircleCollider : public Collider<RigidCircle>
    {
    protected:
        std::vector<RigidCircle> Circles;
    
    public:
        void Handle()
        {
            for (int x = 0; x < this->Circles.size(); x++)
                for (int y = 0; y < this->Circles.size(); y++)
                    if (this->Circles[x].CollidesWith(this->Circles[y]))
                        this->Callback(this->Objects[y]);
        }

         RigidCircleCollider(std::vector<RigidCircle>& circles, std::vector<std::shared_ptr<RigidObject2D>>& objects)
            : Collider<RigidCircle>(objects), Circles(circles)
        {
            this->SetCallback([](std::shared_ptr<RigidObject2D> object){
                object->SetFillColor(Colors[(int)Color::Red]);
                return true;
            });
        }

        virtual ~RigidCircleCollider() {}
    };
};

#endif // COLLIDER_H_
