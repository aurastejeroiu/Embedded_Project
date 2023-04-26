
#include <cstdint>
#include <cstring>
#include <iostream>

class Array {
public:
    int *arr;
    size_t capacity;
    size_t size;

    size_t getSize() const {
        return size;
    }

    size_t getCapacity() const {
        return capacity;
    }

    Array(size_t initial_capacity = 4) {
        arr = (int*) malloc(initial_capacity * sizeof(int));
        capacity = initial_capacity;
        size = 0;
        std::cout << "Acest mesaj provine din constructor\n";
    }

    Array(const Array& source) {
        arr = (int*) malloc(source.capacity * sizeof(int));
        capacity = source.capacity;
        size = source.size;
        memcpy(arr, source.arr, size * sizeof(int));
        std::cout << "Acest mesaj provine din constructorul de copiere\n";
    }

    Array& operator=(const Array& source) {
        if (this == &source) {
            return *this;
        }
        free(arr);
        arr = (int*) malloc(source.capacity * sizeof(int));
        capacity = source.capacity;
        size = source.size;
        memcpy(arr, source.arr, size * sizeof(int));
        std::cout << "Acest mesaj provine din operatorul de copiere prin atribuire\n";
        return *this;
    }
    Array & operator+=(const Array & source) {
        // free(arr); // obiectul curent aici exista deja, deci trebuie sa stergem memoria alocata in constructor inainte de a aloca altceva
        int * new_arr = (int*) malloc((source.dim + dim)* sizeof(int));
        memcpy(new_arr, arr, dim * sizeof(int));


        dim = source.dim;
        std::cout << "Acest mesaj provine din operatorul de copiere prin atribuire" << std::endl;
        return *this;
    }

    int& operator[](size_t index) {
        return arr[index];
    }

    void append(int value) {
        if (size == capacity) {
            capacity *= 2;
            arr = (int*) realloc(arr, capacity * sizeof(int));
        }
        arr[size++] = value;
    }


    ~Array() {
        free(arr);
        std::cout << "Acest mesaj provine din destructor\n";
    }

    
};


