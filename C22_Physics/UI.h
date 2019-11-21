#pragma once
#include <string>
namespace Simplex {
	class UI
	{
		const int uiInfoNum = 2; //the number of information to be displayed on the GUI

	private:
		static UI* instance; //the static reference to the UI singleton
		/*
		USAGE: Private Constructor
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		UI();
		/*
		USAGE: Private Destructor
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		~UI();

		int score; //the score the user has
		bool isAlive; //determines if the player is alive or not
		int killCount; //the amount of steves the player has killed
		std::string* uiInfo; //string array containing the information to put on the GUI

	public:
		/*
		USAGE: Retrieve the instance of the UI
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		static UI* Instance();

		/*
		USAGE: Remove memory allocated for the instance
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		static void ReleaseInstance();

		/*
		USAGE: Adds points to the score
		ARGUMENTS: s -> The number of points to add
		OUTPUT: ---
		*/
		void AddToScore(int s); 

		/*
		USAGE: Adds a point to the enemy count
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void KillEnemy();

		/*
		USAGE: Adds points to the enemy death count
		ARGUMENTS: count -> the mount of enemies to kill
		OUTPUT: ---
		*/
		void KillEnemies(int count);

		/*
		USAGE: Kills the player, sets isAlive to false
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void PlayerDied();

		/*
		USAGE: Returns if the player is alive or not
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		bool IsAlive();

		/*
		USAGE: Returns information for the GUI in a formatted manner
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		std::string* GetGuiInfo();
	};
}

