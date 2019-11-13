/*----------------------------------------------
Programmer: Ruben Young (ray6190@rit.edu)
Date: 2019/11
----------------------------------------------*/
#include "SteveManager.h"
namespace Simplex {

SteveManager* SteveManager::m_pInstance = nullptr;

// Initializes steve count to 0 and grabs handle to lane grid singleton
SteveManager::SteveManager() :
    m_steveCount(0U),
    m_nTotalSteves(0U),
    m_pLaneGrid(LaneGrid::Instance()),
    m_pEntityManager(MyEntityManager::GetInstance())
{}

SteveManager::~SteveManager()
{}

void SteveManager::Update(float dt)
{
    for (const auto entity : m_aliveSteves)
    {
        m_pEntityManager->ApplyForce(entity.second * dt, entity.first);
    }
}

void SteveManager::Init(uint a_initialSteveCount)
{
    SpawnInitialSteves(a_initialSteveCount);
}

void SteveManager::SpawnSteve(vector3 a_position)
{
    // TODO: Generate variance in direction vectors 
    // for now, the vector <0,0,1> will suffice
    vector3 v3Direction(0.0f, 0.0f, 1.0f);

    // Generate uniqueID to pass to the entityManager
    std::ostringstream oss;
    oss << "Steve" << m_nTotalSteves++;
    String uniqueID = oss.str();

    // Link together uniqueID to direction vector in map
    m_aliveSteves[uniqueID] = v3Direction;

    // Load the entity in the entity manager
    m_pEntityManager->AddEntity("Minecraft\\Steve.obj", uniqueID);
    m_pEntityManager->UsePhysicsSolver(); // Resolve any upstanding collisions with this steve

    // Retrieve entity* from manager and set the world space position
    MyEntity* pEntity = m_pEntityManager->GetEntity(m_pEntityManager->GetEntityIndex(uniqueID));
    pEntity->GetSolver()->SetPosition(a_position);

    // Give info to the lane grid
    pEntity->ClearDimensionSet();
    m_pLaneGrid->AddToLane(pEntity);

    ++m_steveCount;
}

void SteveManager::SpawnInitialSteves(uint a_initialSteveCount)
{
    //for (uint i = 0U; i < a_initialSteveCount; ++i)
    //{
    //    // TODO: Distribute steves across floor (glm::gaussRand?)
    //
    //    // SpawnSteve( distributed vector );
    //}

    // For now, spawn three steves, one in each lane
    SpawnSteve(vector3(-10, -.5f, -3.f));
    SpawnSteve(vector3( 0, -.5f, -3.f));
    SpawnSteve(vector3( 10, -.5f, -3.f));
}

#pragma region Singleton-specific method definitions + implementation

// Allows external agents access to the SteveManager singleton through static pointer
SteveManager* SteveManager::GetInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new SteveManager();
    }
    return m_pInstance;
}

void SteveManager::ReleaseInstance()
{
    if (m_pInstance != nullptr)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}
#pragma endregion
}

