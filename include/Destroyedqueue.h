#ifndef DESTROYEDQUEUE_H_INCLUDED
#define DESTROYEDQUEUE_H_INCLUDED
#ifndef DESTROYEDQUEUE_H
#define DESTROYEDQUEUE_H
#include "RobotLinkedList.h"
#include <queue>

class DestroyedQueue {
private:
    std::queue<Robot*> queue;

public:
    // Add a robot to the destroyed queue
    void enqueue(Robot* robot) {
        queue.push(robot);
    }

    // Check if the queue is empty
    bool isEmpty() const {
        return queue.empty();
    }

    // Get the front robot from the destroyed queue
    Robot* dequeue() {
        if (queue.empty()) return nullptr;

        Robot* robot = queue.front();
        queue.pop();
        return robot;
    }
};

#endif // DESTROYEDQUEUE_H



#endif // DESTROYEDQUEUE_H_INCLUDED
