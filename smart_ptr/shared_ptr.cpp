#include <iostream>
#include <assert.h>
#include "shape.h"

namespace gxj {

class counter {
public:
    counter() : count_(1) {}

    void add_count() { count_++; }

    size_t reduce_count() { return --count_; }

    size_t get_count() { return count_; }
private:
    size_t count_;
};

template<typename T>
class shared_ptr {
public:
    explicit shared_ptr(T *ptr = nullptr) noexcept : ptr_(ptr) {
        if (ptr) {
            counter_ = new counter();
        }
    }

    ~shared_ptr() {
        // count-1后为0，则表明这是最后一个管理这个ptr_的shared_ptr,则执行delete ptr_
        if (ptr_ && !counter_->reduce_count()) {
            delete ptr_;
            delete counter_;
        }
    }
    // copy and move enable
    shared_ptr(const shared_ptr &rhs) {
        if (rhs.ptr_) {
            rhs.counter_->add_count();
            ptr_ = rhs.get();
            counter_ = rhs.get_counter();
        }
    }
    shared_ptr &operator=(const shared_ptr &rhs) {
        if (ptr_) {
            if(!counter_->reduce_count()){
                delete ptr_;
                delete counter_;
                ptr_ = nullptr;
                counter_ = nullptr;
            }
        }
        if (rhs.ptr_) {
            rhs.counter_->add_count();
            ptr_ = rhs.get();
            counter_ = rhs.get_counter();
        }
        return *this;
    }

    shared_ptr(shared_ptr &&rhs) {
        if(rhs.ptr_){
            ptr_ = rhs.get();
            counter_ = rhs.get_counter();
            rhs.ptr_ = nullptr;
            rhs.counter_ = nullptr;
        }
    }
    shared_ptr &operator=(shared_ptr &&rhs) {
        if (ptr_) {
            if (!counter_->reduce_count()){
                delete ptr_;
                delete counter_;
                ptr_ = nullptr;
                counter_ = nullptr;
            }
        }
        if (rhs.ptr_) {
            ptr_ = rhs.get();
            counter_ = rhs.get_counter();
            rhs.ptr_ = nullptr;
            rhs.counter_ =nullptr;
        }
        return *this;
    }

    T &operator*() const noexcept { return *ptr_; } 
    T *operator->() const noexcept { return ptr_; }

    operator bool() const noexcept { return ptr_; }

    T *get() const noexcept { return ptr_; }
    counter *get_counter() const noexcept { return counter_; }

    size_t use_count() const noexcept { 
        return (ptr_)? counter_->get_count() : 0;    
    }
private:
    T *ptr_;
    counter * counter_;
};
} // end of namespace gxj

int main(int argc, const char *argv[]) {
    // copy will increase count
    gxj::shared_ptr<shape> ptr1{create_shape(shape_type::triangle)};
    gxj::shared_ptr<shape> ptr2 = ptr1; // copy assignment
    gxj::shared_ptr<shape> ptr3{ptr2}; // copy ctor
    assert(ptr3.use_count() == 3);
    ptr3->print();

    // move decrease count
    gxj::shared_ptr<shape> ptr4{std::move(ptr3)};
    assert(!ptr3); // ptr3 be moved
    assert(ptr4.use_count() == 3);

    // out of scope,count reduce
    {
        gxj::shared_ptr<shape> in_scope = ptr1;
        assert(ptr1.use_count() == 4);
    }
    assert(ptr1.use_count() == 3);

    gxj::shared_ptr<shape> ptr5{create_shape(shape_type::rectangle)};
    gxj::shared_ptr<shape> ptr6 = ptr5;
    assert(ptr5.use_count() == 2);
    assert(ptr6.use_count() == 2);

    gxj::shared_ptr<shape> ptr7{create_shape(shape_type::circle)};
    assert(ptr7.use_count() == 1);

    ptr6 = ptr7; // ptr7's count will ++, ptr5's count will --.
    assert(ptr5.use_count() == 1);
    assert(ptr7.use_count() == 2);

    return 0;
}