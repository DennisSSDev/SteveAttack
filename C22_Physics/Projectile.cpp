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

	SpawnProjectile();
}

Projectile::~Projectile()
{
	
}

void Projectile::SpawnProjectile()
{
	//Get the position of the active camera
	spawnPoint = camera->GetPosition(-1);
	spawnPoint.x -= 0.5f; //Added to make more centered, otherwise the left edge of the cube would be centered not the cube itself

	vector3 force = camera->GetForward();

	//Spawn in the cube
	entityManager->AddEntity("Minecraft\\Cube.obj", "Block");
	matrix4 m4Position = glm::translate(spawnPoint);
	entityManager->SetModelMatrix(m4Position);
	entityManager->UsePhysicsSolver();
	entityManager->ApplyForce(force * 2.0f);
}