#ifndef CUSTOM_QUEUE_H
#define CUSTOM_QUEUE_H

#include <stdexcept>
#include <iostream>

// Node structure for linked-list based CustomQueue
template <typename T>
struct QueueNode {
    T data;
    QueueNode* next;

    explicit QueueNode(const T& value) : data(value), next(nullptr) {}
};

// Custom FIFO Queue implementation built from scratch using linked nodes
template <typename T>
class CustomQueue {
private:
    QueueNode<T>* frontNode; // Pointer to head (front) of queue
    QueueNode<T>* rearNode;  // Pointer to tail (rear) of queue
    int count;               // Current number of elements in queue

public:
    // Constructor initializing an empty queue
    CustomQueue() : frontNode(nullptr), rearNode(nullptr), count(0) {}

    // Destructor freeing memory for all remaining nodes
    ~CustomQueue() {
        clear();
    }

    // Disable copy operations to avoid shallow pointer copying
    CustomQueue(const CustomQueue&) = delete;
    CustomQueue& operator=(const CustomQueue&) = delete;

    // Enqueues an element to the rear of the queue - O(1) Time
    void enqueue(const T& value) {
        QueueNode<T>* newNode = new QueueNode<T>(value);
        if (isEmpty()) {
            frontNode = rearNode = newNode;
        } else {
            rearNode->next = newNode;
            rearNode = newNode;
        }
        count++;
    }

    // Dequeues and returns the element at the front of the queue - O(1) Time
    T dequeue() {
        if (isEmpty()) {
            throw std::underflow_error("Queue Underflow: Cannot dequeue from an empty queue.");
        }
        QueueNode<T>* temp = frontNode;
        T value = temp->data;
        frontNode = frontNode->next;
        if (frontNode == nullptr) {
            rearNode = nullptr;
        }
        delete temp;
        count--;
        return value;
    }

    // Returns element at the front without dequeuing - O(1) Time
    T front() const {
        if (isEmpty()) {
            throw std::underflow_error("Queue Underflow: Cannot access front of an empty queue.");
        }
        return frontNode->data;
    }

    // Checks if queue is empty - O(1) Time
    bool isEmpty() const {
        return count == 0;
    }

    // Returns current number of elements - O(1) Time
    int size() const {
        return count;
    }

    // Deallocates all nodes in the queue - O(n) Time
    void clear() {
        while (!isEmpty()) {
            dequeue();
        }
    }
};

#endif // CUSTOM_QUEUE_H
