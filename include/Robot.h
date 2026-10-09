#ifndef ROBOT_H_INCLUDED
#define ROBOT_H_INCLUDED
#include <iostream>
#include <string>
#include <cstdlib>// Include this for using rand
#include <ctime>
#include <vector>
#include <utility>
#include <cstring>  // Include this for using strncpy
#include <utility>
#include "RobotLinkedList.h"
#include "Queue.h"
using std::vector;
using std::pair;
using namespace std;

extern Queue<Robot*> queueDestroyedRobots;
extern Queue<Robot*> queueWaitingRobots;

// Base abstract class for robots
class Robot {
protected:
    string robot_id;
    string robot_name;
    int posX, posY;
    int lives = 3;
    RobotLinkedList* robotList; // Pointer to RobotLinkedList
public:

    virtual ~Robot() = default;

    string getRobotId() const { return robot_id; };
    void setRobotName(const string& name) { robot_name = name; };
    void setPosition(int x, int y);
    void setName(const string& robotName);
    void displayInfo() const;
    void setRobotList(RobotLinkedList* list) {
        robotList = list;
    };
    void decreaseLives()
    {
    if (lives > 0) {
        lives--;
    }
     if (lives > 0) {
        queueWaitingRobots.enqueue(this);
    } else {
        delete this; // Remove permanently if no lives left
    }
    };

     int getLives() const
    {
        return lives;
    };
    string getRobotIdLetters() {
        return robot_id.substr(0, 2);
    }

    int getRobotIdNumbers() {
        return stoi(robot_id.substr(2));
    }

      pair<int, int> getPosition() const {  // Method to get the position of the robot
        return {posX, posY};
    }

};


//------------------------------------------------------------------------------------------------------------------Derived Abstract Classes
class MovingRobot : virtual public Robot {// declare a method that must be implemented by any subclass
public:
    virtual void actionMove(char** battlefield, int N, int M) = 0;

};

class ShootingRobot : virtual public Robot {
public:

    virtual void actionFire(char** battlefield, int N, int M) = 0; // declare a method that must be implemented by any subclass
};

class SeeingRobot : virtual public Robot {
public:

    virtual void actionLook(char** battlefield, int N, int M) = 0;
};

class SteppingRobot : virtual public Robot {
public:
    virtual void actionStep(char** battlefield, int N, int M) = 0;
};

//------------------------------------------------------------------------------------------------------------------
// Derived classes for different robot types
class RoboCop : public SeeingRobot, public MovingRobot, public ShootingRobot {
private:
    int ammo = 3;  // Number of fires remaining in current turn
    int totalKills = 0;      // Total number of robots killed
    bool upgrade = false;    // Flag to check if upgraded to step on robots
public:

    void actionLook(char** battlefield, int N, int M) override;  // Inspect immediate 3x3 neighborhood
    void actionMove(char** battlefield, int N, int M) override;  //
    void actionFire(char** battlefield, int N, int M) override;
    void upgradeToTerminatorRoboCop(char** battlefield, int posx, int posY);

};

//------------------------------------------------------------------------------------------------------------------
class Terminator : public SeeingRobot, public MovingRobot {
private:
    vector<pair<int, int>> enemyPositions; // To store positions of detected enemies (vector enemyPositions will be a pair of integers.)
    int totalKills = 0;  // Counter for the number of kills
    bool upgrade = false;
public:
    void actionLook(char** battlefield, int N, int M) override;  // Inspect immediate 3x3 neighborhood
    void actionMove(char** battlefield, int N, int M) override;  // Move based on inspection
    void upgradeToTerminatorRoboCop(char** battlefield, int posX, int posY);

};

//------------------------------------------------------------------------------------------------------------------
class TerminatorRoboCop : public SeeingRobot, public ShootingRobot, public SteppingRobot {
private:
    vector<pair<int, int>> enemyPositions; // To store positions of detected enemies
    int totalKills = 0;
    bool upgrade = false;
public:
    void actionLook(char** battlefield, int N, int M) override;
    void actionFire(char** battlefield, int N, int M) override;
    void actionStep(char** battlefield, int N, int M) override;
    void upgradeToUltimateRobot(char** battlefield, int posX, int posY);
};

//------------------------------------------------------------------------------------------------------------------
class BlueThunder : public ShootingRobot {
private:
    int totalKills = 0;
    bool upgrade = false;
    vector<pair<int, int>> firingSequence;
    size_t fireSequenceIndex = 0;
public:
    void actionFire(char** battlefield, int N, int M) override;
    void upgradeToMadBot(char** battlefield, int posX, int posY);

};

//------------------------------------------------------------------------------------------------------------------
class Madbot : public ShootingRobot {
private:
    int totalKills = 0;
    bool upgrade = false;
public:
    void actionFire(char** battlefield, int N, int M) override;
    void upgradeToRobotTank(char** battlefield, int posX, int posY);

};

