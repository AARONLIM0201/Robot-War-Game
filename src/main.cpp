#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <bitset>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "Classheader.h"
#include "Robot.h"
#include "RobotLinkedList.h"
#include "Queue.h"

using namespace std;

string trim(string s); // Removing leading and trailing whitespaces from a string.

// Global queues for destroyed and waiting robots
Queue<Robot*> queueDestroyedRobots;
Queue<Robot*> queueWaitingRobots;

void reEnterBattlefield(char** battlefield, int N, int M, RobotLinkedList* robotList) {
    //queueWaitingRobots.displayQueue();
    if (queueWaitingRobots.robotcounter()>= 4) { //one robot occupies two space
        Robot* r = queueWaitingRobots.front();
        queueWaitingRobots.dequeue();
        // Find a random empty position
        int x, y;
        do {
            x = rand() % M;
            y = rand() % N;
        } while (battlefield[y][x * (CELL_WIDTH + 1)] != ' ');

        // Re-enter the battlefield
        strncpy(battlefield[y] + x * (CELL_WIDTH + 1), r->getRobotId().c_str(), CELL_WIDTH);
        battlefield[y][x * (CELL_WIDTH + 1) + CELL_WIDTH] = '\0'; // Ensure null-termination
        r->setPosition(x, y);
        cout << "Robot " << r->getRobotId() << " re-entered at (" << x << ", " << y << ")\n";

        // Reinsert into the linked list with existing properties
        robotList->insert(r->getRobotId(), r->getRobotIdLetters(), r->getRobotIdNumbers(), x, y, r);
    }

}

int countRobotsOnBattlefield(char** battlefield, int N, int M, int CELL_WIDTH) {
    int count = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            if (battlefield[i][j * (CELL_WIDTH + 1)] != ' ') {
                ++count;
            }
        }
    }
    return count;
}


