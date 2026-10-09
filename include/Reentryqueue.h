#ifndef REENTRYQUEUE_H
#define REENTRYQUEUE_H


class Node;

class ReentryQueue {
private:
    struct QueueNode {
        Node* robotNode;
        QueueNode* next;

        QueueNode(Node* node) : robotNode(node), next(nullptr) {}
    };

    QueueNode* front;
    QueueNode* rear;

public:
    ReentryQueue();
    ~ReentryQueue();

    void enqueue(Node* node);
    Node* dequeue();
    bool isEmpty() const;
};

ReentryQueue::ReentryQueue() : front(nullptr), rear(nullptr) {}

// Destructor
ReentryQueue::~ReentryQueue() {
    while (!isEmpty()) {
        dequeue();
    }
}

// Enqueue a node to the queue
void ReentryQueue::enqueue(Node* node) {
    QueueNode* newNode = new QueueNode(node);
    if (isEmpty()) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
}

// Dequeue a node from the queue
Node* ReentryQueue::dequeue() {
    if (isEmpty()) {
        return nullptr;
    }

    QueueNode* temp = front;
    Node* node = front->robotNode;
    front = front->next;
    if (front == nullptr) {
        rear = nullptr;
    }
    delete temp;
    return node;
}

// Check if the queue is empty
bool ReentryQueue::isEmpty() const {
    return front == nullptr;
}

#endif // REENTRYQUEUE_H




