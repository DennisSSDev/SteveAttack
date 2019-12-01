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
#include <vector>
#include <queue>

namespace Simplex{
class MobManager
{
    uint m_ActiveMobCount = 0;
    uint m_nTotalMobs = 0;
    LaneGrid* m_pLaneGrid = nullptr;
    MyEntityManager* m_pEntityManager = nullptr;

public:
    // Default Constructor and Destructor
    MobManager();
    ~MobManager();

    void Init();

    // This method handles: 
    // - Moving Steves forward
    // - Spawning new Steves (if necessary)
    void Update(float dt, float totalTime);

    #pragma region Singleton-specific method definitions + implementation
    // Allows external agents access to the SteveManager singleton through static pointer
    static MobManager* GetInstance();

    static void ReleaseInstance();
    #pragma endregion

private: // Methods

    // Steve-Spawning
    void SpawnMob(vector3 a_position);

    // Decides (By RNG) what mob to spawn, returns the obj filename and mass associated with that mob through parameters
    void GetMobInfo(_Out_ String* r_fileName, _Out_ float* mass);

    // Used inside Init() to initially populate the lane grid 
    void SpawnInitialMobs();

    // Query individual lane lists, spawning mobs as needed and maintaining the lists as balanced
    void CheckNeededSpawns();

    // Runs through the spawn queue and spawns a mob at each location
    void FlushSpawnQueue();

private: // Members
    static MobManager* m_pInstance;
    
    // Collection of spawn points and amount of points per lane
    std::vector<vector3> m_spawnPoints;
    const uint m_nSpawnPointsPerLane = 4U;

    // The backlog of mob locations to spawn a mob at
    std::queue<vector3> m_ToSpawnQueue;
};
}
#endif