/*----------------------------------------------
Programmer: Ruben Young (ray6190@rit.edu)
Date: 2019/11
----------------------------------------------*/
#ifndef STEVEMANAGER_H
#define STEVEMANAGER_H

#include "Definitions.h"
#include "Simplex/Mesh/Model.h"
#include "MyEntity.h"


namespace Simplex{
class SteveManager
{
    uint m_steveCount;

public:
    // Default Constructor and Destructor
    SteveManager();
    ~SteveManager();

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

private:
    static SteveManager* m_pInstance;
};
}


#endif