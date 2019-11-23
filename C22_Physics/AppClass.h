/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2017/06
----------------------------------------------*/
#ifndef __APPLICATIONCLASS_H_
#define __APPLICATIONCLASS_H_

#include "Definitions.h"

#include "ControllerConfiguration.h"
#include "imgui\ImGuiObject.h"
#include "LaneGrid.h"
#include "MobManager.h"
#include "Projectile.h"

#include "MyEntityManager.h"

namespace Simplex
{
//Adding Application to the Simplex namespace
class Application
{
	MyEntityManager* m_pEntityMngr = nullptr; //Entity Manager
    
private:
	String m_sProgrammer1 = "Dennis Slavinsky - dxs4043@rit.edu"; //programmer1
	String m_sProgrammer2 = "Vincent Bennett - mmb7755@rit.edu"; //programmer 2
	String m_sProgrammer3 = "Ruben Young - "; //programmer 3
	String m_sProgrammer4 = "Anthony Ferraioli - amf7619@rit.edu"; //programmer 4

	static ImGuiObject gui; //GUI object
	bool m_bGUI_Main = true; //show Main GUI window?
	bool m_bGUI_Console = true; //show Credits GUI window?
	bool m_bGUI_Test = false; //show Test GUI window?
	bool m_bGUI_Controller = false; //show Controller GUI window?

	uint m_uRenderCallCount = 0; //count of render calls per frame
	uint m_uControllerCount = 0; //count of controllers connected

	bool m_bFocused = true; //is the window focused?

	float m_fMovementSpeed = 0.1f; //how fast the camera will move

	vector3 m_v3Mouse = vector3(); //position of the mouse in the window

	vector4 m_v4ClearColor; //Color of the scene
	bool m_bRunning = false; //Is App running?
	bool m_bModifier = false; //is shift pressed?

	sf::Window* m_pWindow = nullptr; //SFML window
	SystemSingleton* m_pSystem = nullptr; //Singleton of the system
	LightManager* m_pLightMngr = nullptr; //Light Manager of the system
	MeshManager* m_pMeshMngr = nullptr; //Mesh Manager
	CameraManager* m_pCameraMngr = nullptr; //Singleton for the camera manager
	UI* ui = nullptr;
	String* uiInfo = nullptr;
	ControllerInput* m_pController[8]; //Controller
	uint m_uActCont = 0; //Active Controller of the Application

	sf::SoundBuffer projectileBuffer; //buffer to play sound from
	sf::Sound projectileShot;
	sf::SoundBuffer zombieBuffer;
	sf::Sound zombieDeath;
	sf::Music m_soundBGM; //background music

	bool showDebug = false; //displays debug box on left

