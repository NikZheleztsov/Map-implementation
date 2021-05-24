#ifndef MAP_H
#define MAP_H
#include <functional>
#include <memory>
#include "tree.h"
#include "bidir_iter.h"
#include "bidir_const_iter.h"
#include "reverse_iter.h"

#define Map_impl    Map<Key, T, Compare, Allocator>

template <
    class Key,
    class T,
    class Compare = std::greater<Key>,
    class Allocator = std::allocator<Node<Key,T> >
> class Map
{
    Tree<Key,T, Compare, Allocator>* tr = nullptr;
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
    typedef Reverse_iter<iterator>                      reverse_iterator;
    typedef Reverse_iter<const_iterator>                const_reverse_iterator;
    typedef Node<Key,T>                                 node_type;
    typedef std::allocator_traits<Allocator>            alloc_traits;

    Map() { tr = new Tree<Key,T,Compare,Allocator>; }
    iterator begin();
    iterator end();

    // temporary
    /*
    void insert (std::pair<Key, T> pair)
    {
        tr->push(pair);
    }
    */
};

template <class Key, class T, class Compare, class Allocator>
typename Map_impl::iterator Map_impl::begin()
{
    return iterator(tr->find_min(), tr);
}

template <class Key, class T, class Compare, class Allocator>
typename Map_impl::iterator Map_impl::end()
{
    return iterator(nullptr, tr);
}

#endif
