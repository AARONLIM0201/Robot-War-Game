#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
using namespace std;

template<class T>
struct QueueNode {
    T data;
    QueueNode<T>* next;

    QueueNode(T x) {
        data = x;
        next = nullptr;
    }
};

template<class T>
class Queue {
private:
    QueueNode<T>* front_;
    QueueNode<T>* back_;
    int sz;

public:
    int robotcount=0;
    int robotcounter()
    {
        return robotcount;
    }
    Queue() : front_(nullptr), back_(nullptr), sz(0) {}

    ~Queue() {
        while (!empty()) {
            dequeue();
        }
    }

    Queue(const Queue<T>& q) : front_(nullptr), back_(nullptr), sz(0) {
        QueueNode<T>* ptr = q.front_;
        while (ptr != nullptr) {
            enqueue(ptr->data);
            ptr = ptr->next;
        }
    }

   void clear() {
        while (!empty()) {
            dequeue();
        }
        robotcount = 0; // Reset robot count to 0
    }
    Queue& operator=(const Queue<T>& q) {
        if (this != &q) {
            while (!empty()) {
                dequeue();
            }
            QueueNode<T>* ptr = q.front_;
            while (ptr != nullptr) {
                enqueue(ptr->data);
                ptr = ptr->next;
            }
        }
        return *this;
    }

    Queue(Queue<T>&& q) : front_(q.front_), back_(q.back_), sz(q.sz) {
        q.front_ = nullptr;
        q.back_ = nullptr;
        q.sz = 0;
    }

    Queue& operator=(Queue<T>&& q) {
        if (this != &q) {
            std::swap(sz, q.sz);
            std::swap(front_, q.front_);
            std::swap(back_, q.back_);
        }
        return *this;
    }

    bool empty() const {
        return front_ == nullptr;
    }

    int size() const {
        return sz;
    }

    T front() const {
        return front_->data;
    }

    T back() const {
        return back_->data;
    }

void enqueue(T x) {

        if (empty()) {
            front_ = new QueueNode<T>(x);
            back_ = front_;
        } else {
            back_->next = new QueueNode<T>(x);
            back_ = back_->next;
            //back_ = back_->next;
        }
        sz++;
        robotcount++;
    }

    void dequeue() {
        if (!empty()) {
            QueueNode<T>* p = front_;
            front_ = front_->next;
            front_ = front_->next;
            delete p;
            sz--;
            robotcount--;
            robotcount--;
        } else {
            cout << "Queue is empty\n";
            exit(EXIT_FAILURE);
        }
    }

    template <class U>
    friend ostream& operator<< (ostream& os, const Queue<U>& q);

     void displayQueue() const {
        if (empty()) {
            cout << "Queue is empty\n";
            return;
        }
        QueueNode<T>* ptr = front_;
        cout << "Queue contents: ";
        while (ptr != nullptr) {
            cout <<robotcount<< ' ';
            ptr = ptr->next;
        }
        cout << endl;
    }


};

template <class T>
ostream& operator<< (ostream& os, const Queue<T>& q) {
    QueueNode<T>* ptr = q.front_;
    while (ptr != nullptr) {
        os << ptr->data << ' ';
        ptr = ptr->next;
    }
    return os;
}



#endif // QUEUE_H