//------------------------------------------------------------------------------------------------------------------
class RoboTank : public ShootingRobot {
private:
    int totalKills = 0;
    bool upgrade = false;
public:
    void actionFire(char** battlefield, int posX, int posY) override;
    void upgradeToUltimateRobot(char** battlefield, int posX, int posY);

};
//------------------------------------------------------------------------------------------------------------------
class UltimateRobot : public SeeingRobot, public MovingRobot, public ShootingRobot, public SteppingRobot {
private:
    vector<pair<int, int>> enemyPositions; // To store positions of detected enemies
    int totalKills = 0;
public:
    void actionLook(char** battlefield, int N, int M) override;
    void actionMove(char** battlefield, int N, int M) override;
    void actionFire(char** battlefield, int N, int M) override;
    void actionStep(char** battlefield, int N, int M) override;

};

//------------------------------------------------------------------------------------------------------------------------


void Robot::setPosition(int x, int y) {
    posX = x;
    posY = y;
}

void Robot::setName(const string& robotName) {
    robot_id = robotName;
}

void Robot::displayInfo() const {
    cout << "Robot: " << robot_id << ", Position: (" << posX << ", " << posY << "), Lives: " << getLives() << endl;
}




// Implement the action methods for each robot type
//------------------------------------------------------------------------------------------------------------------
void RoboCop::upgradeToTerminatorRoboCop(char** battlefield, int posX, int posY) {
    // Get the ID of the current RoboCop
    // Clear the battlefield cell of the current RoboCop
    strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
    battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination

    // Inform about the upgrade and update battlefield with TerminatorRoboCop
    upgrade = true;  // Set upgrade flag
    cout << "RoboCop has upgraded to TerminatorRoboCop!" << endl;

    // Create a new TerminatorRoboCop object
    Robot* newRobot = new TerminatorRoboCop();
    // Insert the new TerminatorRoboCop into the list
    //robotList->insert("TR02", "TerminatorRoboCop", 02, posX, posY, newRobot);
    const string& current_robot_id = "RC01"; // Example current robot ID to upgrade
    const string& new_robotid_letters = "TR"; // New robot letters ID
    int new_robotid_numbers = 1; // New robot numbers ID
    robot_id=robotList->nehtid(current_robot_id,new_robotid_letters,new_robotid_numbers,posX,posY,newRobot);;
    strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), robot_id.c_str(), CELL_WIDTH);
    battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
    robotList->upgrade(current_robot_id,new_robotid_letters,new_robotid_numbers,posX,posY,newRobot);

}

void RoboCop::actionLook(char** battlefield, int N, int M) {
    cout << "RoboCop sees its own location at (" << posX << ", " << posY << "): " << endl;

}

void RoboCop::actionMove(char** battlefield, int N, int M) {
    // Attempt to move RoboCop to a random new position
    int newX, newY;

    // Attempt to move to a new position up to 10 times to find a free spot
    for (int attempt = 0; attempt < 10; ++attempt) {
        newX = posX + rand() % 3 - 1;  // Move randomly within -1, 0, or 1 in x direction
        newY = posY + rand() % 3 - 1;  // Move randomly within -1, 0, or 1 in y direction

        // Check if the new position is within bounds and not occupied by another robot
        if (newX >= 0 && newX < M && newY >= 0 && newY < N && battlefield[newY][newX * (CELL_WIDTH + 1)] == ' '&& (newX != posX || newX != posY)) {
            // Erase ID from previous position
            strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
            battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination

            // Update the position of RoboCop
            posX = newX;
            posY = newY;

            // Place ID at the new position
            strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), robot_id.c_str(), CELL_WIDTH);
            battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
            cout<<"Robocop moved to location ("<<posX<<","<<posY<<")"<<endl;
            return;  // Successfully moved to the new position
        }
    }
}

