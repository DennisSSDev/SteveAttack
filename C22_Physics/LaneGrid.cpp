#include "LaneGrid.h"
#include <unordered_set>
using namespace Simplex;

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
	for (const auto t : transform)
	{
		vector3 scale;
		quaternion rotation;
		vector3 translation;
		vector3 skew;
		glm::vec4 perspective;
		glm::decompose(t, scale, rotation, translation, skew, perspective);
		glm::vec3 temp = entity->GetPosition() - translation;
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

LaneGrid::~LaneGrid()
{
	
}

void LaneGrid::Init()
{
	meshManager = Simplex::MeshManager::GetInstance();
	entityManager = Simplex::MyEntityManager::GetInstance();

	leftLaneLocation = vector3(-10.f,2.75f,0.f);
	middleLaneLocation = vector3(0.f,2.75f,0.f);
	rightLaneLocation = vector3(10.f,2.75f,0.f);

	entityIDLaneMap[0] = std::vector<String>();
	entityIDLaneMap[1] = std::vector<String>();
	entityIDLaneMap[2] = std::vector<String>();
	
	// left
	transform[0] = (glm::translate(IDENTITY_M4, leftLaneLocation) * glm::scale(Simplex::vector3(8.f, 5.f, 25.f)));
	// middle 
	transform[1] = (glm::translate(IDENTITY_M4, middleLaneLocation) * glm::scale(Simplex::vector3(8.f, 5.f, 25.f)));
	// right
	transform[2] = (glm::translate(IDENTITY_M4, rightLaneLocation) * glm::scale(Simplex::vector3(8.f, 5.f, 25.f)));

	floor = (glm::translate(IDENTITY_M4, vector3(0.f,-.5f,0.f)) * glm::scale(Simplex::vector3(27.f, 1.f, 26.f)));
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
		if(distSq < 10.f) // todo: this is where you'd access the projectile's range of effect
		{
			toDeleteEntities.push_back(entityID);
		}
		else
		{
			newEntityList.push_back(entityID);
		}
	}
	// todo: request AI system to remove the garbo entities. It just so happens that the ball with also be in the list of garbage

	for (const auto& entityID : toDeleteEntities)
	{
		entityManager->RemoveEntity(entityID);
	}

	projectileInLane = false;
	projectile = nullptr;
	
	// swap with the list of entities that are still valid
	entityIDLaneMap[projectileLane] = newEntityList;
}

void LaneGrid::Update()
{
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
		ExplodeProjectile();
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
