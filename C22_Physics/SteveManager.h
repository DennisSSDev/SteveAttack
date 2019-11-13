/*----------------------------------------------
Programmer: Ruben Young (ray6190@rit.edu)
Date: 2019/11
----------------------------------------------*/
#ifndef STEVEMANAGER_H
#define STEVEMANAGER_H

#include "Definitions.h"
#include "MyEntityManager.h"
#include "LaneGrid.h"
#include <unordered_map>

namespace Simplex{
class SteveManager
{
    uint m_steveCount = 0;
    uint m_nTotalSteves = 0;
    LaneGrid* m_pLaneGrid = nullptr;
    MyEntityManager* m_pEntityManager = nullptr;

    // This is a mapping of the unique id of a steve with its associated direction vector
    // We need to use uniqueids rather than myentity* so that MyEntityManager::RemoveEntity(uniqueID) plays nice with us
    std::unordered_map<String, vector3> m_aliveSteves;

public:
    // Default Constructor and Destructor
    SteveManager();
    ~SteveManager();

    void Init(uint a_initialSteveCount);

    // This method handles: 
    // - Moving Steves forward
    // - Spawning new Steves (if necessary)
    void Update(float dt);

    #pragma region Singleton-specific method definitions + implementation
    // Allows external agents access to the SteveManager singleton through static pointer
    static SteveManager* GetInstance();

    static void ReleaseInstance();
    #pragma endregion

private: // Methods

    // Steve-Spawning
    void SpawnSteve(vector3 a_position);

    // Used inside Init() to initially populate the lane grid 
    void SpawnInitialSteves(uint a_initialSteveCount);

private: // Members
    static SteveManager* m_pInstance;
};
}


#endif