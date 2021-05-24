#ifndef REVERSE_ITER_H
#define REVERSE_ITER_H
#include <iterator>

// iterator adaptor
template <class T>
class Reverse_iter
{
    T* _current = nullptr;

public:

    typedef Reverse_iter Iter;
    typedef T iterator_type;
    typedef typename std::iterator_traits<T>::iterator_category iterator_category;
    typedef typename std::iterator_traits<T>::value_type value_type;
    typedef typename std::iterator_traits<T>::difference_type difference_type;
    typedef typename std::iterator_traits<T>::pointer pointer;
    typedef typename std::iterator_traits<T>::reference reference;

    explicit Reverse_iter( iterator_type x ) : _current(x) {}
    iterator_type base() const { return _current; }
    Iter& operator=( const Iter& other ) { _current = other.base(); return *this; } 
    reference operator*() const { Iter tmp = _current; return *--tmp; }
    pointer operator->() const { return _current - 1; }
    Iter& operator++() { --_current; return *this; }
    Iter& operator++( int ) { auto tmp = *this; _current--; return tmp; }
    Iter& operator--() { ++_current; return *this; }
    Iter& operator--( int ) { auto tmp = *this; _current++; return tmp; }
    bool operator==( const Iter& other ) { return _current == other.base(); }
    bool operator!=(const Iter& other ) { return _current != other.base(); }

};

#endif
