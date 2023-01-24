// вставьте сюда ваш код для класса SimpleVector
// внесите необходимые изменения для поддержки move-семантики
#pragma once

#include <cassert>
#include <initializer_list>
#include <array>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <algorithm>


using namespace std;
#include "array_ptr.h"


class ReserveProxyObj {
public:
    ReserveProxyObj(size_t capacity_to_reserve)
            : capacity_reserve_(capacity_to_reserve) {
    }

    size_t GetCapacity() {
        return capacity_reserve_;
    }

private:
    size_t capacity_reserve_ = 0;
};
template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size)
    : SimpleVector(size, std::move(Type()))
    {
    }

    SimpleVector(ReserveProxyObj obj) {
        Reserve(obj.GetCapacity());
    }

    SimpleVector(size_t size, Type &&value)
    : items_(size),
    size_(size),
    capacity_(size)
    {
        for (size_t i = 0; i < size_; ++i)
        {
            items_[i] = std::move(value);
        }
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value)
    :items_(size)
    {
        size_ = size;
        capacity_ = size;
        std::fill(begin(),end(),value);
    }

    SimpleVector(const SimpleVector& other) {
        assert(size_ == 0);
        SimpleVector<Type> copy(other.GetSize());
        std::copy((other.items_).Get(), ((other.items_).Get() + other.GetSize()), (copy.items_).Get());
        copy.capacity_ = other.capacity_;
        swap(copy);
    }

    SimpleVector(SimpleVector&& other) {
        assert(size_ == 0);
        items_ = std::move(other.items_);
        size_ = std::exchange(other.size_, 0);
        capacity_ = std::exchange(other.capacity_, 0);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init)
            :items_(init.size())
    {
        size_  =  init.size() ;
        capacity_  = init.size();
        std::copy(init.begin(),init.end(),begin());
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if(index >= size_){
            throw std::out_of_range("Out of range");
        }
        return  items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if(index >= size_){
            throw std::out_of_range("Out of range");
        }
        return  items_[index];
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_ = 0;
    }

    // Изменяет размер массива.


    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size){
        if (new_size < size_){
            size_ = new_size;
            return;
        }
        if (new_size < capacity_){
            for (auto i = size_; i < new_size; ++i)
            {
                items_[i] = std::move(Type());
            }
            size_ = new_size;
            return;
        } else {
            size_t curr_size = std::max(new_size, capacity_ * 2);
            Array_Ptr<Type> new_items(curr_size);

            std::copy(std::make_move_iterator(begin()), std::make_move_iterator(end()), new_items.Get());
            for (size_t i = size_; i < new_size; ++i) {
                new_items[i] = std::move(Type());
            }

            items_.swap(new_items);
            capacity_ = curr_size;
            size_ = new_size;
        }
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return &items_[0];
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return &items_[size_];
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return &items_[0];

    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return &items_[size_];

    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        return &items_[0];
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        return &items_[size_];
    }

    SimpleVector& operator=( SimpleVector&& rhs) {
        if (this != &rhs) {
            if (rhs.IsEmpty()) {
                Clear();
            }
            else {
                items_ = std::move(rhs.items_);
                std::swap(size_, rhs.size_);
                std::swap(capacity_, rhs.capacity_);
            }
        }
        return *this;
    }
    SimpleVector& operator=(const SimpleVector& rhs) {
        if (this != &rhs) {
            if (rhs.IsEmpty()) {
                Clear();
            }
            else {
                SimpleVector<Type> copy_vector(rhs);
                swap(copy_vector);
            }
        }
        return *this;
    }

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item) {
        this->Resize(size_ + 1);
        items_[size_ - 1] = std::move(item);
    }
    void PushBack(Type&& item) {
        this->Resize(size_ + 1);
        items_[size_ - 1] = std::move(item);
    }

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type &value)
    {
        assert(begin() <= pos && pos <= end());

        auto n = std::distance(begin(), Iterator(pos));
        if (capacity_ == 0)
        {
            PushBack(value);
            return begin();
        }
        if (size_ < capacity_)
        {
            for (int i = size_; i > n; --i)
            {
                items_[i] = std::move(items_[i - 1]);
            }
            items_[n] = value;
            ++size_;
            return begin() + n;
        }
        else
        {
            SimpleVector<Type> new_vec(capacity_ * 2);

            std::copy(begin(), begin() + n, new_vec.begin());
            std::copy(begin() + n, end(), new_vec.begin() + n + 1);
            *(new_vec.begin() + n) = value;
            auto old_size = size_;
            swap(new_vec);
            size_ = old_size + 1;
            return begin() + n;
        }
    }
    
    Iterator Insert(ConstIterator pos, Type &&value)
    {
        assert(begin() <= pos && pos <= end());

        auto n = std::distance(begin(), Iterator(pos));
        if (capacity_ == 0)
        {
            PushBack(std::move(value));
            return begin();
        }
        if (size_ < capacity_)
        {
            for (int i = size_; i > n; --i)
            {
                items_[i] = std::move(items_[i - 1]);
            }
            items_[n] = std::move(value);

            ++size_;
            return begin() + n;
        }
        else
        {
            SimpleVector<Type> new_vec(capacity_ * 2);

            std::copy(std::make_move_iterator(begin()), std::make_move_iterator(begin() + n), new_vec.begin());
            std::copy(std::make_move_iterator(begin() + n), std::make_move_iterator(end()), new_vec.begin() + n + 1);
            *(new_vec.begin() + n) = std::move(value);
            auto old_size = size_;
            swap(new_vec);
            size_ = old_size + 1;
            return begin() + n;
        }
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity <= capacity_) {
            return;
        }

        Array_Ptr<Type> tmp_items(new_capacity);
        std::copy(std::make_move_iterator(begin()), std::make_move_iterator(end()), tmp_items.Get());
        items_.swap(tmp_items);
        capacity_ = new_capacity;
    }
    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        if ( IsEmpty() ){
            return;
        }
        size_--;
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        auto distance_  = std::distance(cbegin(), pos);
        auto* it = begin() + distance_;
        std::move((it + 1), end(), it);
        size_--;
        return (begin() + distance_);
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        items_.swap(other.items_);
        std::swap(other.size_,this->size_ );
        std::swap(other.capacity_,this->capacity_ );
    }

private:
    Array_Ptr<Type> items_;
    size_t size_ = 0;
    size_t capacity_ = 0;
};




template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return (lhs.GetSize() == rhs.GetSize()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());

}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs ==  rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return (lhs == rhs) || (lhs < rhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return lhs < rhs;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return (lhs == rhs) || (lhs < rhs);
}
ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}