void RoboCop::actionFire(char** battlefield, int N, int M) {
    // Fire at three random positions within the battlefield

    for (int fireCount = 0; fireCount < 3; ++fireCount) {
        int fireX = rand() % M;
        int fireY = rand() % N;

        // Check if the firing position is within the maximum city block distance
        if (abs(fireX - posX) + abs(fireY - posY) <= 10 && abs(fireX - posX) + abs(fireY - posY) >=1 )  {
            // Check if there's a robot at the firing position
            if (battlefield[fireY][fireX * (CELL_WIDTH + 1)] != ' ')
                {
                // A robot is hit
                string destroyedrobotid = battlefield[fireY] + fireX * (CELL_WIDTH + 1);
                const string& destroyedRobotName = destroyedrobotid;
                cout << robot_id << " has open fire towards " << destroyedRobotName << " at (" << fireX << ", " << fireY << ")." << endl;
                Robot* destroyedRobot =robotList->search(destroyedRobotName);
                if (destroyedRobot) {
                    destroyedRobot->decreaseLives();
                    cout << "Remaining lives of " << destroyedRobotName << ": " << destroyedRobot->getLives() << endl;
                if (destroyedRobot->getLives() > 0) {
                         robotList->remove(destroyedRobotName); // Remove from linked list
                         queueWaitingRobots.enqueue(destroyedRobot); // Queue for reentry
                    } else {
                        queueDestroyedRobots.enqueue(destroyedRobot);
                        robotList->remove(destroyedRobotName);
                        cout<< destroyedRobotName << " has no lives left and is permanently destroyed."<< endl;
                        //robotList->remove(destroyedRobotName);
                        //strncpy(battlefield[fireY] + fireX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
                        //battlefield[fireY][fireX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
                        //return;
                            }
                }
                totalKills++;  // Increase the kill count
                strncpy(battlefield[fireY] + fireX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
                battlefield[fireY][fireX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination

                // Check if RoboCop has killed 3 robots to upgrade
                if (totalKills == 3)
                    {
                    string rc=battlefield[posY] + posX * (CELL_WIDTH + 1);
                    const string& robocop = rc;
                    robotList->remove(robocop);
                    upgradeToTerminatorRoboCop(battlefield, posX, posY);
                     // Exit the method after upgrade
                    }

                }
            }


        }

}

//------------------------------------------------------------------------------------------------------------------
void Terminator::upgradeToTerminatorRoboCop(char** battlefield, int posX, int posY) {

    strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
    battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination

    upgrade = true;  // Set upgrade flag
    cout << "Terminator has upgraded to TerminatorRoboCop!" << endl;

    // Create a new TerminatorRoboCop object
    Robot* newRobot = new TerminatorRoboCop();
    const string& current_robot_id = "TE01"; // Example current robot ID to upgrade
    const string& new_robotid_letters = "TR"; // New robot letters ID
    int new_robotid_numbers = 1; // New robot numbers ID
    robot_id=robotList->nehtid(current_robot_id,new_robotid_letters,new_robotid_numbers,posX,posY,newRobot);
    strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), robot_id.c_str(), CELL_WIDTH);
    battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
}

void Terminator::actionLook(char** battlefield, int N, int M) {
     // Clear the previous look results
    enemyPositions.clear();

    // Look at the immediate neighborhood (3x3 window)
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            int x = posX + dx;
            int y = posY + dy;

            // Check if the position is within bounds
            if (x >= 0 && x < M && y >= 0 && y < N &&(x != posX || y != posY)) {
                char* cell = battlefield[y] + x * (CELL_WIDTH + 1);

                // Check if the cell contains an enemy robot
                if (cell[0] != ' ' && cell[0] != robot_id[0]) {
                    enemyPositions.push_back({x, y});
                    cout<<"Terminator found an Enemy at (" << x << ", " << y<< ")."<<endl;
                }
            }
        }
    }
}

