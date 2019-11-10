/*----------------------------------------------
Programmer: Ruben Young (ray6190@rit.edu)
Date: 2019/11
----------------------------------------------*/
#include "SteveManager.h"
namespace Simplex {

// Initializes steve count to 0 and grabs handle to lane grid singleton
SteveManager::SteveManager() : 
    m_steveCount(0),
    m_pLaneGrid(LaneGrid::Instance())
{}

SteveManager::~SteveManager()
{

}

}

