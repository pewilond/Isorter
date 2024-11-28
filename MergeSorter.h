#ifndef MERGESORTER_H
#define MERGESORTER_H

#include "ISorter.h"

template <typename T>
class MergeSorter : public ISorter<T> {
public:
    Sequence<T>* Sort(Sequence<T>* seq, int (*cmp)(T, T)) override {
        MergeSort(seq, 0, seq->GetLength() - 1, cmp);
        return seq;
    }

private:
    void MergeSort(Sequence<T>* seq, int left, int right, int (*cmp)(T, T)) {
        if (left < right) {
            int mid = left + (right - left) / 2;

            MergeSort(seq, left, mid, cmp);
            MergeSort(seq, mid + 1, right, cmp);

            Merge(seq, left, mid, right, cmp);
        }
    }

    void Merge(Sequence<T>* seq, int left, int mid, int right, int (*cmp)(T, T)) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        Sequence<T>* LeftSeq = seq->GetSubsequence(left, mid);
        Sequence<T>* RightSeq = seq->GetSubsequence(mid + 1, right);

        int i = 0;
        int j = 0;
        int k = left;

        while (i < n1 && j < n2) {
            if (cmp(LeftSeq->Get(i), RightSeq->Get(j)) <= 0) {
                seq->Set(k, LeftSeq->Get(i));
                i++;
            } else {
                seq->Set(k, RightSeq->Get(j));
                j++;
            }
            k++;
        }

        while (i < n1) {
            seq->Set(k, LeftSeq->Get(i));
            i++;
            k++;
        }

        while (j < n2) {
            seq->Set(k, RightSeq->Get(j));
            j++;
            k++;
        }

        delete LeftSeq;
        delete RightSeq;
    }
};

#endif // MERGESORTER_H
