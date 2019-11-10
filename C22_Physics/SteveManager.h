/*----------------------------------------------
Programmer: Ruben Young (ray6190@rit.edu)
Date: 2019/11
----------------------------------------------*/
#ifndef STEVEMANAGER_H
#define STEVEMANAGER_H

#include "Definitions.h"
#include "LaneGrid.h"


namespace Simplex{
class SteveManager
{
    uint m_steveCount = 0;
    LaneGrid* m_pLaneGrid = nullptr;

public:
    // Default Constructor and Destructor
    SteveManager();
    ~SteveManager();

    void Init();

    // This method handles:
    // - Checking for dead Steves 
    // - Moving Steves forward
    // - Spawning new Steves (if necessary)
    void Update();

    #pragma region Singleton-specific method definitions + implementation
    // Allows external agents access to the SteveManager singleton through static pointer
    static SteveManager * GetInstance()
    {
        if (m_pInstance == nullptr)
        {
            m_pInstance = new SteveManager();
        }
        return m_pInstance;
    }

    static void ReleaseInstance()
    {
        if (m_pInstance != nullptr)
        {
            delete m_pInstance;
            m_pInstance = nullptr;
        }
    }
    #pragma endregion

private: // Methods

    // Steve-Spawning Methods + Overrides
    void SpawnSteve(uint a_laneIndex = 0);  // Random direction vector
    void SpawnSteve(vector3 a_direction, uint a_laneIndex = 0); // Specific direction vector

    void UpdateSteveCollisions();      // Bounce steves off each other
    void UpdateProjectileCollisions(); // Cleanup steves hit by artillery

private: // Members
    static SteveManager* m_pInstance;
};
}


#endif