	LaneGrid* laneGrid;
    MobManager* m_pSteveMngr;
	Projectile* projectile;

public:
#pragma region Constructor / Run / Destructor
	/*
	USAGE: Constructor
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	Application();
	/*
	USAGE: Initializes the window and rendering context
	ARGUMENTS:
	-	String a_sApplicationName -> Name of the window if blank will use project Name
	-	int size -> formated size, relate to BTO_RESOLUTIONS
	-	bool a_bFullscreen = false -> is the window fullscreen?
	-	bool a_bBorderless = false -> is the window borderless?
	OUTPUT: ---
	*/
	void Init(String a_sApplicationName = "", int a_uSize = BTO_RESOLUTIONS::RES_C_1280x720_16x9_HD,
		bool a_bFullscreen = false, bool a_bBorderless = false);
	/*
	USAGE: Initializes the window and rendering context
	ARGUMENTS:
	-	String a_sApplicationName = "" -> Name of the window if blank will use project Name
	-	uint a_nWidth -> Window Width
	-	uint a_nHeight -> Window Height
	-	bool a_bFullscreen -> is the window fullscreen?
	-	bool a_bBorderless -> is the window borderless?
	OUTPUT: ---
	*/
	void Init(String a_sApplicationName, uint a_uWidth, uint a_uHeight, bool a_bFullscreen, bool a_bBorderless);
	/*
	USAGE: Runs the main loop of this class DO NOT OVERRIDE
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Run(void);
	/*
	USAGE: Destructor
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	~Application(void);
#pragma endregion

private:
#pragma region Initialization / Release
	/*
	USAGE: Initialize the window
	ARGUMENTS: String a_sWindowName = "GLFW" -> Window name
	OUTPUT: ---
	*/
	void InitWindow(String a_sWindowName = "Application");
	/*
	USAGE: Initializes user specific variables, this is executed right after InitWindow,
	the purpose of this member function is to initialize member variables specific for this project.
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void InitVariables(void);
	/*
	USAGE: Reads the configuration of the application to a file
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void ReadConfig(void);
	/*
	USAGE: Writes the configuration of the application to a file
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void WriteConfig(void);
	/*
	USAGE: Releases the application
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Release(void);
#pragma endregion

#pragma region Main Loop
	/*
	USAGE: Updates the scene
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Update(void);
	/*
	USAGE: Displays the scene
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Display(void);
	/*
	USAGE: Clears the OpenGL screen by the specified color
	ARGUMENTS: vector4 a_v4ClearColor = vector4(-1.0f) -> Color to clear the screen with
	OUTPUT: ---
	*/
	void ClearScreen(vector4 a_v4ClearColor = vector4(-1.0f));
	/*
	USAGE: Will initialize the controllers generically
	ARGUMENTS:
	OUTPUT: ---
	*/
	void InitControllers(void);
	/*
	USAGE: Will Release the controllers
	ARGUMENTS:
	OUTPUT: ---
	*/
	void ReleaseControllers(void);
#pragma endregion

#pragma region Application Controls
	/*
	USAGE: Manage constant keyboard state
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void ProcessKeyboard(void);
	/*
	USAGE: Manages the rotation of the camera a_fSpeed is a factor of change
	ARGUMENTS: float a_fSpeed = 0.005f
	OUTPUT: ---
	*/
	void CameraRotation(float a_fSpeed = 0.005f);
#pragma endregion

#pragma region Process Events
	/*
	USAGE: Resizes the window
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Reshape(void);
	/*
	USAGE: Manage the response of key presses
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void ProcessKeyPressed(sf::Event a_event);
	/*
	USAGE: Manage the response of key releases
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void ProcessKeyReleased(sf::Event a_event);
	/*
	USAGE: Manage the response of mouse movement
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void ProcessMouseMovement(sf::Event a_event);
	/*
	USAGE: Manage the response of mouse key presses
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void ProcessMousePressed(sf::Event a_event);
	/*
	USAGE: Manage the response of mouse key release
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void ProcessMouseReleased(sf::Event a_event);
	/*
	USAGE: Manage the response of mouse scrolling
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void ProcessMouseScroll(sf::Event a_event);
#pragma endregion

#pragma region GUI
	/*
	USAGE: Initialize gui
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void InitIMGUI(void);
	/*
	USAGE: Draw GUI elements
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void DrawGUI(void);
	/*
	USAGE: //release gui
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void ShutdownGUI(void);
	/*
	USAGE: Render gui lists
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	static void RenderDrawLists(ImDrawData* draw_data);
	/*
	USAGE: Creates fonts for gui
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	bool CreateFontsTexture(void);
	/*
	USAGE: Create OpenGL resources for gui
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	bool CreateDeviceObjects(void);
	/*
	USAGE: creates a new frame for gui
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void NewFrame(void);
#pragma endregion

#pragma region The Rule of Three
	/*
	USAGE: copy constructor, private so it does not let object copy
	ARGUMENTS: GLFWApp const& input -> object to copy (well in this case not)
	OUTPUT: ---
	*/
	Application(Application const& input);
	/*
	USAGE: copy assignment, private so it does not let object copy
	ARGUMENTS: GLFWApp const& input -> object to copy (well in this case not)
	OUTPUT: ---
	*/
	Application& operator=(Application const& input);
#pragma endregion
};

}//namespace Simplex

#endif //__APPLICATIONCLASS_H_

 /*
 USAGE:
 ARGUMENTS: ---
 OUTPUT: ---
 */