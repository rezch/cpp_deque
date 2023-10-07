#include <iostream>
#include <vector>
#include <cassert>

/*
 * Напишите шаблонный класс Deque, который представляет собой двустороннюю очередь и поддерживает
 * - вставку в начало и в конец в лучшем случае за константное время, а в худшем случае за линейное в зависимости от
 *  текущего количества элементов в деке;
 * - произвольный доступ по порядковому номеру к любым элементам дека за константное время
 * - удаление одного элемента из конца или из начала дека за константное время
 * Более подробно с интерфейсом класса Deque можно ознакомиться в вызывающем коде в функции main
 *
 * С учетом данных ограничений Deque может быть удобно реализовать с помощью Ring Buffer'а
 */

template<typename T>
class Deque {
public:
    Deque() : base(0), size(0), capacity(2), buffer(new T(2)) {};

    ~Deque() {
        delete[] buffer;
    };

    void PushBack(T value) {
        (*this)[size] = value;
        ++size;
        reserve();
    };

    void PushFront(T value) {
        base_move_left();
        (*this)[0] = value;
        ++size;
        reserve();
    };

    void PopBack() {
        --size;
    };

    void PopFront() {
        --size;
        base_move_right();
    };

    size_t Size() {
        return size;
    };

    bool Empty() {
        if (size == 0) {
            return true;
        }
        return false;
    };

    T Front() {
        return (*this)[0];
    };
    T Back() {
        return (*this)[size - 1];
    };

    void Clear() {
        delete[] buffer;
        base = 0;
        size = 0;
        capacity = 2;
        buffer = new T[2];
    };

    T &operator[](int index) {
        return buffer[(base + index) % capacity];
    };

private:
    void reserve() {
        if (size < capacity) {
            return;
        }

        T *new_buffer = new T[capacity * 2];
        for (int i = 0; i < size; ++i) {
            new_buffer[i] = (*this)[i];
        }
        delete[] buffer;
        buffer = new_buffer;
        base = 0;
        capacity *= 2;
    };

    void base_move_left() {
        if (base == 0) {
            base = capacity;
        }
        --base;
    };

    void base_move_right() {
        if (base == capacity - 1) {
            base = 0;
            return;
        }
        ++base;
    };

    size_t base{}; // index of first element
    size_t size{}; // number of elements
    size_t capacity{}; // size of allocated memory
    T *buffer; // ring buffer
};

int main() {
    Deque<int> deque;

    assert(deque.Size() == 0);
    assert(deque.Empty());

    deque.PushBack(1);
    assert(deque.Size() == 1);
    assert(!deque.Empty());
    assert(deque[0] == 1);
    assert(deque.Front() == 1);
    assert(deque.Back() == 1);

    deque.PopFront();
    assert(deque.Size() == 0);
    assert(deque.Empty());

    for (int i = 0; i < 20; ++i) {
        if (i % 2) {
            deque.PushBack(i);
        } else {
            deque.PushFront(i);
        }
    }

    for (size_t i = 0; i < deque.Size(); ++i) {
        std::cout << deque[i] << " ";
    }
    std::cout << std::endl;

    assert(deque.Size() == 20);

    const int beforeBack = deque[deque.Size() - 2];
    const int afterFront = deque[1];

    deque.PopBack();
    deque.PopFront();

    assert(deque.Back() == beforeBack);
    assert(deque.Front() == afterFront);

    std::string s;
    for (size_t i = 0; i < deque.Size(); ++i) {
        s += std::to_string(deque[i]) + " ";
    }

    for (size_t i = 0; i < deque.Size(); ++i) {
        deque.PushBack(deque.Front());
        deque.PopFront();
    }

    std::string s2;
    for (size_t i = 0; i < deque.Size(); ++i) {
        s2 += std::to_string(deque[i]) + " ";
    }

    assert(s == s2);

    assert(deque.Size() == 18);
    deque.Clear();
    assert(deque.Size() == 0);
    assert(deque.Empty());

    return 0;
}
