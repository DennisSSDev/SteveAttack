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
	spawnPoint = vector3(-1.0f, 5.0f, 25.0f); //Found from AppClass.cpp and modified to be more centered

	//Spawn in the cube
	entityManager->AddEntity("Minecraft\\Cube.obj", "Block");
	matrix4 m4Position = glm::translate(spawnPoint);
	entityManager->SetModelMatrix(m4Position * glm::scale(vector3(2.0f)));
}