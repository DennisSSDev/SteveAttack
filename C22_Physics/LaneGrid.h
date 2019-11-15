#pragma once

#include "Simplex/Mesh/Model.h"
#include "MyEntity.h"
#include "MyEntityManager.h"
#include "unordered_map"
#include "vector"

class Projectile;

class LaneGrid
{
private:

	// visual scales of the lanes
	Simplex::matrix4 transform[3];

	Simplex::matrix4 floor;

	//reference to an active projectile in the level
	Simplex::MyEntity* projectile = nullptr;
	bool projectileInLane = false;
	glm::uint projectileLane = 0;

	bool isTimerSet = false;
	float timerValue = 0.f;
		
	// singleton to the mesh manager
	Simplex::MeshManager* meshManager = nullptr;
	Simplex::MyEntityManager* entityManager = nullptr;

	std::unordered_map<glm::uint, std::vector<Simplex::String>> entityIDLaneMap;

	Simplex::vector3 leftLaneLocation;

	Simplex::vector3 middleLaneLocation;

	Simplex::vector3 rightLaneLocation;

	std::vector<Simplex::vector3> laneLocations;

	Projectile* projectileInstance = nullptr;

	static LaneGrid* instance;

private:
	void Init();
	void ExplodeProjectile();
	
public:
	LaneGrid();
	~LaneGrid();
	
	static void ReleaseInstance();
	
	static LaneGrid* Instance()
	{
		if(!instance)
		{
			instance = new LaneGrid();
		}
		return instance;
    }

	// lane can only be either 0, 1 or 2 (0 - leftmost, 2 - rightmost from player perspective)
	const std::vector<Simplex::String>& GetEntityIDMap(const glm::uint& lane);
	

	/**
	 * Call this function the moment you create an Entity utilizing the grid system.
	 * Once an Entity is spawned at a particular location it must be added to a lane immediately
	 */
	glm::uint AddToLane(Simplex::String entityID);

	/**
	 * Call this function the moment you spawn a projectile.
	 * It will initialize the LaneGrid update and handle the collision check for the projectile
	 */
	void AddProjectile(Simplex::MyEntity* projectile);

	void SetProjectileReference(Projectile* instance);

	/**
	 * Lane Grid custom update to detect how far away is the projectile from the grid
	 * if the Projectile is not inside the grid, it will try to check how high is the projectile.
	 * if it's not within the necessary height -> ignore it.
	 * if it is, detect which lane it's closest to and add it to the appropriate dimension
	 * if the  
	 */
	void Update();
	
	void Display();
};
