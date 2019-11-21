#include "AppClass.h"
using namespace Simplex;
//Mouse
void Application::ProcessMouseMovement(sf::Event a_event)
{
	//get global mouse position
	sf::Vector2i mouse = sf::Mouse::getPosition();
	sf::Vector2i window = m_pWindow->getPosition();
	m_v3Mouse.x = static_cast<float>(mouse.x - window.x);
	m_v3Mouse.y = static_cast<float>(mouse.y - window.y);
	if (!m_pSystem->IsWindowFullscreen() && !m_pSystem->IsWindowBorderless())
		m_v3Mouse += vector3(-8.0f, -32.0f, 0.0f);
	gui.io.MousePos = ImVec2(m_v3Mouse.x, m_v3Mouse.y);
}
void Application::ProcessMousePressed(sf::Event a_event)
{
	switch (a_event.mouseButton.button)
	{
	default: break;
	case sf::Mouse::Button::Left:
		gui.m_bMousePressed[0] = true;
		if (!projectile->GetProjectileEntity())
		{
			projectile->SpawnProjectile();
			laneGrid->AddProjectile(projectile->GetProjectileEntity());
			projectileShot.play();
		}
		break;
	case sf::Mouse::Button::Middle:
		gui.m_bMousePressed[1] = true;
		break;
	case sf::Mouse::Button::Right:
		gui.m_bMousePressed[2] = true;
		break;
	}

	for (int i = 0; i < 3; i++)
		gui.io.MouseDown[i] = gui.m_bMousePressed[i];
}
void Application::ProcessMouseReleased(sf::Event a_event)
{
	switch (a_event.mouseButton.button)
	{
	default: break;
	case sf::Mouse::Button::Left:
		gui.m_bMousePressed[0] = false;
		break;
	case sf::Mouse::Button::Middle:
		gui.m_bMousePressed[1] = false;
		break;
	case sf::Mouse::Button::Right:
		gui.m_bMousePressed[2] = false;
		break;
	}

	for (int i = 0; i < 3; i++)
		gui.io.MouseDown[i] = gui.m_bMousePressed[i];
}
void Application::ProcessMouseScroll(sf::Event a_event)
{
	gui.io.MouseWheel = a_event.mouseWheelScroll.delta;
	float fSpeed = a_event.mouseWheelScroll.delta;
	float fMultiplier = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);

	if (fMultiplier)
		fSpeed *= 2.0f;
	//Removed so you cannot move the camera at all
	//m_pCameraMngr->MoveForward(-fSpeed);
}
//Keyboard
void Application::ProcessKeyPressed(sf::Event a_event)
{
	switch (a_event.key.code)
	{
	default: break;
	case sf::Keyboard::Space:
		m_pEntityMngr->ApplyForce(vector3(0.0f, 1.0f, 0.0f), "Steve");
		break;
	case sf::Keyboard::LShift:
	case sf::Keyboard::RShift:
		m_bModifier = true;
		break;
	//This is still here just in case but now you can click the left mouse button to shoot the projectile
	case sf::Keyboard::P:
		if (!projectile->GetProjectileEntity())
		{
			projectile->SpawnProjectile();
			laneGrid->AddProjectile(projectile->GetProjectileEntity());
			projectileShot.play();
		}
		break;
	}

	//gui
	gui.io.KeysDown[a_event.key.code] = true;
	gui.io.KeyCtrl = a_event.key.control;
	gui.io.KeyShift = a_event.key.shift;
}
void Application::ProcessKeyReleased(sf::Event a_event)
{
	static bool bFPSControl = false;

	switch (a_event.key.code)
	{
	default: break;
	case sf::Keyboard::Escape:
		m_bRunning = false;
		break;
	case sf::Keyboard::F:
		bFPSControl = !bFPSControl;
		m_pCameraMngr->SetFPS(bFPSControl);
		break;
	case sf::Keyboard::PageUp:
		break;
	case sf::Keyboard::PageDown:
		break;
	case sf::Keyboard::Add:
		break;
	case sf::Keyboard::Subtract:
		break;
	case sf::Keyboard::LShift:
	case sf::Keyboard::RShift:
		m_bModifier = false;
		break;
	case sf::Keyboard::Tab: //toggle debug box view
		showDebug = !showDebug;
		for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
		{
			m_pEntityMngr->GetRigidBody(i)->SetVisibleOBB(showDebug);
		}
		break;
	}

	//gui
	gui.io.KeysDown[a_event.key.code] = false;
	gui.io.KeyCtrl = a_event.key.control;
	gui.io.KeyShift = a_event.key.shift;
}

