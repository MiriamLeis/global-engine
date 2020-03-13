#include "RigidbodyPC.h"
#include "Factory.h"
#include "FactoriesFactory.h"
 
#include "ComponentsManager.h"
#include "PhysicsContext.h"
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include "OgreVector3.h"
#include "Entity.h"
#include "TransformComponent.h"


RigidbodyPC::RigidbodyPC(Ogre::Vector3 _pos, Ogre::Vector3 _shape, float _mass, bool _trigger)
{
	body = PhysicsContext::instance()->createRB(_pos, _shape, _mass);
	setTrigger(_trigger);
}

RigidbodyPC::~RigidbodyPC()
{
	body = nullptr;
}

void RigidbodyPC::update()
{
	//Este codigo nos puede servir de ayuda, sirve para detectar las colisiones pero mi duda es las variables "btPersistentManifold" y "btCollisionObject"
	//no se exactamente su utilidad y como enlazarlo con el rigibdody
	//int numManifolds = PhysicsContext::instance()->discreteDynamicsWorld->getDispatcher()->getNumManifolds();
	//for (int i = 0; i < numManifolds; i++)
	//{
	//	btPersistentManifold* contactManifold = PhysicsContext::instance()->discreteDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
	//	const btCollisionObject* obA = contactManifold->getBody0();
	//	const btCollisionObject* obB = contactManifold->getBody1();

	//	if (!obA->getCollisionShape()->isNonMoving() && !obB->getCollisionShape()->isNonMoving()) {
	//		Collision = true;
	//		return;
	//	}
	//}

	//Collision = false;

	dynamic_cast<TransformComponent*>(father->getComponent("TransformComponent"))->
		setPosition(Ogre::Vector3(body->getWorldTransform().getOrigin().x(), body->getWorldTransform().getOrigin().y(), body->getWorldTransform().getOrigin().z()));
}

bool RigidbodyPC::collidesWith(std::string id)
{
	return false;
}

void RigidbodyPC::addForce(const Ogre::Vector3 _force, Ogre::Vector3 _relative_pos)
{
	if (_relative_pos == Ogre::Vector3(0.0f, 0.0f, 0.0f))
		body->applyCentralForce((btVector3(btScalar(_force.x), btScalar(_force.y), btScalar(_force.z))));
	else
		body->applyForce((btVector3(btScalar(_force.x), btScalar(_force.y), btScalar(_force.z))), (btVector3(btScalar(_relative_pos.x), btScalar(_relative_pos.y), btScalar(_relative_pos.z))));
}

void RigidbodyPC::setGravity(const Ogre::Vector3 _g)
{
	body->setGravity(btVector3(btScalar(_g.x), btScalar(_g.y), btScalar(_g.z)));
}

bool RigidbodyPC::isTrigger() const
{
	return trigger;
}

bool RigidbodyPC::isKinematic() const
{
	return kinematic;
}

bool RigidbodyPC::isTriggerStatic() const
{
	return stat;
}

void RigidbodyPC::setTrigger(bool _trigger)
{
	body->setCollisionFlags(body->getCollisionFlags() & (body->CF_NO_CONTACT_RESPONSE * trigger));
	trigger = _trigger;
}

void RigidbodyPC::setKinematic(bool _kinematic)
{
	body->setCollisionFlags(body->getCollisionFlags() & (body->CF_KINEMATIC_OBJECT * _kinematic));
	kinematic = _kinematic;
}

void RigidbodyPC::setStatic(bool _static)
{
	body->setCollisionFlags(body->getCollisionFlags() & (body->CF_STATIC_OBJECT * _static));
	stat = _static;
}

// FACTORY INFRASTRUCTURE
class RigidbodyPCFactory : public ComponentFactory {
public:
	RigidbodyPCFactory() {};

	virtual Component* create(Entity* _father, Json::Value& _data, ComponentsManager* _componentManager)
	{
		RigidbodyPC* rb;
		rb->setFather(_father);

		TransformComponent* transform = dynamic_cast<TransformComponent*>(_father->getComponent("TransformComponent"));

		rb = new RigidbodyPC(transform->getPosition(), transform->getPosition(), 1, true);

		_componentManager->addPC(rb);
		return rb;
	};
};

REGISTER_FACTORY("RigidbodyPC", RigidbodyPC);