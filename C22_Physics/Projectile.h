#include "Simplex/Mesh/Model.h"
#include "MyEntityManager.h"
#include "LaneGrid.h"

class Projectile
{
	const float explosionRadius = 10.f;
	
public:
	Projectile();
	~Projectile();
	void SpawnProjectile();
	const float& GetExplosionRadius() const;
	Simplex::MyEntity* GetProjectileEntity() const;
	void InvalidateProjectile();
private:
	Simplex::MyEntity* entity;
	Simplex::vector3 spawnPoint;
	Simplex::MyEntityManager* entityManager;
	Simplex::CameraManager* camera;

	void Init();
};
