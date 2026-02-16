#ifndef TYPED_ARRAY
#define TYPED_ARRAY

#include <assert.h>
#include <iostream>
#include <stdexcept>

template <typename ElementType>
class TypedArray {

public:
    TypedArray();
    TypedArray(const TypedArray& other);
    TypedArray& operator=(const TypedArray& other);
    ~TypedArray();

    ElementType &get(int index);
    ElementType &safe_get(int index) const;
    int size() const;

    void set(int index, ElementType value);

    void push(const ElementType& value);
    void push_front(const ElementType& value);
    ElementType pop();
    ElementType pop_front();

    TypedArray concat(const TypedArray& other) const;
    TypedArray& reverse();

    TypedArray operator+(const TypedArray& other) const;

private:
    int capacity, origin, end;
    ElementType * buffer;
    const int INITIAL_CAPACITY = 10;

    int index_to_offset(int index) const;
    int offset_to_index(int offset) const;
    bool out_of_buffer(int offset) const;
    void extend_buffer(void);
};

template <typename ElementType>
TypedArray<ElementType>::TypedArray() {
    buffer = new ElementType[INITIAL_CAPACITY]();
    capacity = INITIAL_CAPACITY;
    origin = capacity / 2;
    end = origin;
}

template <typename ElementType>
TypedArray<ElementType>::TypedArray(const TypedArray& other) : TypedArray() {
    *this = other;
}

template <typename ElementType>
TypedArray<ElementType>& TypedArray<ElementType>::operator=(const TypedArray<ElementType>& other) {
    if (this != &other) {
        delete[] buffer;
        buffer = new ElementType[other.capacity]();
        capacity = other.capacity;
        origin = other.origin;
        end = origin;
        for (int i = 0; i < other.size(); i++) {
            set(i, other.safe_get(i));
        }
    }
    return *this;
}

template <typename ElementType>
TypedArray<ElementType>::~TypedArray() {
    delete[] buffer;
}

template <typename ElementType>
ElementType &TypedArray<ElementType>::get(int index) {
    if (index < 0) {
        throw std::range_error("Out of range index in array");
    }
    if (index >= size()) {
        ElementType x;
        set(index, x);
    }
    return buffer[index_to_offset(index)];
}

template <typename ElementType>
ElementType &TypedArray<ElementType>::safe_get(int index) const {
    if (index < 0 || index >= size()) {
        throw std::range_error("Out of range index in array");
    }
    return buffer[index_to_offset(index)];
}

template <typename ElementType>
int TypedArray<ElementType>::size() const {
    return end - origin;
}

template <typename ElementType>
void TypedArray<ElementType>::set(int index, ElementType value) {
    if (index < 0) {
        throw std::range_error("Negative index in array");
    }
    while (out_of_buffer(index_to_offset(index))) {
        extend_buffer();
    }
    buffer[index_to_offset(index)] = value;
    if (index >= size()) {
        end = index_to_offset(index + 1);
    }
}

template <typename ElementType>
std::ostream &operator<<(std::ostream &os, TypedArray<ElementType> &array) {
    os << '[';
    for (int i = 0; i < array.size(); i++) {
        os << array.get(i);
        if (i < array.size() - 1) os << ",";
    }
    os << ']';
    return os;
}

template <typename ElementType>
int TypedArray<ElementType>::index_to_offset(int index) const {
    return index + origin;
}

template <typename ElementType>
int TypedArray<ElementType>::offset_to_index(int offset) const {
    return offset - origin;
}

template <typename ElementType>
bool TypedArray<ElementType>::out_of_buffer(int offset) const {
    return offset < 0 || offset >= capacity;
}

template <typename ElementType>
void TypedArray<ElementType>::extend_buffer() {
    auto temp = new ElementType[2 * capacity]();
    int new_origin = capacity - (end - origin) / 2;
    int new_end = new_origin + (end - origin);

    for (int i = 0; i < size(); i++) {
        temp[new_origin + i] = safe_get(i);
    }

    delete[] buffer;
    buffer = temp;

    capacity = 2 * capacity;
    origin = new_origin;
    end = new_end;
}

template <typename ElementType>
void TypedArray<ElementType>::push(const ElementType& value) {
    set(size(), value);
}

template <typename ElementType>
void TypedArray<ElementType>::push_front(const ElementType& value) {
    while (origin == 0) extend_buffer();
    origin--;
    buffer[origin] = value;
}

template <typename ElementType>
ElementType TypedArray<ElementType>::pop() {
    if (size() == 0) {
        throw std::range_error("Cannot pop from an empty array");
    }
    ElementType v = buffer[end - 1];
    buffer[end - 1] = ElementType();
    end--;
    return v;
}

template <typename ElementType>
ElementType TypedArray<ElementType>::pop_front() {
    if (size() == 0) {
        throw std::range_error("Cannot pop from an empty array");
    }
    ElementType v = buffer[origin];
    buffer[origin] = ElementType();
    origin++;
    return v;
}

template <typename ElementType>
TypedArray<ElementType> TypedArray<ElementType>::concat(const TypedArray<ElementType>& other) const {
    TypedArray<ElementType> r;
    int n = size();
    int m = other.size();
    for (int i = 0; i < n; i++) r.set(i, safe_get(i));
    for (int j = 0; j < m; j++) r.set(n + j, other.safe_get(j));
    return r;
}

template <typename ElementType>
TypedArray<ElementType>& TypedArray<ElementType>::reverse() {
    int n = size();
    for (int i = 0; i < n / 2; i++) {
        ElementType tmp = safe_get(i);
        buffer[index_to_offset(i)] = safe_get(n - 1 - i);
        buffer[index_to_offset(n - 1 - i)] = tmp;
    }
    return *this;
}

template <typename ElementType>
TypedArray<ElementType> TypedArray<ElementType>::operator+(const TypedArray<ElementType>& other) const {
    return concat(other);
}

#endif
