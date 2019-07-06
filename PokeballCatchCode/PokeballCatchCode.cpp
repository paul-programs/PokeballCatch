// PokeballCatchCode.cpp : This file contains the 'main' function. Program execution begins and ends there.
// https://bulbapedia.bulbagarden.net/wiki/Catch_rate

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
using namespace std;

int GetCaptureRate(int health, int maxHealth, int rate, int status, int ball);
bool CaptureCheck(int capRate, int ball);
bool ShakeCheck(int capRate);

int main()
{
	srand(time(NULL));

	bool gameOver = false;

	int maxHealth = 100;
	int creatureHealth = 100;
	int creatureRate = (rand() % 255) + 1;

	int creatureStatus = 0;//0 - no status, 1 - sleep, 2 - poison, 3 - paralyze, 4 - confuse
	string statusNames[] = {"Healthy", "Asleep", "Poisoned", "Paralyzed", "Confused"};
	double statusValues[] = {1};
	
	bool newCreature = true;

	int ballType = 0;//0 - none, 1 - standard, 2 - super, 3 - ultra, 4 - perfect
	int ballCounts[] = {50, 25, 10, 1};

	int currState = 0;//0 - intro, 1 - attack choice, 2 - debuff choice, 3 - capture choice

	int mainSelection = 0;

	cout << "Welcome to the monster catcher demo! Enter -1 to exit at any time.\n\n";

	while (!gameOver) {
		//encounter wild creature for the first time
		if(newCreature){
			cout << "You ran into a wild creature!\n\n";
			newCreature = false;
		}

		if(currState == 0){
			//current creature info
			cout << "The creature currently has " << creatureHealth << " HP!\n";
			cout << "The creature currently is currently " << statusNames[creatureStatus] + "!\n\n";
			cout << "The capture rate is " << creatureRate << "!\n";
			currState = 1;
		} else if(currState == 1){
			//ask how much health to remove
			int damage = 0;

			cout << "How much damage will you do? (0-100)\n";
			cin >> damage;

			//if a valid input then apply that amount of damage
			if(damage >= 0 && damage <= 100){
				creatureHealth = creatureHealth - damage;
				currState = 2;

				if(creatureHealth == 0){
					cout << "\nYou knocked out the creature and are unable to catch it!\n";
					newCreature = true;
				}
			} else if(damage == -1){
				cout << "\nYou quit the game!\n";
				break;
			} else {
				cout << "\nIncorrect input.\n\n";
			}
		} else if(currState == 2){
			//ask what kind of debuff to use
			int status = 0;

			cout << "What status will you inflict?\n";
			cout << "0 - None\n";
			cout << "1 - Sleep\n";
			cout << "2 - Poison\n";
			cout << "3 - Paralyze\n";
			cout << "4 - Confuse\n";
			cin >> status;

			//if a valid input then apply that status
			if(status >= 0 && status <= 4){
				creatureStatus = status;
				currState = 3;
			} else if(status == -1){
				cout << "\nYou quit the game!\n";
				break;
			} else {
				cout << "\nIncorrect input.\n\n";
			}
		} else if(currState == 3){
			//ask what kind of capture device to use
			int ball = 0;

			cout << "What ball will you use?\n";
			cout << "1 - Standard - "<<ballCounts[0]<<"\n";
			cout << "2 - Super - "<<ballCounts[1]<<"\n";
			cout << "3 - Ultra - "<<ballCounts[2]<<"\n";
			cout << "4 - Perfect - "<<ballCounts[3]<<"\n";
			cin >> ball;

			//check if it is a valid ball type
			if(ball >= 1 && ball <= 4){
				ballType = ball;

				//check if there are enough of the ball selected
				if(ballCounts[ballType-1] > 0){
					ballCounts[ballType-1] = ballCounts[ballType-1] - 1;
				
					//check for catch
					int captureRate = GetCaptureRate(creatureHealth, maxHealth, creatureRate, statusValues[0], ballType);
					bool captured = CaptureCheck(captureRate, ballType);

					//if captured then move on to a new creature to catch otherwise shake checks
					if(captured){
						cout << "\nCongrats you caught the creature!\n";
						newCreature = true;
					} else {
						//loop thru and perform shake checks
						for(int i=0; i<4; i++){
							bool shakeCheck = ShakeCheck(captureRate);

							//check if shake check has failed
							if(!shakeCheck){
								cout << "\nFailed to catch the creature!\n";
								currState = 0;
								break;
							}

							//check if last shake check completed and passed
							if(i == 3){
								cout << "\nCongrats you caught the creature!\n";
								newCreature = true;
							}
						}
					}
				} else {
					cout << "\nYou do not have enough of that ball. Please select another.\n";
				}
			} else if(ball == -1){
				cout << "\nYou quit the game!\n";
				break;
			} else {
				cout << "\nIncorrect input.\n\n";
			}
		}

		//reset all values when dealing with a new creature
		if(newCreature){
			creatureHealth = maxHealth;
			creatureStatus = 0;
			ballType = 0;
			currState = 0;
			creatureRate = (rand() % 255) + 1;
		}
	}

	//message for closing the game
	cout << "\nClosing the game....";
}

int GetCaptureRate(int health, int maxHealth, int rate, int status, int ball){
	int capRate = 1;
	
	//do the formula magic here
	capRate = ((maxHealth * 3) - (health * 2)) * rate * ball;
	capRate = capRate / (maxHealth * 3);
	capRate = capRate * status;

	if (capRate == 0)
		capRate = 1;

	return capRate;
}

bool CaptureCheck(int capRate, int ball){
	bool captured = false;
	
	if(ball == 4 || capRate >= 255){
		captured = true;
	}

	return captured;
}

bool ShakeCheck(int capRate){
	bool shakeSuccess = false;

	//do checks here
	int randomCheck = rand() * 65535 + 1;
	double shakeRate = 65536 / (pow((255/capRate), 0.1875));

	if(randomCheck < shakeRate)
		shakeSuccess = true;

	return shakeSuccess;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
