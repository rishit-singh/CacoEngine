#ifndef RIGIDMESH_H_
#define RIGIDMESH_H_

#include "rigidbody.hpp"
#include "objects.hpp"

namespace CacoEngine
{
    class RigidMesh : public Mesh, public RigidBody2D
    {
    public:
        RigidMesh(std::vector<Triangle> triangles = new std::vector<Triangle>()) : Mesh()
        {}
    }
}

#endif
 