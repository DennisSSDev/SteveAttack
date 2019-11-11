#include "Simplex/Mesh/Model.h"
#include "MyEntity.h"
#include "LaneGrid.h"

class Projectile
{
public:
	Projectile();
	~Projectile();
	void SpawnProjectile();

private:
	Simplex::vector3 spawnPoint;

	void Init();
};
