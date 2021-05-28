#ifndef MAP_H
#define MAP_H
#include <functional>
#include <memory>
#include <limits>
#include <stdexcept>
#include "tree.h"
#include "bidir_iter.h"
#include "bidir_const_iter.h"
#include "reverse_iter.h"
#include "reverse_const_iter.h"

#define Map_impl    Map<Key, T, Compare, Allocator>

template <
    class Key,
    class T,
    class Compare = std::greater<Key>,
    class Allocator = std::allocator<Node<Key,T> >
> class Map
{
    Tree<Key,T, Compare, Allocator> tr = nullptr;
    Allocator alloc = Allocator();

public:
    typedef Key                                         key_type;
    typedef T                                           mapped_type;
    typedef std::pair<Key,T>                            value_type;
    typedef std::size_t                                 size_type;
    typedef std::ptrdiff_t                              difference_type;
    typedef Compare                                     key_compare;
    typedef Allocator                                   allocator_type;
    typedef value_type&                                 reference;           
    typedef const value_type&                           const_reference;
    typedef value_type*                                 pointer;
    typedef const value_type*                           const_pointer;
    typedef Bidir_iter<Key,T,Compare,Allocator>         iterator;
    typedef Bidir_const_iter<Key,T,Compare,Allocator>   const_iterator;     
    typedef Reverse_iter<Key,T,Compare,Allocator>       reverse_iterator; // need to be an adaptor
    typedef Reverse_const_iter<Key,T,Compare,Allocator> const_reverse_iterator; // adaptor
    typedef Node<Key,T>                                 node_type;
    typedef std::allocator_traits<Allocator>            alloc_traits;

    // Constructors & destructor 
    Map() : tr() {}
    template <class InputIt> Map (InputIt first, InputIt last);
    Map( const Map& other ) : tr(other.tr) {} 
    Map( Map&& other ) : tr(std::move(other.tr)) {}
    Map( std::initializer_list<value_type> init );
    ~Map() = default;

    // operator=
    Map& operator=(Map& other) { tr = other.tr; return *this; }

    // Element access
    T& at (const Key& key) { return tr.find(key)->second; }
    T& operator[]( const Key& key); //

    // Iterators
    iterator begin() { return iterator(tr.find_min(), tr); }
    const_iterator cbegin() const noexcept { return const_iterator(tr.find_min(), tr); }
    iterator end() { return iterator(nullptr, tr); }
    const_iterator cend() const noexcept { return const_iterator(nullptr, tr); }
    
    reverse_iterator rbegin() { return reverse_iterator(tr.find_max(), tr); }
    const_reverse_iterator crbegin() const { return const_reverse_iterator(tr.find_max(), tr); }
    reverse_iterator rend() { return reverse_iterator(nullptr, tr); }
    const_reverse_iterator crend() const noexcept {return const_reverse_iterator(nullptr, tr); }

    // Capacity
    bool empty() const { return !tr._size; }
    size_type size() const { return tr._size; }
    size_type max_size() const { return std::numeric_limits<difference_type>::max(); }

    // Modifiers
    void clear() { tr.root = tr.destruct_in(tr.root, nullptr); }

    std::pair<iterator, bool> insert (const value_type& value);
    template <class InputIt> void insert (InputIt first, InputIt last);
    void insert(std::initializer_list<value_type> ilist);

    iterator erase( iterator pos );
    iterator erase( iterator first, iterator last );
    size_type erase (const Key& key);

    void swap(Map& other);

    node_type extract (const_iterator position) 
        { return *tr.del_in(position.node, position.node->first, position.node->top); }
    node_type extract (const key_type& x)
        { return *tr.del(x); }

    // with reallocations, unfortunately
    template<class C2> void merge( Map<Key,T,C2,Allocator>& source );

    // Lookup
    size_type count(const Key& key) const { return (tr.find(key) == nullptr ? 0 : 1); }
    iterator find(const Key& key) { return iterator(tr.find(key), tr); }
    const_iterator find( const Key& key) const { return const_iterator(tr.find(key), tr); }
};

template <class Key, class T, class Compare, class Allocator> template <class InputIt> 
Map<Key,T,Compare,Allocator>::Map (InputIt first, InputIt last) : tr()
{
    auto dist = std::distance(first, last);
    if (dist < 0 || dist > max_size())
        throw std::invalid_argument("Map::Map(...) - first It > last It");

    for (; first != last; first++)
        tr->push (std::make_pair(first.first(), first.second()));
}

template <class Key, class T, class Compare, class Allocator> 
Map<Key,T,Compare,Allocator>::Map( std::initializer_list<Map::value_type> init ) : tr()
{
    for (auto it = init.begin(); it < init.end(); it++)
        tr.push(*it);
}

template <class Key, class T, class Compare, class Allocator>
typename Map_impl::mapped_type& Map_impl::operator[] (const Key& key)
{
    auto find = tr.find(key);
    if (find == nullptr)
        find = tr.push(std::make_pair(key, T()));

    return find->second;
}

template <class Key, class T, class Compare, class Allocator>
std::pair<typename Map_impl::iterator, bool> Map_impl::insert (const value_type& value)
{
    auto ret = tr.push(value);
    return std::make_pair(iterator(ret, tr), 
            (ret->second == value.second && ret->first == value.first)); 
}

template <class Key, class T, class Compare, class Allocator> template <class InputIt> 
void Map_impl::insert (InputIt first, InputIt last)
{
    auto dist = std::distance(first, last);
    if (dist < 0 || dist > max_size())
        throw std::invalid_argument("Map::insert - first It > last It");

    for (; first != last; first++)
        tr.push (std::make_pair(first.first(), first.second()));
}

template <class Key, class T, class Compare, class Allocator> 
void Map_impl::insert(std::initializer_list<value_type> ilist)
{
    for (auto it = ilist.begin(); it < ilist.end(); it++)
        tr.push(*it);
}

template <class Key, class T, class Compare, class Allocator> 
typename Map_impl::iterator Map_impl::erase( iterator pos )
{
    auto tmp = pos; tmp++;
    Node<Key,T>* nd = tr.find(pos.node->first);
    if (nd == pos.node) // comparison of addresses
        tr.del_in(pos.node, pos.node->first, pos.node->top);

    return tmp;
}

template <class Key, class T, class Compare, class Allocator> 
typename Map_impl::iterator Map_impl::erase( iterator first, iterator last )
{
    auto tmp = last; tmp++;
    auto dist = std::distance(first, last);
    if (dist < 0 || dist > max_size())
        throw std::invalid_argument("Map::erase - first It > last It");

    if (tr.find(first.node->first) == first.node &&
            (tr.find(last.node->first) == last.node || last.node == nullptr))

        for (; first != last; first++)
            tr.del_in(first.node, first.node->first, first.node->top);
    
    return tmp;
}

template <class Key, class T, class Compare, class Allocator> 
typename Map_impl::size_type Map_impl::erase (const Key& key)
{
    auto nd = tr.del(key);
    if (nd != nullptr) 
    {
        std::allocator_traits<Allocator>::deallocate(alloc, nd, 1);
        return 1;
    }

    return 0;
}

template <class Key, class T, class Compare, class Allocator> 
void Map_impl::swap(Map& other)
{
    auto tmp(std::move(other));
    other = *this;
    *this = tmp;
}

// not right
template <class Key, class T, class Compare, class Allocator> 
template<class C2> void Map_impl::merge( Map<Key,T,C2,Allocator>& source )
{
    for (auto x : source)
        if (find(x.first).node == nullptr)
        {
            insert(std::make_pair(x.first, x.second));
            source.erase(x.first);
        }
}

#endif
