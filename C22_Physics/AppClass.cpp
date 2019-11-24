#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 5.0f, 25.0f), //Position
		vector3(0.0f, 0.0f, 0.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

	m_pEntityMngr->AddEntity("Minecraft\\Steve.obj", "Steve");
	m_pEntityMngr->UsePhysicsSolver();

	m_pEntityMngr->AddEntity("Minecraft\\Cube.obj", "Cube_0");
	vector3 v3Position = vector3(100, 0.f, 100.f);
	v3Position.y = 0.0f;
	matrix4 m4Position = glm::translate(v3Position);
	m_pEntityMngr->SetModelMatrix(m4Position * glm::scale(vector3(2.0f)));

	if (!projectileBuffer.loadFromFile("Data/Audio/BombExplosionShort.wav"))
	{
		std::cout << "AUDIO FILE COULD NOT BE FOUND" << std::endl;
	}
	else
	{
		projectileShot.setBuffer(projectileBuffer);
	}

	if (!zombieBuffer.loadFromFile("Data/Audio/minecraftOOF.mp3"))
	{
		std::cout << "AUDIO FILE COULD NOT BE FOUND" << std::endl;
	}
	else
	{
		zombieDeath.setBuffer(zombieBuffer);
	}

	laneGrid = LaneGrid::Instance();
    m_pSteveMngr = MobManager::GetInstance();
	projectile = new Projectile();
	laneGrid->SetProjectileReference(projectile);
	const uint steveIndex = m_pEntityMngr->GetEntityIndex("Steve");
	laneGrid->SetHelperSteveReference(m_pEntityMngr->GetEntity(steveIndex));
	ui = UI::Instance();
	uiInfo = ui->GetGuiInfo();
    m_pSteveMngr->Init();
}
void Application::Update(void)
{	
	float delta = m_pSystem->GetDeltaTime(18);

	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the first person camera active?
	CameraRotation();

    // Update the AI Manager
    m_pSteveMngr->Update(delta);

    //Update Entity Manager
	m_pEntityMngr->Update();

	//Set the model matrix for the main object
	//m_pEntityMngr->SetModelMatrix(m_m4Steve, "Steve");

	laneGrid->Update(delta);

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
	//m_pEntityMngr->AddEntityToRenderList(-1, true);
}

void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	laneGrid->Display(showDebug);

	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui,
	DrawGUI();

	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//Release MyEntityManager
	MyEntityManager::ReleaseInstance();
	LaneGrid::ReleaseInstance();
	SafeDelete(projectile);
    MobManager::ReleaseInstance();
	UI::ReleaseInstance();
	//release GUI
	ShutdownGUI();
}