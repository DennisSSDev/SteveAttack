/*----------------------------------------------
Programmer: Ruben Young (ray6190@rit.edu)
Date: 2019/11
----------------------------------------------*/
#include "SteveManager.h"
#include <stdlib.h>
#include <time.h>

namespace Simplex {

SteveManager* SteveManager::m_pInstance = nullptr;

// Initializes steve count to 0 and grabs handle to lane grid singleton
SteveManager::SteveManager() :
    m_steveCount(0U),
    m_nTotalSteves(0U),
    m_pLaneGrid(LaneGrid::Instance()),
    m_pEntityManager(MyEntityManager::GetInstance())
{
    // Seed random
    srand(time(NULL));
}

SteveManager::~SteveManager()
{}

void SteveManager::Update(float dt)
{

    /*for (const auto entity : m_pLaneGrid->GetEntityIDMap())
    {
        m_pEntityManager->ApplyForce(vector3(0.f,0.f,1.f) * dt, entity.second);
    }*/
}

void SteveManager::Init(uint a_initialSteveCount)
{
    SpawnInitialSteves(a_initialSteveCount);
}

void SteveManager::SpawnMob(vector3 a_position)
{
    // TODO: Generate variance in direction vectors 
    // for now, the vector <0,0,1> will suffice
    vector3 v3Direction(0.0f, 0.0f, 1.0f);

    // Generate uniqueID to pass to the entityManager
    std::ostringstream oss;
    oss << "Steve" << m_nTotalSteves++;
    String uniqueID = oss.str();

    // Load the entity in the entity manager
    String tFileName;
    uint tMass;
    GetMobInfo(&tFileName, &tMass);
    m_pEntityManager->AddEntity(tFileName, uniqueID);
    m_pEntityManager->UsePhysicsSolver(); // Resolve any upstanding collisions with this steve

    // Retrieve entity* from manager and set the world space position and mass
    MyEntity* pEntity = m_pEntityManager->GetEntity(m_pEntityManager->GetEntityIndex(uniqueID));
    pEntity->GetSolver()->SetPosition(a_position);
    pEntity->SetMass(tMass);

    // Give info to the lane grid
    m_pLaneGrid->AddToLane(uniqueID);

    ++m_steveCount;
}

void SteveManager::GetMobInfo(_Out_ String* r_fileName, _Out_ uint* r_mass)
{
    // Random integer E [1,100]
    int randomNumber = rand() % 100 + 1;

    /* All possible mob cases */
    if (randomNumber < 50)
    {
        *r_fileName = "Minecraft\\Steve.obj";
        *r_mass = 10;
    } 
    else
    {
        *r_fileName = "Minecraft\\Zombie.obj";
        *r_mass = 20;
    }
        
    return;
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
    SpawnMob(vector3(-10, -.5f, -3.f));
    SpawnMob(vector3( 0, -.5f, -3.f));
    SpawnMob(vector3( 10, -.5f, -3.f));
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