void Terminator::actionMove(char** battlefield, int N, int M) {

    // If enemies are found, move to the first enemy position
    if (!enemyPositions.empty()) {
        int bestX = enemyPositions[0].first;
        int bestY = enemyPositions[0].second;
        // "Kill" the enemy
        totalKills++;


       // Print message about destroyed robot
        string destroyedrobotid=battlefield[bestY] + bestX * (CELL_WIDTH + 1);
        const string& destroyedRobotName = destroyedrobotid;
        cout << robot_id << " has stomp on " << destroyedrobotid << " at (" << bestX << ", " << bestY << ")." << endl;
        // Remove the destroyed robot from the linked list
         Robot* destroyedRobot = robotList->search(destroyedRobotName);
                if (destroyedRobot) {
                    destroyedRobot->decreaseLives();
                    cout << "Remaining lives of " << destroyedRobotName << ": " << destroyedRobot->getLives() << endl;
                if (destroyedRobot->getLives() > 0) {
                         robotList->remove(destroyedRobotName); // Remove from linked list
                         queueWaitingRobots.enqueue(destroyedRobot); // Queue for reentry
                    } else {
                        queueDestroyedRobots.enqueue(destroyedRobot);
                        robotList->remove(destroyedRobotName);
                        cout<< destroyedRobotName << " has no lives left and is permanently destroyed."<< endl;
                        //robotList->remove(destroyedRobotName);
                        //strncpy(battlefield[bestY] + bestX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
                        //battlefield[bestY][bestX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
                        //return;

                            }
                }

        // Clear the battlefield position of the destroyed robot
        strncpy(battlefield[bestY] + bestX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
        battlefield[bestY][bestX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination

        // Erase ID from previous position
        strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
        battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination

        // Move to the new position
        posX = bestX;
        posY = bestY;

        // Mark the new position with the robot ID
        strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), robot_id.c_str(), CELL_WIDTH);
        battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
         if (totalKills == 3) {
            // Upgrade to TerminatorRoboCop
            string te=battlefield[posY] + posX * (CELL_WIDTH + 1);
            const string& terminator = te;
            robotList->remove(terminator);
            upgradeToTerminatorRoboCop(battlefield, posX, posY);

        }
        return;
    } else {
        // No enemies found, attempt to move to a new position
        int newX, newY;

        // Attempt to move to a new position up to 3 times to find a free spot
        for (int attempt = 0; attempt < 3; ++attempt) {
            newX = posX + rand() % 3 - 1;  // Move randomly within -1, 0, or 1 in x direction
            newY = posY + rand() % 3 - 1;  // Move randomly within -1, 0, or 1 in y direction


            if (newX >= 0 && newX < M && newY >= 0 && newY < N && battlefield[newY][newX * (CELL_WIDTH + 1)] == ' '&&(newX != posX || newY != posY)) {
                // Erase ID from previous position
                strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
                battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination

                // Update the position of the Terminator
                posX = newX;
                posY = newY;

                // Place ID at the new position
                strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), robot_id.c_str(), CELL_WIDTH);
                battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
                cout<<"Terminator moved to location ("<<posX<<","<<posY<<")"<<endl;
                return;  // Successfully moved to the new position
            }
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
void TerminatorRoboCop::upgradeToUltimateRobot(char** battlefield, int posX, int posY) {


    // Inform about the upgrade and update battlefield with TerminatorRoboCop
    upgrade = true;  // Set upgrade flag
    cout << "TerminatorRoboCop has upgraded to Ultimate Robot!" << endl;

    // Create a new TerminatorRoboCop object
    Robot* newRobot = new UltimateRobot();
    const string& current_robot_id = "TR01"; // Example current robot ID to upgrade
    const string& new_robotid_letters = "UR"; // New robot letters ID
    int new_robotid_numbers = 1; // New robot numbers ID
    robot_id=robotList->nehtid(current_robot_id,new_robotid_letters,new_robotid_numbers,posX,posY,newRobot);;
    strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), robot_id.c_str(), CELL_WIDTH);
    battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
    robotList->upgrade(current_robot_id,new_robotid_letters,new_robotid_numbers,posX,posY,newRobot);

}
void TerminatorRoboCop::actionLook(char** battlefield, int N, int M) {
     // Clear the previous look results
    enemyPositions.clear();
    // Look at the immediate neighborhood (3x3 window)
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            int x = posX + dx;
            int y = posY + dy;

            // Check if the position is within bounds
            if (x >= 0 && x < M && y >= 0 && y < N &&(x != posX || y != posY)) {
                char* cell = battlefield[y] + x * (CELL_WIDTH + 1);

                // Check if the cell contains an enemy robot
                if (cell[0] != ' ' && cell[0] != robot_id[0]) {
                    enemyPositions.push_back({x, y});
                    cout<<"TerminatorRoboCop found an Enemy at (" << x << ", " << y<< ")."<<endl;
                }
            }
        }
    }

}

