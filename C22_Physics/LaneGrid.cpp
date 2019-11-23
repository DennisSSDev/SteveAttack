﻿#include "LaneGrid.h"
#include "Projectile.h"
#include <unordered_set>
#include "MobManager.h"

using namespace Simplex;

LaneGrid* LaneGrid::instance = nullptr;

LaneGrid::LaneGrid()
{
	Init();
}

const std::vector<Simplex::String>& LaneGrid::GetEntityIDMap(const uint& lane)
{
	return entityIDLaneMap[lane];
}

uint LaneGrid::AddToLane(Simplex::String entityID)
{
	uint closestBox = 0;
	uint trackedBox = 0;
	float closestDistance = 100000000000.f;
	auto entity = entityManager->GetEntity(entityManager->GetEntityIndex(entityID));
	for (const auto t : laneLocations)
	{
		glm::vec3 temp = entity->GetPosition() - t;
		const float distSqr = dot(temp, temp);
		if(distSqr < closestDistance)
		{
			closestDistance = distSqr;
			closestBox = trackedBox;
		}
		++trackedBox;
	}
	entity->ClearDimensionSet();
	entity->AddDimension(closestBox);
	entityIDLaneMap[closestBox].push_back(entityID);
	return closestBox;
}

void LaneGrid::AddProjectile(Simplex::MyEntity* projectile)
{
	// get a copy of the pointer so that now you will be responsible for checking distance from the ground
	this->projectile = projectile;
}

void LaneGrid::SetProjectileReference(Projectile* instance)
{
	projectileInstance = instance;
}

LaneGrid::~LaneGrid()
{
}

void LaneGrid::Init()
{
	meshManager = Simplex::MeshManager::GetInstance();
	entityManager = Simplex::MyEntityManager::GetInstance();
	uiInstance = Simplex::UI::Instance();

	leftLaneLocation = vector3(-10.f,2.75f,0.f);
	middleLaneLocation = vector3(0.f,2.75f,0.f);
	rightLaneLocation = vector3(10.f,2.75f,0.f);

	entityIDLaneMap[0] = std::vector<String>();
	entityIDLaneMap[1] = std::vector<String>();
	entityIDLaneMap[2] = std::vector<String>();
	
    laneScaleFactor = Simplex::vector3(8.f, 5.f, 25.f);
	// left
    transform[0] = (glm::translate(IDENTITY_M4, leftLaneLocation) * glm::scale(laneScaleFactor));
	// middle 
	transform[1] = (glm::translate(IDENTITY_M4, middleLaneLocation) * glm::scale(laneScaleFactor));
	// right
	transform[2] = (glm::translate(IDENTITY_M4, rightLaneLocation) * glm::scale(laneScaleFactor));

    floorScaleFactor = Simplex::vector3(27.f, 1.f, 26.f);
	floor = (glm::translate(IDENTITY_M4, vector3(0.f,-.5f,0.f)) * glm::scale(floorScaleFactor));

	laneLocations.push_back(leftLaneLocation);
	laneLocations.push_back(middleLaneLocation);
	laneLocations.push_back(rightLaneLocation);
}

void LaneGrid::ExplodeProjectile()
{
	const auto& entityIDs = entityIDLaneMap[projectileLane];
	const vector3 projLocation = projectile->GetPosition();

	std::vector<Simplex::String> toDeleteEntities;
	std::vector<Simplex::String> newEntityList;

	
	for (const auto& entityID : entityIDs)
	{
		const auto entity = entityManager->GetEntity(entityManager->GetEntityIndex(entityID));
		const vector3 distV = entity->GetPosition() - projLocation;
		const float distSq = dot(distV, distV);
		const float& expRadius = projectileInstance->GetExplosionRadius();
		if(distSq < expRadius * expRadius)
		{
			toDeleteEntities.push_back(entityID);
		}
		else
		{
			newEntityList.push_back(entityID);
		}
	}
	
	for (const auto& entityID : toDeleteEntities)
	{
		entityManager->RemoveEntity(entityID);
	}

	projectileInLane = false;

	projectileInstance->InvalidateProjectile();
	
	projectile = nullptr;
	
	uiInstance->KillEnemies(toDeleteEntities.size()-1);
	uiInstance->AddToScore(1);
	// swap with the list of entities that are still valid
	entityIDLaneMap[projectileLane] = newEntityList;
}

void LaneGrid::ReleaseInstance()
{
	if(instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void LaneGrid::Update(float delta)
{
	// todo: check distance against an arbitrary value and if any of the steves crosses it -> you lose
	for (uint i = 0; i < 3; ++i)
	{
		const auto& entityIDs = entityIDLaneMap[i];
		for (const auto& entityID : entityIDs)
		{
			const auto entity = entityManager->GetEntity(entityManager->GetEntityIndex(entityID));
			if (distanceToEnd - entity->GetPosition().z < 0.f) 
			{
				uiInstance->PlayerDied();
				break;
			}
		}
	}

	if(!projectile) return;
	
	const auto projLocation = projectile->GetPosition();
	if(projLocation.y > middleLaneLocation.y + 1.f) return;
	if(!projectileInLane)
	{
		projectileInLane = true;
		// add it to a dimension
		projectileLane = AddToLane(projectile->GetUniqueID());
		return;
	}
	// check if close to ground / hit the ground
	// if it hit the ground -> explode and invalidate the projectile
	if(projLocation.y < 0.01f)
	{
		if(!isTimerSet)
		{
			isTimerSet = true;
			timerValue = 0.f;
			return;
		}
		timerValue += 15.f*delta;
		if(timerValue > 10.f)
		{
			ExplodeProjectile();
			isTimerSet = false;
		}
	}
}

void LaneGrid::Display()
{
	meshManager->AddWireCubeToRenderList(transform[0], C_YELLOW, RENDER_WIRE);
	meshManager->AddWireCubeToRenderList(transform[1], C_YELLOW, RENDER_WIRE);
	meshManager->AddWireCubeToRenderList(transform[2], C_YELLOW, RENDER_WIRE);
	meshManager->AddCubeToRenderList(floor, C_GRAY);
	meshManager->AddCubeToRenderList(floor, C_GREEN, RENDER_WIRE);
}
