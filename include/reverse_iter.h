#ifndef REVERSE_ITER_H
#define REVERSE_ITER_H
#include <iterator>
#include "tree.h"

// need to be an adaptor
template <typename Key, typename T, class Comp, class Alloc>
class Reverse_iter
{
    Node<Key,T>* node;
    Tree<Key,T,Comp,Alloc>* tree;

public:

    typedef Reverse_iter Iter;
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef Node<Key,T> value_type;
    typedef std::ptrdiff_t difference_type;
    typedef Node<Key,T>* pointer;
    typedef Node<Key,T>& reference;

    Reverse_iter(Node<Key,T>* nd, Tree<Key,T,Comp,Alloc>& tr) : 
        node(nd), tree(&tr) {}
            
    bool operator==( Iter const& other ) const 
        { return (node == other.node); }
    bool operator!=( Iter const& other ) const 
        { return (node != other.node); }
    reference operator*() const { return *node; }
    pointer operator->() const { return node; };
    Iter operator++();
    Iter operator++( int ) { auto tmp = *this; ++*this; return tmp; }
    Iter operator--();
    Iter operator--( int ) { auto tmp = *this; --*this; return tmp; }

    // Additional 
    Key first() { return node == nullptr ? Key() : node->first; }
    T second() { return node == nullptr ? T() : node->second; }

    template <typename, typename, class, class> friend class Map;
};

template <typename Key, typename T, class Comp, class Alloc>
Reverse_iter<Key,T,Comp,Alloc> Reverse_iter<Key,T,Comp,Alloc>::operator--()
{
    Node<Key,T>* parent;

    if (node == nullptr)
    {
        return *this;

    } else if (node->right != nullptr)
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
Reverse_iter<Key,T,Comp,Alloc> Reverse_iter<Key,T,Comp,Alloc>::operator++()
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
