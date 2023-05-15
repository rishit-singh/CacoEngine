#ifndef RIGIDMESH_H_
#define RIGIDMESH_H_

#include "rigidbody.hpp"
#include "objects.hpp"

namespace CacoEngine
{
    class RigidMesh2D : public Mesh, public RigidBody2D
    {
    public:
        RigidMesh2D(std::vector<Triangle> = std::vector<Triangle>());
    };

    class RigidObject2D : public Object, public RigidBody2D
    {
    public:
        RigidObject2D();
    };
}

#endif
 