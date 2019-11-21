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

void Simplex::UI::ReleaseInstance()
{
	delete instance;
	instance = nullptr;
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
}

void UI::AddToScore(int s)
{
	score += s;
}

void UI::KillEnemy()
{
	killCount++;
}

void Simplex::UI::KillEnemies(int count)
{
	killCount += count;
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
	uiInfo[0] = "Projectiles Exploded: " + std::to_string(score);
	uiInfo[1] = "Kill Count: " + std::to_string(killCount);
	return uiInfo;
}
