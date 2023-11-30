#include "shape.h"
#include <iostream>
#include <assert.h>
namespace gxj {
template<typename T>
class unique_ptr {
public:
    explicit unique_ptr(T *ptr = nullptr) noexcept : ptr_(ptr) {}
    // disable copy ctor 
    unique_ptr(const unique_ptr &rhs) = delete;
    unique_ptr &operator=(const unique_ptr &rhs) = delete;

    // enable move ctor 
    unique_ptr(unique_ptr &&rhs) noexcept {
        ptr_ = rhs.release();
    }
    unique_ptr &operator=(unique_ptr &&rhs) noexcept {
        ptr_ = rhs.release();
        return *this;
    }

    ~unique_ptr() {
        if (ptr_) {
            delete ptr_;
        }
    }

    // mimic raw pointer's * and -> operator
    T &operator*() const noexcept {
        return *ptr_;
    }
    T *operator->() const noexcept {
        return ptr_;
    }

    // nullptr test
    operator bool() const noexcept { return ptr_; }

    /* give access to the managed ptr */
    T *get() const noexcept { return ptr_; }

    /* release the ownership of the managed ptr */
    T *release() noexcept {
        T * tmp = ptr_;
        ptr_ = nullptr;
        return tmp;
    }

    void swap(unique_ptr &&rhs) noexcept {
        using std::swap;
        swap(ptr_, rhs.ptr_);
    }
private:
    T *ptr_;
};

}


int main(int argc, const char *argv[]) {
    /* polymorphism */
    std::cout << "ptr1:" << std::endl;
    gxj::unique_ptr<shape> ptr1{create_shape(shape_type::circle)};
    ptr1->print();
    //auto raw_ptr = ptr1.get();
    
    
    // gxj::unique_ptr<shape> ptr2{ptr1}; // error call to deleted constructor
    std::cout << "ptr2:" << std::endl;
    gxj::unique_ptr<shape> ptr2{std::move(ptr1)};
    assert(!ptr1); // ptr1 should have been moved out
    std::cout << "ptr1's ptr has been move out" << std::endl;
    ptr2->print();
    
    // gxj::unique_ptr<shape> ptr3 = ptr2; // error call to deleted constructor
    std::cout << "ptr3:" << std::endl;
    gxj::unique_ptr<shape> ptr3 = std::move(ptr2);
    ptr3->print();
    
    /* release the ownership */
    std::cout << "released raw ptr:" << std::endl;
    shape *alone_ptr = ptr3.release();
    assert(!ptr3);
    alone_ptr->print();
    return 0;
}
