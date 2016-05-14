#include "SoloBulletBoxCollider.h"

using namespace solo;


BulletBoxCollider::BulletBoxCollider(const Node& node, const Vector3& halfExtents):
    BoxCollider(node)
{
    shape = std::make_unique<btBoxShape>(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
}


auto BoxCollider::create(const Node& node, const Vector3& size) -> sptr<BoxCollider>
{
    return std::make_shared<BulletBoxCollider>(node, size);
}


BoxCollider::BoxCollider(const Node& node):
    Collider(node)
{
}
