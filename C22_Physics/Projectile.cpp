#include "Projectile.h"
using namespace Simplex;

Projectile::Projectile()
{
	Init();
}

void Projectile::Init()
{
	//Getting instances of managers needed
	entityManager = MyEntityManager::GetInstance();
	camera = CameraManager::GetInstance();
}

Projectile::~Projectile()
{
	
}

void Projectile::SpawnProjectile()
{
	//Get the position of the active camera
	spawnPoint = camera->GetPosition(-1);
	spawnPoint.x -= 0.5f; //Added to make more centered, otherwise the left edge of the cube would be centered not the cube itself

	//Create the cube + add to laneGrid
	entityManager->AddEntity("Minecraft\\Cube.obj", "Block");
	const uint index = entityManager->GetEntityIndex("Block");
	entity = entityManager->GetEntity(index);
	entity->GetRigidBody()->SetVisibleOBB(false);

	//Get force from forward
	vector3 force = camera->GetForward();

	//Spawn in the cube in the world and apply the force to shoot
	matrix4 m4Position = glm::translate(spawnPoint);
	entityManager->SetModelMatrix(m4Position);
	entityManager->UsePhysicsSolver();
	entityManager->ApplyForce(force * 2.75f);
}

const float& Projectile::GetExplosionRadius() const
{
	return explosionRadius;
}

Simplex::MyEntity* Projectile::GetProjectileEntity() const
{
	return entity;
}

void Projectile::InvalidateProjectile()
{
	SafeDelete(entity);
	entity = nullptr;
}
