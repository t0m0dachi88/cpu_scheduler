#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <stdexcept>
#include <iostream>
#include <utility>

// Default comparator function template returning true if a < b
template <typename T>
bool defaultMinHeapComparator(const T& a, const T& b) {
    return a < b;
}

// Custom array-based Min Heap implementation built from scratch without STL algorithms
template <typename T>
class MinHeap {
private:
    T* heap;        // Dynamic array buffer holding heap elements
    int capacity;   // Current capacity of heap array
    int count;      // Current number of elements stored
    bool (*compareFunc)(const T& a, const T& b); // Comparator function pointer

    // Parent and child index helper functions (0-based indexing)
    int parent(int i) const { return (i - 1) / 2; }
    int leftChild(int i) const { return (2 * i) + 1; }
    int rightChild(int i) const { return (2 * i) + 2; }

    // Resizes internal heap array buffer when full
    void resize(int newCapacity) {
        T* newHeap = new T[newCapacity];
        for (int i = 0; i < count; ++i) {
            newHeap[i] = heap[i];
        }
        delete[] heap;
        heap = newHeap;
        capacity = newCapacity;
    }

    // Swaps two elements in the heap buffer
    void swapElements(int i, int j) {
        T temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;
    }

    // Restores min heap property upwards after insertion - O(log n) Time
    void heapifyUp(int index) {
        while (index > 0 && compareFunc(heap[index], heap[parent(index)])) {
            swapElements(index, parent(index));
            index = parent(index);
        }
    }

    // Restores min heap property downwards after extraction - O(log n) Time
    void heapifyDown(int index) {
        while (leftChild(index) < count) {
            int smallest = leftChild(index);
            int right = rightChild(index);

            if (right < count && compareFunc(heap[right], heap[smallest])) {
                smallest = right;
            }

            if (compareFunc(heap[smallest], heap[index])) {
                swapElements(index, smallest);
                index = smallest;
            } else {
                break;
            }
        }
    }

public:
    // Constructor accepting optional initial capacity and comparator function pointer
    explicit MinHeap(int initialCapacity = 10, bool (*comp)(const T& a, const T& b) = defaultMinHeapComparator<T>)
        : heap(nullptr), capacity(initialCapacity > 0 ? initialCapacity : 10), count(0), compareFunc(comp) {
        heap = new T[capacity];
    }

    // Destructor freeing dynamic heap array
    ~MinHeap() {
        delete[] heap;
        heap = nullptr;
    }

    // Disable copy constructor and assignment operator to prevent shallow pointer copy
    MinHeap(const MinHeap&) = delete;
    MinHeap& operator=(const MinHeap&) = delete;

    // Inserts an element into the Min Heap - O(log n) Time
    void insert(const T& value) {
        if (count >= capacity) {
            resize(capacity * 2);
        }
        heap[count] = value;
        heapifyUp(count);
        count++;
    }

    // Extracts and returns minimum element from heap - O(log n) Time
    T extractMin() {
        if (isEmpty()) {
            throw std::underflow_error("MinHeap Underflow: Cannot extract min from an empty heap.");
        }
        T minVal = heap[0];
        heap[0] = heap[count - 1];
        count--;
        if (count > 0) {
            heapifyDown(0);
        }
        return minVal;
    }

    // Peeks minimum element without removing - O(1) Time
    T peekMin() const {
        if (isEmpty()) {
            throw std::underflow_error("MinHeap Underflow: Cannot peek min of an empty heap.");
        }
        return heap[0];
    }

    // Checks if heap is empty - O(1) Time
    bool isEmpty() const {
        return count == 0;
    }

    // Returns number of elements in heap - O(1) Time
    int size() const {
        return count;
    }

    // Returns current capacity - O(1) Time
    int getCapacity() const {
        return capacity;
    }

    // Clears all elements from heap - O(1) Time
    void clear() {
        count = 0;
    }
};

#endif // MIN_HEAP_H
