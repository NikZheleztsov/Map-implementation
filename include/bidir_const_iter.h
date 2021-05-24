#ifndef BIDIR_CONST_ITER_H
#define BIDIR_CONST_ITER_H
#include <iterator>
#include "tree.h"

template <typename Key, typename T, class Comp, class Alloc>
class Bidir_const_iter
{
    const Node<Key,T>* node;
    const Tree<Key,T,Comp,Alloc>* tree;

public:

    typedef const Bidir_const_iter Iter;
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef const Node<Key,T> value_type;
    typedef std::ptrdiff_t difference_type;
    typedef const Node<Key,T>* pointer;
    typedef const Node<Key,T>& reference;

    Bidir_const_iter(Node<Key,T>* nd, Tree<Key,T,Comp,Alloc>* tr) : node(nd), tree(tr) {}
    bool operator==( Iter const& other ) const 
        { return (node == other.node); }
    bool operator!=( Iter const& other ) const 
        { return (node != other.node); }
    reference operator*() const { return *node; }
    pointer operator->() const { return node; };
    Iter operator++() const;
    Iter operator++( int ) const { auto tmp = *this; ++*this; return tmp; }
    Iter operator--() const;
    Iter operator--( int ) const { auto tmp = *this; --*this; return tmp; }

    // Additional
    Key first() { return node->first; }
    T second () { return node->second; }
};

template <typename Key, typename T, class Comp, class Alloc>
typename Bidir_const_iter<Key,T,Comp,Alloc>::Iter Bidir_const_iter<Key,T,Comp,Alloc>::operator++() const
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
typename Bidir_const_iter<Key,T,Comp,Alloc>::Iter Bidir_const_iter<Key,T,Comp,Alloc>::operator--() const
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
