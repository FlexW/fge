#include "physic_manager.hpp"
#include "platform/bullet/bullet_physic_world.hpp"

namespace Fge
{

void PhysicManager::init()
{
  physic_world = std::make_shared<Bullet::BulletPhysicWorld>();
}

void PhysicManager::update(float delta_time)
{
  physic_world->update(delta_time);
}

void PhysicManager::render() { physic_world->render(); }

void PhysicManager::terminate() { physic_world->terminate(); }

} // namespace Fge
