#ifndef STACK_H
#define STACK_H

#include <stdexcept>
#include <iostream>

// Node structure for linked-list based Stack
template <typename T>
struct StackNode {
    T data;
    StackNode* next;

    explicit StackNode(const T& value) : data(value), next(nullptr) {}
};

// Custom LIFO Stack implementation built from scratch using linked nodes
template <typename T>
class Stack {
private:
    StackNode<T>* topNode; // Pointer to top element of stack
    int count;             // Total number of elements in stack

public:
    // Constructor initializing an empty stack
    Stack() : topNode(nullptr), count(0) {}

    // Destructor freeing all allocated nodes
    ~Stack() {
        clear();
    }

    // Disable copy constructor and assignment operator for memory safety
    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    // Pushes an element onto top of stack - O(1) Time
    void push(const T& value) {
        StackNode<T>* newNode = new StackNode<T>(value);
        newNode->next = topNode;
        topNode = newNode;
        count++;
    }

    // Pops and returns the top element from stack - O(1) Time
    T pop() {
        if (isEmpty()) {
            throw std::underflow_error("Stack Underflow: Cannot pop from an empty stack.");
        }
        StackNode<T>* temp = topNode;
        T val = temp->data;
        topNode = topNode->next;
        delete temp;
        count--;
        return val;
    }

    // Peeks at top element without removing - O(1) Time
    T top() const {
        if (isEmpty()) {
            throw std::underflow_error("Stack Underflow: Cannot peek top of an empty stack.");
        }
        return topNode->data;
    }

    // Checks if stack is empty - O(1) Time
    bool isEmpty() const {
        return count == 0;
    }

    // Returns number of elements in stack - O(1) Time
    int size() const {
        return count;
    }

    // Clears all elements from stack - O(n) Time
    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }

    // Prints stack elements from top to bottom (for execution history debugging)
    void printStack() const {
        if (isEmpty()) {
            std::cout << "[Stack Empty]\n";
            return;
        }
        StackNode<T>* curr = topNode;
        std::cout << "Top -> ";
        while (curr != nullptr) {
            std::cout << curr->data << " ";
            curr = curr->next;
        }
        std::cout << "\n";
    }
};

#endif // STACK_H