int main(int argc, char* argv[]) {
    ifstream infile;
    string inputPath = (argc > 1) ? argv[1] : "data/input/fileinput1.txt";
    string outputPath = (argc > 2) ? argv[2] : "data/output/fileOutput1.txt";
    ofstream outfile(outputPath);
    queueWaitingRobots.clear();
    queueDestroyedRobots.clear();
    infile.open(inputPath);

    if (infile.fail()) {
        cout << "Error: input file not found: " << inputPath << endl;
        return EXIT_FAILURE;
    }


    // Check if the output file is opened successfully
    if (!outfile.is_open()) {
        cout << "Error: Unable to open output file: " << outputPath << endl;
        return EXIT_FAILURE;
    }

    // Clear the queue at the beginning
    queueWaitingRobots.clear();
    srand(static_cast<unsigned>(time(0))); // Initialize random seed
    DisplayFunctions df; // Object for Display Functions class
    int M, N, turns, robots; // Variables to store the extracted numbers
    int robotCount,turn;
    const int CELL_WIDTH = 10;
    const int CELL_HEIGHT = 1;

    // Read the first line separately to get M and N
    string line, robot_id, robotid_letters;
    int robotid_numbers;
    if (getline(infile, line)) {
        istringstream sstream(line);
        string label;
        sstream >> label >> label >> label; // Skip the "M by N:" part
        sstream >> M >> N; // Extract the numbers (M = columns, N = rows)
    }
    char** battlefield = new char*[N];
    for (int i = 0; i < N; ++i) {
        battlefield[i] = new char[M * (CELL_WIDTH + 1)];
    }

    df.initializeBattlefield(battlefield, N, M, CELL_WIDTH);

    cout << "M: " << M << ", N: " << N << endl;
    // Output the extracted M and N for debugging

    // Create a linked list to store robots
    RobotLinkedList robotList;

    while (getline(infile, line)) { // Read each subsequent line
        istringstream sstream(line);
        string instruction;
        sstream >> instruction; // Extract the first word (instruction)
        instruction = trim(instruction); // Trim the instruction

        if (instruction == "Turns:") {
            sstream >> turns; // Extract the number after "Turns:"
            cout << "Turns: " << turns << endl;
            // Additional logic for Turns
        }
        else if (instruction == "Robots:") {
            sstream >> robots; // Extract the number after "Turns:"
            cout << "Robots: " << robots << endl;
        }
        else if (instruction == "RoboCop" || instruction == "Terminator" || instruction == "TerminatorRoboCop" || instruction == "BlueThunder" || instruction == "Madbot" || instruction == "RoboTank" || instruction == "UltimateRobot") {
            string coordinate_X, coordinate_Y;
            int x, y;
            sstream >> robot_id >> coordinate_X >> coordinate_Y;

            if (coordinate_X == "random" || coordinate_Y == "random") {
                x = rand() % M; // Random column
                y = rand() % N; // Random row
            } else {
                x = stoi(coordinate_X); // Column
                y = stoi(coordinate_Y); // Row
            }
            robotid_letters = robot_id.substr(0, 2);
            robotid_numbers = stoi(robot_id.substr(2));
            Robot* robot = nullptr;
            if (instruction == "RoboCop") {
                robot = new RoboCop();
            }
            else if (instruction == "Terminator") {
                robot = new Terminator();
            }
            else if (instruction == "TerminatorRoboCop") {
                robot = new TerminatorRoboCop();
            }
            else if (instruction == "BlueThunder") {
                robot = new BlueThunder();
            }
            else if (instruction == "Madbot") {
                robot = new Madbot();
            }
            else if (instruction == "RoboTank") {
                robot = new RoboTank();
            }
            else if (instruction == "UltimateRobot") {
                robot = new UltimateRobot();
            }

            if (robot) {
                robot->setName(robot_id);
                robot->setRobotList(&robotList);
                robot->setPosition(x, y);
                robotList.insert(robot_id, robotid_letters, robotid_numbers, x, y, robot);
                df.location(battlefield, N, M, CELL_WIDTH, x, y, robot_id);
            }

        }
        else {
            cout << "Error: Unknown instruction: " << instruction << endl;
        }
    }
    cout << "Initial Battlefield" << endl;

    df.displayBattlefield(battlefield, N, M, CELL_WIDTH, CELL_HEIGHT);
    cout << endl << endl << endl;

    for ( turn = 0; turn < turns; ++turn) {
        // Only the current robot takes an action
        Robot* currentRobot = robotList.getHead()->robot;

        // Perform actions based on the type of robot
        if (SeeingRobot* seeingRobot = dynamic_cast<SeeingRobot*>(currentRobot)) {
            seeingRobot->actionLook(battlefield, N, M);
        }
        if (MovingRobot* movingRobot = dynamic_cast<MovingRobot*>(currentRobot)) {
            movingRobot->actionMove(battlefield, N, M);
        }
        if (ShootingRobot* shootingRobot = dynamic_cast<ShootingRobot*>(currentRobot)) {
            shootingRobot->actionFire(battlefield, N, M);
        }
        if (SteppingRobot* steppingRobot = dynamic_cast<SteppingRobot*>(currentRobot)) {
            steppingRobot->actionStep(battlefield, N, M);
        }

        //robotList.display();
        currentRobot->displayInfo();
        df.displayBattlefield(battlefield, N, M, CELL_WIDTH, CELL_HEIGHT);
        cout << "Turn " << turn + 1 << " completed by " << currentRobot->getRobotId() << "." << endl<<endl;
        robotList.display();
        cout << endl << "--------------------------------------------------------------------------------------------" << endl;
        robotCount = countRobotsOnBattlefield(battlefield, N, M, CELL_WIDTH);
        if (robotCount <= 1) {
            cout << "Only " << robotCount << " robot left on the battlefield. Ending the program." << endl;
            break;
        }
        reEnterBattlefield(battlefield, N, M, &robotList);
        robotList.rotate(); // Move to the next robot in the linked list
}

   df.displayBattlefield(battlefield, N, M, CELL_WIDTH, CELL_HEIGHT, outfile);
outfile << "Turn " << turn << " : Final Turn" << endl;
outfile << "Only " << robotCount << " robot left on the battlefield. Ending the program." << endl;
outfile << endl << "--------------------------------------------------------------------------------------------" << endl;

    outfile.close();
    infile.close();

    // Deallocate memory for battlefield
    for (int i = 0; i < N; ++i) {
        delete[] battlefield[i];
    }
    delete[] battlefield;

    return 0;
}

string trim(string s) {
    s.erase(s.find_last_not_of(" \n\r\t") + 1);
    s.erase(0, s.find_first_not_of(" \n\r\t"));
    return s;
}
