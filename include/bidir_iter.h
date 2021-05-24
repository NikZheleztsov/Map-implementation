#ifndef BIDIR_ITER_H
#define BIDIR_ITER_H
#include <iterator>
#include "tree.h"

template <typename Key, typename T, class Comp, class Alloc>
class Bidir_iter
{
    Node<Key,T>* node;
    Tree<Key,T,Comp,Alloc>* tree;

public:

    typedef Bidir_iter Iter;
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef Node<Key,T> value_type;
    typedef std::ptrdiff_t difference_type;
    typedef Node<Key,T>* pointer;
    typedef Node<Key,T>& reference;

    Bidir_iter(Node<Key,T>* nd, Tree<Key,T,Comp,Alloc>* tr) : node(nd), tree(tr) {}
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
    Key first() { return node->first; }
    T second () { return node->second; }
};

template <typename Key, typename T, class Comp, class Alloc>
Bidir_iter<Key,T,Comp,Alloc> Bidir_iter<Key,T,Comp,Alloc>::operator++()
{
    Node<Key,T>* parent;

    if (node == nullptr)
    {
        return *this;;

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
Bidir_iter<Key,T,Comp,Alloc> Bidir_iter<Key,T,Comp,Alloc>::operator--()
{
    Node<Key,T>* parent;

    if (node == nullptr) // if end()
    {
        // find max in tree
        node = tree->get_root(); 
        if (node == nullptr)
            return (node = nullptr); // if empty tree

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
