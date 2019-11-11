#include "Simplex/Mesh/Model.h"
#include "MyEntityManager.h"
#include "LaneGrid.h"

class Projectile
{
public:
	Projectile();
	~Projectile();
	void SpawnProjectile();
	bool SetProjectileActive(bool active);

private:
	Simplex::MyEntity* model;
	Simplex::vector3 spawnPoint;
	Simplex::MyEntityManager* entityManager;
	Simplex::CameraManager* camera;
	bool isActive = false;

	void Init();
};
