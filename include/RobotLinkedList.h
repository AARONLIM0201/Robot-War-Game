#ifndef ROBOTLINKEDLIST_H
#define ROBOTLINKEDLIST_H
#include "Robot.h"
#include <iostream>
using namespace std;
// Node structure for the linked list
#include "Robot.h" // Include the Robot class header
#include <string>
#include <queue>  // Include the queue header for std::queue
// Forward declaration of Robot class
class Robot;
const int CELL_WIDTH = 10;
const int CELL_HEIGHT = 1;

// Node structure for the linked list
struct Node {
    string robot_id,robotid_letters;  // Identifier for the robot
    int x, y,robotid_numbers,lives;         // Coordinates or other robot-related data
    Robot* robot;          // Pointer to the actual Robot object
    Node* next;            // Pointer to the next node in the list

    // Constructor to initialize a Node with all necessary data
    Node(const std::string& id, string letter, int number ,int posX, int posY, int lives, Robot* r = nullptr, Node* n = nullptr)
        : robot_id(id),robotid_letters(letter), robotid_numbers(number), x(posX), y(posY),lives(lives), robot(r), next(n) {}
};


// RobotLinkedList class
class RobotLinkedList {
private:
    Node *head; // Pointer to the first node in the list

public:
    // Constructor to initialize an empty list
    RobotLinkedList();

    // Destructor to clean up memory
    ~RobotLinkedList();
    // Insert a new robot at the beginning of the list
    void insert(const string &robot_id,string robotid_letters,int robotid_numbers, int x, int y,Robot *robot,int lives = 3);

    // Rotate to the next robot in the list
    void rotate();

    // Get the head of the linked list
    Node *getHead() const;

    // Remove a robot from the list by name
    void remove(const string& robot_id);

    // Search for a robot by name to deduct its life
    void deduct(const string& robot_id)const ;

    // Display the details of all robots in the list
    void display() const;

    Robot* search(const string& idOrName) const;

    void upgrade(const string& robot_id, const string& robotid_letters, int robotid_numbers, int x, int y, Robot* robot);

    string nehtid(const string& robot_id, const string& robotid_letters, int robotid_numbers, int x, int y, Robot* robot);
};

// Constructor
RobotLinkedList::RobotLinkedList() {
    head = nullptr;  // Initialize head to nullptr in the constructor
    srand(time(0)); // Seed for random number generation
}

// Destructor
RobotLinkedList::~RobotLinkedList() {
    if (head == nullptr) {
        return;
    }

    // Break the cycle temporarily
    Node *current = head;
    do {
        Node *next = current->next;
        delete current->robot;  // Delete the Robot object
        delete current;         // Delete the Node
        current = next;         // Move to the next Node
    } while (current != head);  // Stop when we reach the head again
}

// Insert a new robot at the beginning of the list
void RobotLinkedList::insert(const string &robot_id, string robotid_letters, int robotid_numbers, int x, int y, Robot *robot,int lives) {
    Node *newNode = new Node(robot_id, robotid_letters, robotid_numbers, x, y,lives, robot); // Create a new Node with robot_id, x, y, and robot pointer
    if (head == nullptr) {
        head = newNode; // If list is empty, newNode becomes the head
        head->next = head; // Points to itself since it's the only node
    } else {
        // Find the last node
        Node *current = head;
        while (current->next != head) {
            current = current->next;
        }
        current->next = newNode; // Insert newNode at the end of the list
        newNode->next = head; // Point newNode back to head to maintain circular list
    }
    return;
}



// Get the head of the linked list
Node *RobotLinkedList::getHead() const {
    return head;
}

// Rotate the linked list
void RobotLinkedList::rotate() {
    if (head == nullptr || head->next == head) {
        // No rotation needed for empty list or single node list
        return;
    }
    head = head->next;
}


void RobotLinkedList::remove(const string& robot_id)  {
    if (head == nullptr) {  // If list is empty
        cout << "List is empty. Cannot remove robot with ID: " << robot_id << std::endl;
        return;
    }

    Node* current = head;
    Node* prev = nullptr;

    // Special case: if the robot to be removed is the head
    if (head->robot_id == robot_id) {
        if (head->next == head) {  // Only one node in the list
            //delete head->robot;
            delete head;
            head = nullptr;  // List is now empty

        } else {  // More than one node in the list

            Node* last = head;
            while (last->next != head) {
                last = last->next;

            }
            last->next = head->next;
            Node* toDelete = head;
            head = head->next;
            //delete toDelete->robot;
            delete toDelete;

        }
        return;
    }

    // Traverse the list to find the node to remove
    prev = head;
    current = head->next;
    while (current != head && current->robot_id != robot_id) {
        prev = current;
        current = current->next;
    }

    // If the node to remove is found
    if (current->robot_id == robot_id) {
        // Display robot details before removal
        prev->next = current->next;
        //delete current->robot;
        //current->robot->decreaseLives;
        delete current;
        //display();
        return;

    } else {
        cout << "Robot with ID " << robot_id << " not found." << std::endl;
    }
}


