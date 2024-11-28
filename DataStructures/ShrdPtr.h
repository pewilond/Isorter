#ifndef SHRDPTR_H
#define SHRDPTR_H

#include <cstddef>

template<typename T>
class ShrdPtr {
private:
    T *ptr;
    size_t *ref_count;

    void add_ref() {
        if (ref_count) {
            ++(*ref_count);
        }
    }

    void release() {
        if (ref_count) {
            --(*ref_count);
            if (*ref_count == 0) {
                delete ptr;
                delete ref_count;
            }
            ptr = nullptr;
            ref_count = nullptr;
        }
    }

public:
    explicit ShrdPtr(T *p = nullptr)
            : ptr(p), ref_count(p ? new size_t(1) : nullptr) {}

    ShrdPtr(const ShrdPtr<T> &other)
            : ptr(other.ptr), ref_count(other.ref_count) {
        add_ref();
    }

    template<typename U>
    ShrdPtr(const ShrdPtr<U> &other)
            : ptr(other.ptr), ref_count(other.ref_count) {
        add_ref();
    }

    ShrdPtr<T> &operator=(const ShrdPtr<T> &other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            ref_count = other.ref_count;
            add_ref();
        }
        return *this;
    }

    template<typename U>
    ShrdPtr<T> &operator=(const ShrdPtr<U> &other) {
        release();
        ptr = other.ptr;
        ref_count = other.ref_count;
        add_ref();
        return *this;
    }

    ~ShrdPtr() {
        release();
    }

    T &operator*() const {
        return *ptr;
    }

    T *operator->() const {
        return ptr;
    }

    explicit operator bool() const {
        return ptr != nullptr;
    }

    bool operator!() const {
        return ptr == nullptr;
    }

    void reset(T *p = nullptr) {
        release();
        if (p) {
            ptr = p;
            ref_count = new size_t(1);
        } else {
            ptr = nullptr;
            ref_count = nullptr;
        }
    }

    size_t use_count() const {
        return ref_count ? *ref_count : 0;
    }
};

template<typename T>
class ShrdPtr<T[]> {
private:
    T *ptr;
    size_t *ref_count;

    void add_ref() {
        if (ref_count) {
            ++(*ref_count);
        }
    }

    void release() {
        if (ref_count) {
            --(*ref_count);
            if (*ref_count == 0) {
                delete[] ptr;
                delete ref_count;
            }
            ptr = nullptr;
            ref_count = nullptr;
        }
    }

public:
    explicit ShrdPtr(T *p = nullptr)
            : ptr(p), ref_count(p ? new size_t(1) : nullptr) {}

    ShrdPtr(const ShrdPtr<T[]> &other)
            : ptr(other.ptr), ref_count(other.ref_count) {
        add_ref();
    }

    template<typename U>
    ShrdPtr(const ShrdPtr<U[]> &other)
            : ptr(other.ptr), ref_count(other.ref_count) {
        add_ref();
    }

    ShrdPtr<T[]> &operator=(const ShrdPtr<T[]> &other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            ref_count = other.ref_count;
            add_ref();
        }
        return *this;
    }

    template<typename U>
    ShrdPtr<T[]> &operator=(const ShrdPtr<U[]> &other) {
        release();
        ptr = other.ptr;
        ref_count = other.ref_count;
        add_ref();
        return *this;
    }

    ~ShrdPtr() {
        release();
    }

    T &operator*() const {
        return *ptr;
    }

    T *operator->() const {
        return ptr;
    }


    explicit operator bool() const {
        return ptr != nullptr;
    }

    bool operator!() const {
        return ptr == nullptr;
    }

    void reset(T *p = nullptr) {
        release();
        if (p) {
            ptr = p;
            ref_count = new size_t(1);
        } else {
            ptr = nullptr;
            ref_count = nullptr;
        }
    }

    size_t use_count() const {
        return ref_count ? *ref_count : 0;
    }

    T &operator[](size_t index) const {
        return ptr[index];
    }

};

#endif // SHRDPTR_H