/*----------------------------------------------
Programmer: Ruben Young (ray6190@rit.edu)
Date: 2019/11
----------------------------------------------*/
#include "SteveManager.h"
#include <stdlib.h> // rand
#include "Projectile.h" // projectile
#include <time.h>   // time(NULL)

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
    const MyEntity* projectile = m_pLaneGrid->GetProjectileReference()->GetProjectileEntity();
    const vector3 force(0.f, 0.f, 5.0f * dt);
    for (int i = 0; i < 3; ++i) // for every lane
    {
        for (const auto entityID : m_pLaneGrid->GetEntityIDMap(i)) // for every entity in lane i
        {
            const MyEntity* entity = m_pEntityManager->GetEntity(m_pEntityManager->GetEntityIndex(entityID));
            
            if (entity != projectile)
            m_pEntityManager->ApplyForce(force, entityID);
        }
    }
    
}

void SteveManager::Init(uint a_initialSteveCount)
{
    SpawnInitialSteves(a_initialSteveCount);
}

void SteveManager::SpawnMob(vector3 a_position)
{
    // Generate uniqueID to pass to the entityManager
    std::ostringstream oss;
    oss << "Mob" << m_nTotalSteves++;
    String uniqueID = oss.str();

    // Load the entity in the entity manager
    String tFileName;
    float tMass;
    GetMobInfo(&tFileName, &tMass);

    // Initialize Entity with initial values
    m_pEntityManager->AddEntity(tFileName, uniqueID);
    m_pEntityManager->SetPosition(a_position, uniqueID);
    m_pEntityManager->SetMass(tMass, uniqueID);
	m_pEntityManager->GetRigidBody(uniqueID)->SetVisibleOBB(false);
    m_pEntityManager->UsePhysicsSolver(); // Resolve any upstanding collisions with this steve

    // Give info to the lane grid
    m_pLaneGrid->AddToLane(uniqueID);

    ++m_steveCount;
}

void SteveManager::GetMobInfo(_Out_ String* r_fileName, _Out_ float* r_mass)
{
    // Random integer E [1,100]
    int randomNumber = rand() % 100 + 1;

    /* All possible mob cases */
    if (randomNumber < 50)
    {
        *r_fileName = "Minecraft\\Steve.obj";
        *r_mass = 1.f;
    } 
    else
    {
        *r_fileName = "Minecraft\\Zombie.obj";
        *r_mass = 2.f;
    }
        
    return;
}

void SteveManager::SpawnInitialSteves(uint a_initialSteveCount)
{
    for (uint i = 0U; i < a_initialSteveCount; ++i)
    {
        float xValue = rand() % 20 - 10; // Random float [-10,10]

        SpawnMob(vector3(xValue, -.5f, -3.f));
    }
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

