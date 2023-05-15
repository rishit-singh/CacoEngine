#include "rigidmesh.hpp"

CacoEngine::RigidMesh2D::RigidMesh2D(std::vector<Triangle> triangles) : Mesh(triangles), RigidBody2D()
{}

CacoEngine::RigidObject2D::RigidObject2D() : Object(), RigidBody2D()
{}
