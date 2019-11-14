#include "Simplex/Mesh/Model.h"
#include "MyEntityManager.h"
#include "LaneGrid.h"

class Projectile
{
public:
	Projectile();
	~Projectile();
	Projectile* SpawnProjectile();
	bool SetProjectileActive(bool active);
	Simplex::MyEntity* GetProjectileEntity() const;
private:
	Simplex::MyEntity* entity;
	Simplex::vector3 spawnPoint;
	Simplex::MyEntityManager* entityManager;
	Simplex::CameraManager* camera;
	LaneGrid* laneGrid;
	bool isActive = false;

	void Init();
};
