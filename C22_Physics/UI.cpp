#include "UI.h"
#include <iostream>
using namespace Simplex;

UI* UI::instance = nullptr;

UI* UI::Instance()
{
	if (!instance)
	{
		instance = new UI();
	}
	return instance;
}

UI::UI()
{
	killCount = 0;
	score = 0;
	isAlive = true;

	uiInfo = new std::string[uiInfoNum];
}

Simplex::UI::~UI()
{
	delete[] uiInfo;
	uiInfo = nullptr;

	delete instance;
	instance = nullptr;
}

void UI::AddToScore(int s)
{
	score += s;
}

void UI::KillEnemy()
{
	killCount++;
}

void UI::PlayerDied()
{
	isAlive = false;
}

bool UI::IsAlive()
{
	return isAlive;
}

std::string* UI::GetGuiInfo()
{
	uiInfo[0] = "Score: " + std::to_string(score);
	uiInfo[1] = "Kill Count: " + std::to_string(killCount);
	return uiInfo;
}