void TerminatorRoboCop::actionFire(char** battlefield, int N, int M) {
    for (int fireCount = 0; fireCount < 3; ++fireCount) {
        int fireX, fireY;
        bool validTarget = false;

        while (!validTarget) {
            int range = rand() % 10 + 1; // Random range between 1 and 10
            int dx = rand() % (2 * range + 1) - range; // Random x offset between -range and +range
            int dy = rand() % (2 * range + 1) - range; // Random y offset between -range and +range

            if (abs(dx) + abs(dy) >= 1 && abs(dx) + abs(dy) <= 10) {
                fireX = posX + dx;
                fireY = posY + dy;

                if (fireX >= 0 && fireX < M && fireY >= 0 && fireY < N) {
                    bool isEnemyPosition = std::find(enemyPositions.begin(), enemyPositions.end(), std::make_pair(fireX, fireY)) != enemyPositions.end();

                    if (!isEnemyPosition) {
                        validTarget = true;
                    }
                }
            }
        }

        if (battlefield[fireY][fireX * (CELL_WIDTH + 1)] != ' ') {
            totalKills++;
            string destroyedrobotid = battlefield[fireY] + fireX * (CELL_WIDTH + 1);
            const string& destroyedRobotName = destroyedrobotid;
            Robot* destroyedRobot = robotList->search(destroyedRobotName);
            cout << robot_id << " has open fire towards " << destroyedRobotName << " at (" << fireX << ", " << fireY << ")." << endl;
            if (destroyedRobot) {
                destroyedRobot->decreaseLives();
                cout << "Remaining lives of " << destroyedRobotName << ": " << destroyedRobot->getLives() << endl;
                if (destroyedRobot->getLives() > 0) {
                    robotList->remove(destroyedRobotName); // Remove from linked list
                    queueWaitingRobots.enqueue(destroyedRobot); // Queue for reentry
                } else {
                    queueDestroyedRobots.enqueue(destroyedRobot);
                    robotList->remove(destroyedRobotName);
                    cout << destroyedRobotName << " has no lives left and is permanently destroyed." << endl;
                }
            }

            if (totalKills == 3) {
                string tre = battlefield[posY] + posX * (CELL_WIDTH + 1);
                const string& terminatorRoboCop = tre;
                robotList->remove(terminatorRoboCop);
                upgradeToUltimateRobot(battlefield, posX, posY);
            }
            strncpy(battlefield[fireY] + fireX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
            battlefield[fireY][fireX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
        } else {
            cout << robot_id << " fired at an empty position at (" << fireX << ", " << fireY << ")." << endl;
        }
    }
}


void TerminatorRoboCop::actionStep(char** battlefield, int N, int M) {

    // If enemies are found, move to the first enemy position
    if (!enemyPositions.empty()) {
        int bestX = enemyPositions[0].first;
        int bestY = enemyPositions[0].second;

        // "Kill" the enemy
        totalKills++;

        // Print message about destroyed robot
        string destroyedrobotid = battlefield[bestY] + bestX * (CELL_WIDTH + 1);
        const string& destroyedRobotName = destroyedrobotid;
        cout << robot_id << " has stomp on " << destroyedRobotName << " at (" << bestX << ", " << bestY << ")." << endl;
        Robot* destroyedRobot = robotList->search(destroyedRobotName);
                 if (destroyedRobot) {
                    destroyedRobot->decreaseLives();
                    cout << "Remaining lives of " << destroyedRobotName << ": " << destroyedRobot->getLives() << endl;
                if (destroyedRobot->getLives() > 0) {
                         robotList->remove(destroyedRobotName); // Remove from linked list
                         queueWaitingRobots.enqueue(destroyedRobot); // Queue for reentry
                    } else {
                         queueDestroyedRobots.enqueue(destroyedRobot);
                         robotList->remove(destroyedRobotName);
                         cout<< destroyedRobotName << " has no lives left and is permanently destroyed."<< endl;
                         //robotList->remove(destroyedRobotName);
                         //strncpy(battlefield[bestY] + bestX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
                         //battlefield[bestY][bestX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination

                        //return;
                            }
                }

        // Clear the battlefield position of the destroyed robot
        strncpy(battlefield[bestY] + bestX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
        battlefield[bestY][bestX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination

        // Erase ID from previous position
        strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
        battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination

        // Move to the new position
        posX = bestX;
        posY = bestY;

        // Mark the new position with the robot ID
        strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), robot_id.c_str(), CELL_WIDTH);
        battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
         if (totalKills == 3)
                {
                    upgradeToUltimateRobot(battlefield, posX, posY);
                }
        return;
    } else {
        // No enemies found, attempt to move to a new position
        int newX, newY;

        // Attempt to move to a new position up to 3 times to find a free spot
        for (int attempt = 0; attempt < 3; ++attempt) {
            newX = posX + rand() % 3 - 1;  // Move randomly within -1, 0, or 1 in x direction
            newY = posY + rand() % 3 - 1;  // Move randomly within -1, 0, or 1 in y direction


            if (newX >= 0 && newX < M && newY >= 0 && newY < N && battlefield[newY][newX * (CELL_WIDTH + 1)] == ' '&&(newX != posX || newY != posY)) {
                // Erase ID from previous position
                strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
                battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination

                // Update the position of the Terminator
                posX = newX;
                posY = newY;

                // Place ID at the new position
                strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), robot_id.c_str(), CELL_WIDTH);
                battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
                cout<<"TerminatoRoboCop moved to location ("<<posX<<","<<posY<<")"<<endl;
                return;  // Successfully moved to the new position
            }
        }
    }

}
//------------------------------------------------------------------------------------------------------------------
void BlueThunder::upgradeToMadBot(char** battlefield, int posX, int posY){

    strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
    battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
    // Inform about the upgrade and update battlefield with TerminatorRoboCop
    upgrade = true;  // Set upgrade flag
    cout << "BlueThunder has upgraded to MadBot!" << endl;
    Robot* newRobot = new Madbot();
    const string& current_robot_id = "BT01"; // Example current robot ID to upgrade
    const string& new_robotid_letters = "MB"; // New robot letters ID
    int new_robotid_numbers = 1; // New robot numbers ID
    robot_id=robotList->nehtid(current_robot_id,new_robotid_letters,new_robotid_numbers,posX,posY,newRobot);;
    strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), robot_id.c_str(), CELL_WIDTH);
    battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
    robotList->upgrade(current_robot_id,new_robotid_letters,new_robotid_numbers,posX,posY,newRobot);

}

