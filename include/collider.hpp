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

        std::vector<T> Objects;

        std::vector<std::shared_ptr<RigidObject2D>> mObjects;

    public:
        virtual void Handle()
        {
            for (int x = 0; x < this->mObjects.size(); x++)
                for (int y = 0; y < this->mObjects.size(); y++)
                    if (x != y && this->mObjects[x]->CollidesWith(*this->mObjects[y]))
                        this->Callback(this->mObjects[y]);
        }

        void AddObject(T object)
        {
            this->Objects.push_back(object);
        }


        void SetCallback(ColliderCallback<T> callback)
        {
            this->Callback = callback;
        }

        std::vector<T>& GetObjects()
        {
            return this->Objects;
        }

        std::vector<std::shared_ptr<RigidObject2D>> GetmObjects()
        {
            return this->mObjects;
        }

        Collider(std::vector<T> objects, std::vector<std::shared_ptr<RigidObject2D>>& mObjects, ColliderCallback<T> callback = nullptr) : Callback(callback), Objects(objects), mObjects(mObjects)
        {
        }

        virtual ~Collider()
        {
        }
    };

    class RigidCircleCollider : public Collider<RigidCircle>
    {
    protected:

    public:

        void Handle()
        {
            for (int x = 0; x < this->Objects.size(); x++)
                for (int y = 0; y < this->Objects.size(); y++)
                    if (x != y && this->Objects[x].CollidesWith(this->Objects[y]))
                    {
                        this->Callback(this->mObjects[y]);

                        std::cout << "Collides\n";
                    }
        }


         RigidCircleCollider(std::vector<RigidCircle>& circles, std::vector<std::shared_ptr<RigidObject2D>>& objects)
            : Collider<RigidCircle>(circles, objects)
        {
            this->SetCallback([](std::shared_ptr<RigidObject2D> object)
            {
                object->SetFillColor(Colors[(int)Color::Red]);
                return true;
            });
        }

        virtual ~RigidCircleCollider() {}
    };
};

#endif // COLLIDER_H_
