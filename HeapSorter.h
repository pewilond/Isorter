#ifndef HEAPSORTER_H
#define HEAPSORTER_H

#include "ISorter.h"

template <typename T>
class HeapSorter : public ISorter<T> {
public:
    Sequence<T>* Sort(Sequence<T>* seq, int (*cmp)(T, T)) override {
        int n = seq->GetLength();

        for (int i = n / 2 - 1; i >= 0; i--)
            Heapify(seq, n, i, cmp);

        for (int i = n - 1; i >= 0; i--) {

            T temp = seq->Get(0);
            seq->Set(0, seq->Get(i));
            seq->Set(i, temp);

            Heapify(seq, i, 0, cmp);
        }

        return seq;
    }

private:
    void Heapify(Sequence<T>* seq, int n, int i, int (*cmp)(T, T)) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && cmp(seq->Get(left), seq->Get(largest)) > 0)
            largest = left;

        if (right < n && cmp(seq->Get(right), seq->Get(largest)) > 0)
            largest = right;

        if (largest != i) {
            T swap = seq->Get(i);
            seq->Set(i, seq->Get(largest));
            seq->Set(largest, swap);

            Heapify(seq, n, largest, cmp);
        }
    }
};

#endif // HEAPSORTER_H
