#include <cstdint>
#include <cstring>
#include <iostream>

class Array {
private:
    int* mArr;
    size_t mCapacity;
    size_t mSize;

public:
    size_t getSize() const {
        return mSize;
    }

    size_t getCapacity() const {
        return mCapacity;
    }

    Array(size_t initialCapacity = 4) {
        mArr = (int*)malloc(initialCapacity * sizeof(int));
        mCapacity = initialCapacity;
        mSize = 0;
        std::cout << "This message is from constructor\n";
    }

    Array(const Array& source) {
        mArr = (int*)malloc(source.mCapacity * sizeof(int));
        mCapacity = source.mCapacity;
        mSize = source.mSize;
        memcpy(mArr, source.mArr, mSize * sizeof(int));
        std::cout << "This message is from copy constructor\n";
    }

    Array& operator=(const Array& source) {
        if (this == &source) {
            return *this;
        }
        free(mArr);
        mArr = (int*)malloc(source.mCapacity * sizeof(int));
        mCapacity = source.mCapacity;
        mSize = source.mSize;
        memcpy(mArr, source.mArr, mSize * sizeof(int));
        std::cout << "This message is from copy assignment operator\n";
    }

    Array operator+(const Array& source) const {
        Array result(mSize + source.mSize);
        memcpy(result.mArr, mArr, mSize * sizeof(int));
        memcpy(result.mArr + mSize, source.mArr, source.mSize * sizeof(int));
        result.mSize = mSize + source.mSize;
        std::cout << "This message is from addition operator\n";
        return result;
    }

    int& operator[](size_t index) {
        if (index > mSize)
            return mArr[mSize-1];
        else
            return mArr[index];
    }

    void append(int value) {
        if (mSize == mCapacity) {
            mCapacity *= 2;
            mArr = (int*)realloc(mArr, mCapacity * sizeof(int));
        }
        mArr[mSize++] = value;
    }

    bool push(int value, size_t index) {
        if (index > mSize) {
            return false;
        }

        if (mSize == mCapacity) {
            size_t newCapacity = mCapacity * 2;
            int* newArr = new int[newCapacity];
            memcpy(newArr, mArr, mSize * sizeof(int));
            delete[] mArr;
            mArr = newArr;
            mCapacity = newCapacity;
        }
        for (size_t i = mSize; i > index; i--) {
            mArr[i] = mArr[i - 1];
        }
        mArr[index] = value;
        mSize++;

        return true;
    }

    bool pop() {
        if (mSize > 0) {
            mSize--;
            return true;
        }
        else {
            return false;
        }
    }


    ~Array() {
        free(mArr);
        std::cout << "This message is from destructor\n";
    }
};


int main() {
    Array arr1(4);
    std::cout << "arr1 size: " << arr1.getSize() << ", capacity: " << arr1.getCapacity() << std::endl;

    arr1.append(1);
    arr1.append(2);
    arr1.append(3);
    arr1.append(4);
    arr1.append(5);
    std::cout << "arr1 size: " << arr1.getSize() << ", capacity: " << arr1.getCapacity() << std::endl;
    Array arr2,arr4, arr5;
    arr5 = arr4 = arr2 = arr1;
    std::cout << "arr2 size: " << arr2.getSize() << ", capacity: " << arr2.getCapacity() << std::endl;
    
    Array arr3(4);
    arr3 = arr1;
    std::cout << "arr3 size: " << arr3.getSize() << ", capacity: " << arr3.getCapacity() << std::endl;
    std::cout << "arr1[0]: " << arr1[0] << ", arr1[2]: " << arr1[2] << std::endl;

    // use push method to add element to end of array
    arr1.push(6, 2);
    std::cout << "arr1 size: " << arr1.getSize() << ", capacity: " << arr1.getCapacity() << std::endl;

    // use pop method to remove element from end of array
    arr1.pop();
    std::cout << "arr1 size: " << arr1.getSize() << ", capacity: " << arr1.getCapacity() << std::endl;

    return 0;
}
