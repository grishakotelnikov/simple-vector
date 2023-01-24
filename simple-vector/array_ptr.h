// вставьте сюда ваш код для класса ArrayPtr
// внесите в него изменения, 
// которые позволят реализовать move-семантику
#pragma once

#include <cstdlib>
#include <utility>
template <typename T>
class Array_Ptr{
public:
    Array_Ptr() = default;

    explicit Array_Ptr(size_t size)
    {
        if (size == 0) {
            raw_ptr_ = nullptr;
        }else {
            raw_ptr_ = new T[size];
        }
    }



    explicit Array_Ptr(T* raw_ptr) noexcept {
        raw_ptr_ = raw_ptr;
    }
    Array_Ptr(Array_Ptr&& other) {
        raw_ptr_ = std::exchange(other.raw_ptr_, nullptr);
    }

    Array_Ptr(const Array_Ptr&) = delete;

    ~Array_Ptr(){
        delete[] raw_ptr_;
        raw_ptr_ = nullptr;
    }
    Array_Ptr& operator=(const Array_Ptr&) = delete;

    Array_Ptr& operator=(Array_Ptr&& other) {
        raw_ptr_ = std::exchange(other.raw_ptr_, nullptr);
        return *this;
    }

    T* Get() const noexcept {
        return raw_ptr_;
    }

    T& operator[](size_t index) noexcept{
        return raw_ptr_[index];
    }
    const T& operator[](size_t index) const noexcept{
        return raw_ptr_[index];
    }
    void swap(Array_Ptr& other) noexcept {
        auto* ptr = raw_ptr_;
        raw_ptr_ = other.raw_ptr_;
        other.raw_ptr_ = ptr;
    }
private:
    T* raw_ptr_ = nullptr;
};
