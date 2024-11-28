#ifndef LINKEDLISTSMART_H
#define LINKEDLISTSMART_H

#include "Sequence.h"
#include "ShrdPtr.h"
#include <stdexcept>

#define EMPTY_LINKED_LIST "LinkedListSmart is empty"
#define OUT_OF_RANGE "Index out of range"
#define SUBSEQ_ERR "Invalid subsequence indices"

template <typename T>
class LinkedListSmart : public Sequence<T> {
private:
    struct Node {
        ShrdPtr<T> data;
        ShrdPtr<Node> next;

        explicit Node(const T& item) : data(new T(item)), next(nullptr) {}
    };

    ShrdPtr<Node> head;
    size_t length;

public:
    LinkedListSmart() : head(nullptr), length(0) {}

    LinkedListSmart(const T* data, int size) : head(nullptr), length(0) {
        for (int i = 0; i < size; ++i) {
            Append(data[i]);
        }
    }

    ~LinkedListSmart() override = default;

    T GetFirst() const override {
        if (!head)
            throw std::out_of_range(EMPTY_LINKED_LIST);
        return *(head->data);
    }

    T GetLast() const override {
        if (!head)
            throw std::out_of_range(EMPTY_LINKED_LIST);
        ShrdPtr<Node> current = head;
        while (current->next) {
            current = current->next;
        }
        return *(current->data);
    }

    T Get(int index) const override {
        if (index < 0 || static_cast<size_t>(index) >= length)
            throw std::out_of_range(OUT_OF_RANGE);
        ShrdPtr<Node> current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return *(current->data);
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        if (startIndex < 0 || endIndex >= static_cast<int>(length) || startIndex > endIndex)
            throw std::out_of_range(SUBSEQ_ERR);

        auto* subseq = new LinkedListSmart<T>();
        ShrdPtr<Node> current = head;
        for (int i = 0; i <= endIndex; ++i) {
            if (i >= startIndex) {
                subseq->Append(*(current->data));
            }
            current = current->next;
        }
        return subseq;
    }

    int GetLength() const override {
        return static_cast<int>(length);
    }

    void Append(const T& item) override {
        auto newNode = ShrdPtr<Node>(new Node(item));
        if (!head) {
            head = newNode;
        } else {
            ShrdPtr<Node> current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
        ++length;
    }

    void Prepend(const T& item) override {
        auto newNode = ShrdPtr<Node>(new Node(item));
        newNode->next = head;
        head = newNode;
        ++length;
    }

    void InsertAt(const T& item, int index) override {
        if (index < 0 || index > static_cast<int>(length))
            throw std::out_of_range(OUT_OF_RANGE);
        if (index == 0) {
            Prepend(item);
        } else {
            ShrdPtr<Node> current = head;
            for (int i = 0; i < index - 1; ++i) {
                current = current->next;
            }
            auto newNode = ShrdPtr<Node>(new Node(item));
            newNode->next = current->next;
            current->next = newNode;
            ++length;
        }
    }

    Sequence<T>* Concat(Sequence<T>* list) const override {
        auto* newList = new LinkedListSmart<T>();
        ShrdPtr<Node> current = head;
        while (current) {
            newList->Append(*(current->data));
            current = current->next;
        }
        for (int i = 0; i < list->GetLength(); ++i) {
            newList->Append(list->Get(i));
        }
        return newList;
    }

    void Set(int index, const T& item) override {
        if (index < 0 || static_cast<size_t>(index) >= length)
            throw std::out_of_range(OUT_OF_RANGE);
        ShrdPtr<Node> current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        *(current->data) = item;
    }
};

#endif // LINKEDLISTSMART_H
