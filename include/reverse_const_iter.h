#ifndef REVERSE_CONST_ITER_H
#define REVERSE_CONST_ITER_H
#include <iterator>
#include "tree.h"

// need to be an adaptor
template <typename Key, typename T, class Comp, class Alloc>
class Reverse_const_iter
{
    const Node<Key,T>* node;
    const Tree<Key,T,Comp,Alloc>* const tree;

public:

    typedef const Reverse_const_iter Iter;
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef const Node<Key,T> value_type;
    typedef std::ptrdiff_t difference_type;
    typedef const Node<Key,T>* pointer;
    typedef const Node<Key,T>& reference;

    Reverse_const_iter(Node<Key,T>* nd, 
            const Tree<Key,T,Comp,Alloc>& tr) : node(nd), tree(&tr) {}

    bool operator==( const Iter& other ) const 
        { return (node == other.node); }
    bool operator!=( const Iter& other ) const 
        { return (node != other.node); }
    //////
    bool operator!=( std::nullptr_t ) const  // Maybe there's another way ? 
        { return (node != nullptr); }
    //////
    reference operator*() const { return *node; }
    pointer operator->() const { return node; };
    Iter operator++();
    Iter operator++( int ){ auto tmp = *this; ++*this; return tmp; }
    Iter operator--(); 
    Iter operator--( int ) { auto tmp = *this; --*this; return tmp; }

    // Additional 
    typedef Key k_type;
    typedef T v_type;
    k_type first() { return node == nullptr ? Key() : node->first; }
    v_type second() { return node == nullptr ? T() : node->second; }

    template <typename, typename, class, class> friend class Map;
};

template <typename Key, typename T, class Comp, class Alloc>
typename Reverse_const_iter<Key,T,Comp,Alloc>::Iter Reverse_const_iter<Key,T,Comp,Alloc>::operator--()
{
    Node<Key,T>* parent;

    if (node == nullptr)
        return *this;

    else if (node->right != nullptr)
    {
        node = node->right;
        while (node->left != nullptr)
            node = node->left;

    } else {

        parent = node->top;
        while (parent != nullptr && node == parent->right)
        {
            node = parent;
            parent = parent->top;
        }

        node = parent;
    }

    return *this;
}

template <typename Key, typename T, class Comp, class Alloc>
typename Reverse_const_iter<Key,T,Comp,Alloc>::Iter Reverse_const_iter<Key,T,Comp,Alloc>::operator++()
{
    Node<Key,T>* parent;

    if (node == nullptr) // if end()
    {
        // find max in tree
        node = tree->get_root(); 
        if (node == nullptr)
            return *this; // if empty tree

        while (node->right != nullptr)
            node = node->right;

    } else if (node->left != nullptr)
    {
        node = node->left;
        while (node->right != nullptr)
            node = node->right;

    } else {

        parent = node->top;
        while (parent != nullptr && node == parent->left)
        {
            node = parent;
            parent = parent->top;
        }

        node = parent;
    }

    return *this;
}

#endif