void BlueThunder::actionFire(char** battlefield, int N, int M) {

    firingSequence = { {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1} };
    int targetX = posX + firingSequence[fireSequenceIndex].first;
    int targetY = posY + firingSequence[fireSequenceIndex].second;

    // Ensure the target position is within bounds
    if (targetX >= 0 && targetX < M && targetY >= 0 && targetY < N)
    {
        //Check if there's a robot at the target position
        if (battlefield[targetY][targetX * (CELL_WIDTH + 1)] != ' ') {
            // A robot is hit
            string destroyedrobotid = battlefield[targetY] + targetX * (CELL_WIDTH + 1);
            const string& destroyedRobotName = destroyedrobotid;
            cout << robot_id << " has open fire towards " << destroyedRobotName << " at (" << targetX << ", " << targetY << ")." << endl;
            Robot* destroyedRobot = robotList->search(destroyedRobotName);
                if (destroyedRobot) {
                    destroyedRobot->decreaseLives();
                    cout << "Remaining lives of " << destroyedRobotName << ": " << destroyedRobot->getLives() << endl;
                if (destroyedRobot->getLives() > 0) {
                         robotList->remove(destroyedRobotName); // Remove from linked list
                         queueWaitingRobots.enqueue(destroyedRobot); // Queue for reentry
                    } else {
                         queueDestroyedRobots.enqueue(destroyedRobot);
                         robotList->remove(destroyedRobotName);
                         cout<< destroyedRobotName << " has no lives left and is permanently destroyed."<< endl;
                         //robotList->remove(destroyedRobotName);
                         //strncpy(battlefield[targetY] + targetX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
                         //battlefield[targetY][targetX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
                        //return;
                            }
                }
            //robotList->remove(destroyedRobotName);
            //robotList.remove(destroyedRobotName);
            totalKills++;  // Increase the shot count

            // Clear the battlefield position of the destroyed robot
            strncpy(battlefield[targetY] + targetX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
            battlefield[targetY][targetX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination

            // Check if BlueThunder has shot 3 robots to upgrade
            if (totalKills == 3) {
                    string mb=battlefield[posY] + posX * (CELL_WIDTH + 1);
                    const string& madbot = mb;
                    robotList->remove(madbot);
                    upgradeToMadBot(battlefield, posX, posY);
            }
        }
        else {
            cout << robot_id << " fired at an empty position at (" << targetX << ", " << targetY << ")." << endl;
    }
    fireSequenceIndex = (fireSequenceIndex + 1) % firingSequence.size(); // Move to the next position in the firing sequence (circular)
}
}
//------------------------------------------------------------------------------------------------------------------
void Madbot::upgradeToRobotTank(char** battlefield, int posX, int posY){

    strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
    battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
    // Inform about the upgrade and update battlefield with TerminatorRoboCop
    upgrade = true;  // Set upgrade flag
    cout << "Madbot has upgraded to RoboTank!" << endl;
    Robot* newRobot = new RoboTank();
    const string& current_robot_id = "MB01"; // Example current robot ID to upgrade
    const string& new_robotid_letters = "RT"; // New robot letters ID
    int new_robotid_numbers = 1; // New robot numbers ID
    robot_id=robotList->nehtid(current_robot_id,new_robotid_letters,new_robotid_numbers,posX,posY,newRobot);;
    strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), robot_id.c_str(), CELL_WIDTH);
    battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
    robotList->upgrade(current_robot_id,new_robotid_letters,new_robotid_numbers,posX,posY,newRobot);

}
void Madbot::actionFire(char** battlefield, int N, int M) {
    int fireX, fireY;
    bool validTarget = false;

    // Loop until a valid target is found within bounds
    while (!validTarget) {
        fireX = posX + rand() % 3 - 1;  // Fire randomly within -1, 0, or 1 in x direction
        fireY = posY + rand() % 3 - 1;  // Fire randomly within -1, 0, or 1 in y direction

        if (fireX >= 0 && fireX < M && fireY >= 0 && fireY < N && (fireX != posX || fireY != posY)) {
            validTarget = true;

            // Check if there's a robot at the target position
            if (battlefield[fireY][fireX * (CELL_WIDTH + 1)] != ' ') {
                // A robot is hit
                string destroyedrobotid = battlefield[fireY] + fireX * (CELL_WIDTH + 1);
                const string& destroyedRobotName = destroyedrobotid;
                cout << robot_id << " has open fire towards " << destroyedRobotName << " at (" << fireX << ", " << fireY << ")." << endl;

                Robot* destroyedRobot = robotList->search(destroyedRobotName);
                 if (destroyedRobot) {
                    destroyedRobot->decreaseLives();
                    cout << "Remaining lives of " << destroyedRobotName << ": " << destroyedRobot->getLives() << endl;
                      if (destroyedRobot->getLives() > 0) {
                         robotList->remove(destroyedRobotName); // Remove from linked list
                         queueWaitingRobots.enqueue(destroyedRobot); // Queue for reentry
                    } else {
                         queueDestroyedRobots.enqueue(destroyedRobot);
                         robotList->remove(destroyedRobotName);
                         cout<< destroyedRobotName << " has no lives left and is permanently destroyed."<< endl;
                         //robotList->remove(destroyedRobotName);
                         strncpy(battlefield[fireY] + fireX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
                         battlefield[fireY][fireX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
                        //return;
                            }
                            }
                totalKills++;  // Increase the shot count

                // Clear the battlefield position of the destroyed robot
                strncpy(battlefield[fireY] + fireX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
                battlefield[fireY][fireX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination

                // Upgrade to RobotTank if totalKills reaches 1
                if (totalKills == 3) {
                    string rt = battlefield[posY] + posX * (CELL_WIDTH + 1);
                    const string& robottank = rt;
                    robotList->remove(robottank);
                    upgradeToRobotTank(battlefield, posX, posY);
                }

                }
                 else {
            cout << robot_id << " fired at an empty position at (" << fireX << ", " << fireY << ")." << endl;
                     }



            }
        }
    }


//------------------------------------------------------------------------------------------------------------------
void RoboTank::upgradeToUltimateRobot(char** battlefield, int posX, int posY){

    strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
    battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
    // Inform about the upgrade and update battlefield with TerminatorRoboCop
    upgrade = true;  // Set upgrade flag
    cout << "RoboTank has upgraded to UltimateRobot!" << endl;
    Robot* newRobot = new RoboTank();
    const string& current_robot_id = "RT01"; // Example current robot ID to upgrade
    const string& new_robotid_letters = "UR"; // New robot letters ID
    int new_robotid_numbers = 1; // New robot numbers ID
    robot_id=robotList->nehtid(current_robot_id,new_robotid_letters,new_robotid_numbers,posX,posY,newRobot);;
    strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), robot_id.c_str(), CELL_WIDTH);
    battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
    robotList->upgrade(current_robot_id,new_robotid_letters,new_robotid_numbers,posX,posY,newRobot);

}
void RoboTank::actionFire(char** battlefield, int N, int M) {

    // Fire at one random position within the battlefield
    int fireX = rand() % M;
    int fireY = rand() % N;

    // Ensure that the target position is not the robot's own position
    if (fireX != posX || fireY != posY) {
        // Check if there's a robot at the target position
        if (battlefield[fireY][fireX * (CELL_WIDTH + 1)] != ' ') {
            // A robot is hit
            string destroyedrobotid = battlefield[fireY] + fireX * (CELL_WIDTH + 1);
            const string& destroyedRobotName = destroyedrobotid;
            cout << robot_id << " has open fire towards " << destroyedRobotName << " at (" << fireX << ", " << fireY << ")." << endl;
            totalKills++;  // Increase the kill count
            Robot* destroyedRobot = robotList->search(destroyedRobotName);
                 if (destroyedRobot) {
                    destroyedRobot->decreaseLives();
                    cout << "Remaining lives of " << destroyedRobotName << ": " << destroyedRobot->getLives() << endl;
                if (destroyedRobot->getLives() > 0) {
                         robotList->remove(destroyedRobotName); // Remove from linked list
                         queueWaitingRobots.enqueue(destroyedRobot); // Queue for reentry
                    } else {
                         queueDestroyedRobots.enqueue(destroyedRobot);
                         robotList->remove(destroyedRobotName);
                         cout<< destroyedRobotName << " has no lives left and is permanently destroyed."<< endl;
                         //robotList->remove(destroyedRobotName);
                         strncpy(battlefield[fireY] + fireX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
                         battlefield[fireY][fireX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
                        //return;
                            }
                }
            //robotList->remove(destroyedRobotName);
            //robotList.remove(destroyedRobotName)
            // Clear the battlefield position of the destroyed robot
            strncpy(battlefield[fireY] + fireX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
            battlefield[fireY][fireX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination

            if (totalKills == 3) {
                    string ur = battlefield[posY] + posX * (CELL_WIDTH + 1);
                    const string& ulti = ur;
                    robotList->remove(ulti);
                    upgradeToUltimateRobot(battlefield, posX, posY);
                }
        }
        else {
            cout << robot_id << " fired at an empty position at (" << fireX << ", " << fireY << ")." << endl;
                     }

    }
}
//------------------------------------------------------------------------------------------------------------------
void UltimateRobot::actionLook(char** battlefield, int N, int M) {

    // Clear the previous look results
    enemyPositions.clear();

    // Look at the immediate neighborhood (3x3 window)
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            int x = posX + dx;
            int y = posY + dy;

            // Check if the position is within bounds
            if (x >= 0 && x < M && y >= 0 && y < N &&(x != posX || y != posY)) {
                char* cell = battlefield[y] + x * (CELL_WIDTH + 1);

                // Check if the cell contains an enemy robot
                if (cell[0] != ' ' && cell[0] != robot_id[0]) {
                    enemyPositions.push_back({x, y});
                    cout<<"UltimateRobot found an Enemy at (" << x << ", " << y<< ")."<<endl;
                }
            }
        }
    }

}

void UltimateRobot::actionMove(char** battlefield, int N, int M) {

}

void UltimateRobot::actionFire(char** battlefield, int N, int M) {

 // Fire at three random positions within the battlefield
    for (int fireCount = 0; fireCount < 3; ++fireCount) {
        int fireX = rand() % M;
        int fireY = rand() % N;
        bool validTarget = false;

        while (!validTarget) {
                if (fireX >= 0 && fireX < M && fireY >= 0 && fireY < N) {
                    bool isEnemyPosition = std::find(enemyPositions.begin(), enemyPositions.end(), std::make_pair(fireX, fireY)) != enemyPositions.end();

                    if (!isEnemyPosition) {
                        validTarget = true;
                    }
                }

        }
        // Check if the firing position is within the maximum city block distance
            if (battlefield[fireY][fireX * (CELL_WIDTH + 1)] != ' ') {
                // A robot is hit

                string destroyedrobotid = battlefield[fireY] + fireX * (CELL_WIDTH + 1);
                const string& destroyedRobotName = destroyedrobotid;
                cout << robot_id << " has open fire towards " << destroyedRobotName << " at (" << fireX << ", " << fireY << ")." << endl;
                Robot* destroyedRobot = robotList->search(destroyedRobotName);
                if (destroyedRobot) {
                    destroyedRobot->decreaseLives();
                    cout << "Remaining lives of " << destroyedRobotName << ": " << destroyedRobot->getLives() << endl;
                if (destroyedRobot->getLives() > 0) {
                         robotList->remove(destroyedRobotName); // Remove from linked list
                         queueWaitingRobots.enqueue(destroyedRobot); // Queue for reentry
                    } else {
                         queueDestroyedRobots.enqueue(destroyedRobot);
                         robotList->remove(destroyedRobotName);
                         cout<< destroyedRobotName << " has no lives left and is permanently destroyed."<< endl;
                         //robotList->remove(destroyedRobotName);
                         strncpy(battlefield[fireY] + fireX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
                         battlefield[fireY][fireX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination

                        //return;
                            }
                }
                 //robotList->remove(destroyedRobotName);
                //robotList.remove(destroyedRobotName);
                totalKills++;  // Increase the kill count
                strncpy(battlefield[fireY] + fireX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
                battlefield[fireY][fireX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination


            }
            else {
            cout << robot_id << " fired at an empty position at (" << fireX << ", " << fireY << ")." << endl;
                     }
        }
    }


void UltimateRobot::actionStep(char** battlefield, int N, int M) {

    // If enemies are found, move to the first enemy position
    if (!enemyPositions.empty()) {
        int bestX = enemyPositions[0].first;
        int bestY = enemyPositions[0].second;

        // "Kill" the enemy
        totalKills++;

        // Print message about destroyed robot
        string  destroyedrobotid = battlefield[bestY] + bestX * (CELL_WIDTH + 1);
        const string& destroyedRobotName = destroyedrobotid;
        cout << robot_id << " has stomp on " << destroyedRobotName << " at (" << bestX << ", " << bestY << ")." << endl;
        Robot* destroyedRobot = robotList->search(destroyedRobotName);
                 if (destroyedRobot) {
                    destroyedRobot->decreaseLives();
                    cout << "Remaining lives of " << destroyedRobotName << ": " << destroyedRobot->getLives() << endl;
                if (destroyedRobot->getLives() > 0) {
                         robotList->remove(destroyedRobotName); // Remove from linked list
                         queueWaitingRobots.enqueue(destroyedRobot); // Queue for reentry
                    } else {
                         queueDestroyedRobots.enqueue(destroyedRobot);
                         robotList->remove(destroyedRobotName);
                         cout<< destroyedRobotName << " has no lives left and is permanently destroyed."<< endl;
                         strncpy(battlefield[bestY] + bestX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
                         battlefield[bestY][bestX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination

                        //return;
                            }
                }
         //robotList->remove(destroyedRobotName);
        //robotList.remove(destroyedRobotName);
        // Clear the battlefield position of the destroyed robot
        strncpy(battlefield[bestY] + bestX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
        battlefield[bestY][bestX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination

        // Erase ID from previous position
        strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
        battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination

        // Move to the new position
        posX = bestX;
        posY = bestY;

        // Mark the new position with the robot ID
        strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), robot_id.c_str(), CELL_WIDTH);
        battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
        return;
    } else {
        // No enemies found, attempt to move to a new position
        int newX, newY;

        // Attempt to move to a new position up to 3 times to find a free spot
        for (int attempt = 0; attempt < 3; ++attempt) {
            newX = posX + rand() % 3 - 1;  // Move randomly within -1, 0, or 1 in x direction
            newY = posY + rand() % 3 - 1;  // Move randomly within -1, 0, or 1 in y direction


            if (newX >= 0 && newX < M && newY >= 0 && newY < N && battlefield[newY][newX * (CELL_WIDTH + 1)] == ' '&&(newX != posX || newY != posY)) {
                // Erase ID from previous position
                strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), " ", CELL_WIDTH);
                battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination

                // Update the position of the Terminator
                posX = newX;
                posY = newY;

                // Place ID at the new position
                strncpy(battlefield[posY] + posX * (CELL_WIDTH + 1), robot_id.c_str(), CELL_WIDTH);
                battlefield[posY][posX * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
                cout<<"UltimateRobot moved to location ("<<posX<<","<<posY<<")"<<endl;
                return;  // Successfully moved to the new position
            }
        }
    }
}

#endif // ROBOT_H_INCLUDED


