#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include <stdexcept>
#include <iostream>

// Custom array-based Circular Queue implementation for Round Robin process rotation
template <typename T>
class CircularQueue {
private:
    T* buffer;     // Dynamic array buffer holding queue elements
    int frontIdx;  // Index of the front element
    int rearIdx;   // Index of the rear element
    int capacity;  // Current total buffer capacity
    int count;     // Current number of elements stored

    // Resizes the internal circular buffer and realigns elements from index 0
    void resize(int newCapacity) {
        T* newBuffer = new T[newCapacity];
        for (int i = 0; i < count; ++i) {
            newBuffer[i] = buffer[(frontIdx + i) % capacity];
        }
        delete[] buffer;
        buffer = newBuffer;
        frontIdx = 0;
        rearIdx = count - 1;
        capacity = newCapacity;
    }

public:
    // Constructor with configurable initial capacity (default 10)
    explicit CircularQueue(int initialCapacity = 10)
        : buffer(nullptr), frontIdx(0), rearIdx(-1),
          capacity(initialCapacity > 0 ? initialCapacity : 10), count(0) {
        buffer = new T[capacity];
    }

    // Destructor freeing dynamic buffer
    ~CircularQueue() {
        delete[] buffer;
        buffer = nullptr;
    }

    // Disable copy constructor and assignment operator to enforce unique ownership
    CircularQueue(const CircularQueue&) = delete;
    CircularQueue& operator=(const CircularQueue&) = delete;

    // Enqueues an element to the rear of the circular queue - O(1) Amortized Time
    void enqueue(const T& value) {
        if (isFull()) {
            resize(capacity * 2);
        }
        rearIdx = (rearIdx + 1) % capacity;
        buffer[rearIdx] = value;
        count++;
    }

    // Dequeues and returns element at front of the circular queue - O(1) Time
    T dequeue() {
        if (isEmpty()) {
            throw std::underflow_error("CircularQueue Underflow: Cannot dequeue from an empty circular queue.");
        }
        T val = buffer[frontIdx];
        frontIdx = (frontIdx + 1) % capacity;
        count--;
        if (count == 0) {
            frontIdx = 0;
            rearIdx = -1;
        }
        return val;
    }

    // Rotates the front element to the rear of the queue (Round Robin rotation) - O(1) Time
    void rotate() {
        if (count > 1) {
            T item = dequeue();
            enqueue(item);
        }
    }

    // Returns front element without removing - O(1) Time
    T front() const {
        if (isEmpty()) {
            throw std::underflow_error("CircularQueue Underflow: Cannot access front of an empty circular queue.");
        }
        return buffer[frontIdx];
    }

    // Checks if queue is empty - O(1) Time
    bool isEmpty() const {
        return count == 0;
    }

    // Checks if queue buffer is full - O(1) Time
    bool isFull() const {
        return count == capacity;
    }

    // Returns current number of elements - O(1) Time
    int size() const {
        return count;
    }

    // Returns current capacity - O(1) Time
    int getCapacity() const {
        return capacity;
    }

    // Clears all elements - O(1) Time
    void clear() {
        frontIdx = 0;
        rearIdx = -1;
        count = 0;
    }
};

#endif // CIRCULAR_QUEUE_H
