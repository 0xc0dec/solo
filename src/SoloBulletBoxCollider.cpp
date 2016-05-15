#include "SoloBulletBoxCollider.h"
#include "SoloVector3.h"

using namespace solo;


BulletBoxCollider::BulletBoxCollider(const Vector3& halfExtents)
{
    shape = std::make_unique<btBoxShape>(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
}


auto BoxCollider::create(const Vector3& size) -> sptr<BoxCollider>
{
    return std::make_shared<BulletBoxCollider>(size);
}