void RobotLinkedList::display() const {
    if (head == nullptr) {
        cout << "List is empty." << endl;
        return;
    }

    Node* current = head;
    do {
        cout << "Robot ID: " << current->robot_id << endl;
        current = current->next;
    } while (current != head);
}

// Search for a robot by ID or name and return a pointer to the Robot object
Robot* RobotLinkedList::search(const string& idOrName) const {
    if (head == nullptr) {
        cout << "List is empty. Cannot search." << endl;
        return nullptr;
    }

    Node* current = head;
    do {
        if (current->robot_id == idOrName) {
            return current->robot;  // Return the Robot pointer if ID or name matches
        }
        current = current->next;
    } while (current != head);

    // If no robot with matching ID or name is found
    cout << "Robot with ID or name " << idOrName << " not found." << endl;
    return nullptr;
}

void RobotLinkedList::upgrade(const std::string& robot_id, const std::string& robotid_letters, int robotid_numbers, int x, int y, Robot* robot) {
    // Check if the robot is of type RoboCop
    if (robotid_letters == "TR") {
        // Count existing TerminatorRoboCop robots
        int count = 1; // Start with 1 to include the current robot being inserted
        Node* current = head;
        do {
        if (current->robotid_letters == "TR") {
        count++;
            }
            current = current->next;
        } while (current != head);

        // Generate new ID for RoboCop robot
        string new_id = "TR0" + std::to_string(count);

        // Insert the robot with the new ID

        insert(new_id, robotid_letters, count, x, y, robot);
    }
    else if (robotid_letters == "UR"){
        int count = 1; // Start with 1 to include the current robot being inserted
        Node* current = head;
        do {
        if (current->robotid_letters == "UR") {
        count++;
            }
            current = current->next;
        } while (current != head);

        // Generate new ID for RoboCop robot
        string new_id = "UR0" + std::to_string(count);
        insert(new_id, robotid_letters, count, x, y, robot);
    }
    else if (robotid_letters == "MB"){
        int count = 1; // Start with 1 to include the current robot being inserted
        Node* current = head;
        do {
        if (current->robotid_letters == "MB") {
        count++;
            }
            current = current->next;
        } while (current != head);

        // Generate new ID for RoboCop robot
        string new_id = "MB0" + std::to_string(count);
        insert(new_id, robotid_letters, count, x, y, robot);
    }
     else if (robotid_letters == "RT"){
        int count = 1; // Start with 1 to include the current robot being inserted
        Node* current = head;
        do {
        if (current->robotid_letters == "RT") {
        count++;
            }
            current = current->next;
        } while (current != head);

        // Generate new ID for RoboCop robot
        string new_id = "RT0" + std::to_string(count);
        insert(new_id, robotid_letters, count, x, y, robot);
    }

}

string RobotLinkedList::nehtid(const std::string& robot_id, const std::string& robotid_letters, int robotid_numbers, int x, int y, Robot* robot) {
    // Check if the robot is of type RoboCop
    if (robotid_letters == "TR") {
        // Count existing TerminatorRoboCop robots
        int count = 1; // Start with 1 to include the current robot being inserted
        Node* current = head;
        do {
        if (current->robotid_letters == "TR") {
        count++;
            }
            current = current->next;
        } while (current != head);

        // Generate new ID for RoboCop robot
        string new_id = "TR0" + std::to_string(count);
        return new_id;
        // Insert the robot with the new ID


    } else if(robotid_letters == "UR"){

        int count = 1; // Start with 1 to include the current robot being inserted
        Node* current = head;
        do {
        if (current->robotid_letters == "UR") {
        count++;
            }
            current = current->next;
        } while (current != head);


        string new_id = "UR0" + std::to_string(count);
        return new_id;

    }
    else if(robotid_letters == "MB"){

        int count = 1; // Start with 1 to include the current robot being inserted
        Node* current = head;
        do {
        if (current->robotid_letters == "MB") {
        count++;
            }
            current = current->next;
        } while (current != head);

        // Generate new ID for RoboCop robot
        string new_id = "MB0" + std::to_string(count);
        return new_id;

    }
    else if(robotid_letters == "RT"){

        int count = 1; // Start with 1 to include the current robot being inserted
        Node* current = head;
        do {
        if (current->robotid_letters == "RT") {
        count++;
            }
            current = current->next;
        } while (current != head);

        string new_id = "RT0" + std::to_string(count);
        return new_id;

    }
}

#endif // ROBOTLINKEDLIST_H
