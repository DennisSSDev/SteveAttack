﻿#pragma once

#include "Simplex/Mesh/Model.h"
#include "MyEntity.h"
#include "unordered_map"

class LaneGrid
{
private:

	// visual scales of the lanes
	Simplex::matrix4 transform[4];

	Simplex::matrix4 floor;

	//reference to an active projectile in the level
	Simplex::MyEntity* projectile = nullptr;
	bool projectileInLane = false;
	glm::uint projectileLane = 0;
		
	// singleton to the mesh manager
	Simplex::MeshManager* meshManager = nullptr;

	// map that stores the ids of every entity into a list associated with a particular lane
	std::unordered_map<glm::uint, std::vector<Simplex::MyEntity*>> entityLaneMap;

	Simplex::vector3 leftLaneLocation;

	Simplex::vector3 middleLaneLocation;

	Simplex::vector3 rightLaneLocation;

private:
	void Init();
	void ExplodeProjectile();
	void AddEntryInEntityMap(const glm::uint lane, Simplex::MyEntity* entity);
	
public:
	LaneGrid();
	~LaneGrid();

	static LaneGrid* Instance()
	{
		static LaneGrid* instance = new LaneGrid();
		return instance;
    }

	/**
	 * Call this function the moment you create an Entity utilizing the grid system.
	 * Once an Entity is spawned at a particular location it must be added to a lane immediately
	 */
	glm::uint AddToLane(Simplex::MyEntity* entity);

	/**
	 * Call this function the moment you spawn a projectile.
	 * It will initialize the LaneGrid update and handle the collision check for the projectile
	 */
	void AddProjectile(Simplex::MyEntity* projectile);

	/**
	 * Lane Grid custom update to detect how far away is the projectile from the grid
	 * if the Projectile is not inside the grid, it will try to check how high is the projectile.
	 * if it's not within the necessary hight -> ignore it.
	 * if it is, detect which lane it's closest to and add it to the appropriate dimension
	 * if the  
	 */
	void Update();
	
	void Display();
};