void Application::CameraRotation(float a_fSpeed)
{
	if (!m_bFocused)
		return;

	UINT	MouseX, MouseY;		// Coordinates for the mouse
	UINT	CenterX, CenterY;	// Coordinates for the center of the screen.

	uint MaxMoveVertical, MaxMoveHorizontal; //Maximum value the camera should be able to move in either direction
	uint LeastMoveVertical, LeastMoveHorizontal; //Minimum value the camera should be able to move in either direction

								//Initialize the position of the pointer to the middle of the screen
	CenterX = m_pSystem->GetWindowX() + m_pSystem->GetWindowWidth() / 2;
	CenterY = m_pSystem->GetWindowY() + m_pSystem->GetWindowHeight() / 2;

	MaxMoveVertical = CenterX + m_pSystem->GetWindowHeight() / 2;
	MaxMoveHorizontal = CenterY + m_pSystem->GetWindowWidth() / 2;
	LeastMoveVertical = CenterX - m_pSystem->GetWindowHeight() / 2;
	LeastMoveHorizontal = CenterY - m_pSystem->GetWindowWidth() / 2;

	//Calculate the position of the mouse and store it
	POINT pt;
	GetCursorPos(&pt);
	MouseX = pt.x;
	MouseY = pt.y;

	//Calculate the difference in view with the angle
	float fAngleX = 0.0f;
	float fAngleY = 0.0f;
	float fDeltaMouse = 0.0f;
	if (MouseX < CenterX)
	{
		fDeltaMouse = static_cast<float>(CenterX - MouseX);
		fAngleY += fDeltaMouse * a_fSpeed;
	}
	else if (MouseX > CenterX)
	{
		fDeltaMouse = static_cast<float>(MouseX - CenterX);
		fAngleY -= fDeltaMouse * a_fSpeed;
	}

	if (MouseY < CenterY)
	{
		fDeltaMouse = static_cast<float>(CenterY - MouseY);
		fAngleX -= fDeltaMouse * a_fSpeed;
	}
	else if (MouseY > CenterY)
	{
		fDeltaMouse = static_cast<float>(MouseY - CenterY);
		fAngleX += fDeltaMouse * a_fSpeed;
	}

	//Change the Yaw and the Pitch of the camera
	m_pCameraMngr->ChangeYaw(fAngleY * 0.25f);
	m_pCameraMngr->ChangePitch(-fAngleX * 0.25f);
	SetCursorPos(CenterX, CenterY);//Position the mouse in the center
}
//Keyboard
void Application::ProcessKeyboard(void)
{
	if (!m_bFocused)
		return;

#pragma region Camera Position
	//Commented out to stop the camera from moving from it's default position
	/*
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		m_pCameraMngr->MoveForward(m_fMovementSpeed * fMultiplier);
	*/
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) //Just allowing myself to back up for now to see if the cube is spawned
		m_pCameraMngr->MoveForward(-m_fMovementSpeed);
	/*
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		m_pCameraMngr->MoveSideways(-m_fMovementSpeed * fMultiplier);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		m_pCameraMngr->MoveSideways(m_fMovementSpeed * fMultiplier);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		m_pCameraMngr->MoveVertical(-m_fMovementSpeed * fMultiplier);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		m_pCameraMngr->MoveVertical(m_fMovementSpeed * fMultiplier);
	*/
#pragma endregion

#pragma region Character Position
	float fDelta = m_pSystem->GetDeltaTime(0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_pEntityMngr->ApplyForce(vector3(-2.0f * fDelta, 0.0f, 0.0f), "Steve");
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_pEntityMngr->ApplyForce(vector3(2.0f * fDelta, 0.0f, 0.0f), "Steve");
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		m_pEntityMngr->ApplyForce(vector3(0.0f, 0.0f, -2.0f * fDelta), "Steve");
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_pEntityMngr->ApplyForce(vector3(0.0f, 0.0f, 2.0f * fDelta), "Steve");
	}
#pragma endregion
}