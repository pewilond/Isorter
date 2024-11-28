#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <cstddef>
#include <stdexcept>

#define INDEX_OUT_OF_RANGE "Index out of range"
#define EMPTYLINKEDLIST "LinkedList is empty"

template<class T>
class LinkedListNode {
public:
    T value;
    LinkedListNode<T>* next;

    LinkedListNode(const T& value, LinkedListNode<T>* next = nullptr);
};

template<class T>
class LinkedList {
private:
    LinkedListNode<T>* head;
    LinkedListNode<T>* tail;
    size_t length;

    void Clear();

public:
    LinkedList();
    LinkedList(const T* items, int count);
    LinkedList(const LinkedList<T>& list);
    ~LinkedList();

    LinkedList<T>* GetSubList(int startIndex, int endIndex);
    void Append(T item);
    void Prepend(T item);
    void InsertAt(T item, int index);
    size_t GetLength() const;
    T GetFirst() const;
    T GetLast() const;
    T Get(int index) const;
    void Set(int index, const T& value);
};

#include "LinkedList.tpp"

#endif // LINKEDLIST_H
