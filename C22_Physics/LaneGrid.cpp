#include "LaneGrid.h"
#include <unordered_set>
using namespace Simplex;

LaneGrid::LaneGrid()
{
	Init();
}

uint LaneGrid::AddToLane(Simplex::MyEntity* entity)
{
	uint closestBox = 0;
	uint trackedBox = 0;
	float closestDistance = 100000000000.f;
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
	// todo: add the entity to the floor dimension
	entity->AddDimension(closestBox);
	
	return closestBox;
}

void LaneGrid::AddProjectile(Simplex::MyEntity* projectile)
{
	// get a copy of the pointer so that now you will be responsible for checking distance from the ground
	this->projectile = projectile;
}

LaneGrid::~LaneGrid()
{
	SafeDelete(projectile);
}

void LaneGrid::Init()
{
	meshManager = Simplex::MeshManager::GetInstance();

	leftLaneLocation = vector3(-10.f,2.75f,0.f);
	middleLaneLocation = vector3(0.f,2.75f,0.f);
	rightLaneLocation = vector3(10.f,2.75f,0.f);

	entityLaneMap[0] = std::vector<MyEntity*>();
	entityLaneMap[1] = std::vector<MyEntity*>();
	entityLaneMap[2] = std::vector<MyEntity*>();
	
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
	const auto& entities = entityLaneMap[projectileLane];
	const vector3 projLocation = projectile->GetPosition();

	std::vector<MyEntity*> toDeleteEntities;
	std::vector<MyEntity*> newEntityList;
	
	for (const auto& entity : entities)
	{
		const vector3 distV = entity->GetPosition() - projLocation;
		const float distSq = dot(distV, distV);
		if(distSq < 10000.f) // todo: this is where you'd access the projectile's range of effect
		{
			toDeleteEntities.push_back(entity);
		}
		else
		{
			newEntityList.push_back(entity);
		}
	}
	// todo: request AI system to remove the garbo entities. It just so happens that the ball with also be in the list of garbage

	projectileInLane = false;
	projectile = nullptr;
	
	// swap with the list of entities that are still valid
	entityLaneMap[projectileLane] = newEntityList;
}

void LaneGrid::AddEntryInEntityMap(const uint lane, Simplex::MyEntity* entity)
{
	if(lane > 2)
	{
		// cause the program to crash if the lane is larger than the allowed range
		assert(false);
	}
	entityLaneMap[lane].push_back(entity);
}

void LaneGrid::Update()
{
	if(!projectile) return;
	
	const auto projLocation = projectile->GetPosition();
	if(projLocation.y > middleLaneLocation.y + 5.f) return;
	if(!projectileInLane)
	{
		projectileInLane = true;
		// add it to a dimension
		projectileLane = AddToLane(projectile);
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
