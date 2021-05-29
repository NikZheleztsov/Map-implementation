#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <iomanip>
#include "tree.h"
#include "bidir_const_iter.h"

// really slow one 
template <typename Key, typename T, class Comp, class Allocator>
Tree<Key,T,Comp,Allocator>::Tree(const Tree<Key,T,Comp,Allocator>& other)
{
    if (other.root != nullptr)
    {
        const_iterator beg (other.find_min(), *this);
        while (beg != nullptr)
        {
            push(std::make_pair(beg.first(), beg.second()));
            beg++;
        }

        _size = other._size;
    }
}

template <typename Key, typename T, class Comp, class Allocator>
Tree<Key,T,Comp,Allocator>& Tree<Key,T,Comp,Allocator>::operator=( Tree<Key,T,Comp,Allocator>& other )
{
    this->~Tree();

    if (other.root != nullptr)
    {
        const_iterator beg (other.find_min(), *this);
        while (beg != nullptr)
        {
            push(std::make_pair(beg.first(), beg.second()));
            beg++;
        }

        _size = other._size;
    }
    
    return *this;
}

// calculating height based on bottom Nodes
template <typename Key, typename T, class Comp, class Allocator>
void Tree<Key,T,Comp,Allocator>::height (Node<Key, T>* nd) 
{
    if (nd)
    {
        if (nd->right != nullptr && nd->left != nullptr)
        {
            nd->height = std::max(nd->right->height, nd->left->height) + 1;
            nd->dif = nd->right->height - nd->left->height;

        } else if (nd->right == nullptr && nd->left != nullptr)
        {
            nd->height = nd->left->height + 1;
            nd->dif = 0 - nd->left->height;

        } else if (nd->right != nullptr && nd->left == nullptr)
        {
            nd->height = nd->right->height + 1;
            nd->dif = nd->right->height;

        } else if (nd->right == nullptr && nd->left == nullptr)
        {
            nd->height = 1;
            nd->dif = 0;
        }
    }
}

template <typename Key, typename T, class Comp, class Allocator>
Node<Key, T>* Tree<Key,T,Comp,Allocator>::small_l_rot (Node<Key, T>* nd) 
{
    Node<Key, T>* temp = nd->right;

    nd->right = temp->left;
    if (temp->left)
        temp->left->top = nd;

    temp->left = nd;
    nd->top = temp;
    nd = temp;

    return nd;
} 

template <typename Key, typename T, class Comp, class Allocator>
Node<Key, T>* Tree<Key,T,Comp,Allocator>::small_r_rot (Node<Key, T>* nd)
{
    Node<Key, T>* temp = nd->left;

    nd->left = temp->right;
    if (temp->right)
        temp->right->top = nd;

    temp->right = nd;
    nd->top = temp;
    nd = temp;

    return nd;
}

template <typename Key, typename T, class Comp, class Allocator>
Node<Key, T>* Tree<Key,T,Comp,Allocator>::big_l_rot (Node<Key, T>* nd)
{
    nd->right = small_r_rot(nd->right);
    nd = small_l_rot(nd);

    return nd;
}

template <typename Key, typename T, class Comp, class Allocator>
Node<Key,T>* Tree<Key,T,Comp,Allocator>::big_r_rot(Node<Key, T>* nd)
{
    nd->left = small_l_rot(nd->left);
    nd = small_r_rot(nd);

    return nd;
}

template <typename Key, typename T, class Comp, class Allocator>
Node<Key,T>* Tree<Key,T,Comp,Allocator>::balance (Node<Key, T>* nd, Node<Key,T>* top)
{
    if (nd)
    {
        height(nd);

        bool root_flag = 0;
        if (nd == root)
            root_flag = 1;

        if (nd->dif == 2 && ((nd->right == nullptr ? 1 : (nd->right->dif == 0 ? 1 : 0))
                    || (nd->right == nullptr ? 0 : (nd->right->dif == 1 ? 1 : 0))))
        {
            nd = small_l_rot(nd);
            nd->top = top;
            height(nd->left);
            height(nd);

            if (root_flag)
                root = nd;

        } 

        if (nd->right != nullptr)
        {
            if (nd->dif == 2 && nd->right->dif < 0 )
            {
                nd = big_l_rot(nd);
                nd->top = top;
                height(nd->left);
                height(nd->right);
                height(nd);

                if (root_flag)
                    root = nd;
            }
        }

        if (nd->dif == -2 && ((nd->left == nullptr ? 1 : (nd->left->dif == 0 ? 1 : 0))
                    || (nd->left == nullptr ? 0 : (nd->left->dif  == -1 ? 1 : 0))))
        {
            nd = small_r_rot(nd);
            nd->top = top;
            height(nd->right);
            height(nd);

            if (root_flag)
                root = nd;

        } 

        if (nd->left != nullptr) 
        {
            if (nd->dif == -2 && nd->left->dif > 0 )
            {
                nd = big_r_rot(nd);
                nd->top = top;
                height(nd->left);
                height(nd->right);
                height(nd);

                if (root_flag)
                    root = nd;
            }
        }
    }

    return nd;
}

// node_r for no reallocation 
template <typename Key, typename T, class Comp, class Allocator>
std::pair<Node<Key,T>*, Node<Key,T>*> Tree<Key,T,Comp,Allocator>::push_in(Node<Key, T>* nd, const std::pair<Key,T>& pair, Node<Key, T>* top)
{
    if (nd == nullptr)
    {
        nd = alloc_traits::allocate(alloc, 1);
        alloc_traits::construct(alloc, nd, pair, top, nullptr, nullptr, 0, 1);
        // don't know how to implement move semantics here

        _size++;
        if (root == nullptr)
            root = nd;

        return std::make_pair(nd,nd);

    } else if (nd->first == pair.first)
    {
        // similar keys
        return std::make_pair(nd,nd);

    } else if (Comp{}(pair.first, nd->first))
    {
        auto ret = push_in(nd->right, pair, nd);
        nd->right = ret.first;
        nd = balance(nd, top);
        return std::make_pair(nd, ret.second);

    } else 
    {
        auto ret = push_in(nd->left, pair, nd);
        nd->left = ret.first;
        nd = balance(nd, top);
        return std::make_pair(nd, ret.second);;
    }
}

template <typename Key, typename T, class Comp, class Allocator>
Node<Key,T>* Tree<Key,T,Comp,Allocator>::l_max (Node<Key, T>* nd)
{
    Node<Key,T>* max = nd;

    do {

        if (Comp{}(nd->first, max->first))
            max = nd;
        nd = nd->right;

    } while (nd != nullptr);

    return max;
}

template <typename Key, typename T, class Comp, class Allocator>
bool Tree<Key,T,Comp,Allocator>::is_leaf (Node<Key,T>* nd)
{
    return (nd->left == nullptr && nd->right == nullptr);
}

template <typename Key, typename T, class Comp, class Allocator>
Node<Key,T>* Tree<Key,T,Comp,Allocator>::del_in(Node<Key, T>* nd, Key key, Node<Key, T>* top)
{
    if (nd->first == key)
    {
        _size--;
        if (is_leaf(nd))
        {
            if (top)
            {
                if (top->right == nd)
                    top->right = nullptr;
                else 
                    top->left = nullptr;
            } else 
                root = nullptr;

            // alloc_traits::deallocate(alloc, nd, 1);
            return nd;

        } else {

            Node<Key,T>* for_ch;
            if (nd->left)
                for_ch = l_max(nd->left);
            else 
                for_ch = nd->right;

            Node<Key,T>* bal = for_ch->top;

            nd->second = for_ch->second;
            nd->first = for_ch->first;

            if (is_leaf(for_ch))
            {
                if (bal)
                {
                    if (bal->right == for_ch)
                        bal->right = nullptr;
                    else 
                        bal->left = nullptr;
                }

                // alloc_traits::deallocate(alloc, for_ch, 1);

            } else {

                for_ch->left->top = bal;
                if (bal->right == for_ch)
                    bal->right = for_ch->left;
                else 
                    bal->left = for_ch->left;

                // alloc_traits::deallocate(alloc, for_ch, 1);
            }

            //balancing from for_ch
            if (bal)
            {
                while (bal != nullptr)
                {
                    if (bal->top) 
                    {
                        if (bal == nd->top)
                            break;

                        if (bal->top->right == bal)
                            bal->top->right = balance(bal, bal->top);
                        else 
                            bal->top->left = balance(bal, bal->top);

                    } else {
                        Node<Key,T>* null = nullptr;
                        balance(bal, null);
                        break;
                    }

                    bal = bal->top;
                }
            }

            return for_ch;
        }

    } else if (key > nd->first && nd->right != nullptr)
    {
        auto tmp = del_in(nd->right, key, nd);
        nd = balance(nd, top);
        return tmp;

    } else if (key < nd->first && nd->left != nullptr)
    {
        auto tmp = del_in(nd->left, key, nd);
        nd = balance(nd, top);
        return tmp;

    } else {
        // extract has to return empty node
        Node<Key,T>* ret = alloc_traits::allocate(alloc, 1);
        alloc_traits::construct(alloc, ret);
        return ret;
    }
}

template <typename Key, typename T, class Comp, class Allocator>
bool Tree<Key,T,Comp,Allocator>::is_number (char num)
{
    try  {
        std::stoi(static_cast<std::string>(&num));
        return 1;
    } catch (...) {
        return (num == '\\' || num == '/');
    }
}

template <typename Key, typename T, class Comp, class Allocator>
Node<Key,T>* Tree<Key,T,Comp,Allocator>::destruct_in (Node<Key, T>* nd, Node<Key, T>* top)
{
    if (is_leaf(nd))
        alloc_traits::deallocate(alloc, nd, 1);

    if (nd->right != nullptr)
        nd->right = destruct_in(nd->right, nd);
    if (nd->left != nullptr)
        nd->left = destruct_in(nd->left, nd);

    _size = 0;
    return nullptr;
}

template <typename Key, typename T, class Comp, class Allocator>
Node<Key,T>* Tree<Key,T,Comp,Allocator>::find_in (Key key, Node<Key,T>* nd)
{
    if (nd != nullptr)
    {
        if (nd->first == key)
            return nd;

        else if (Comp{}(key, nd->first) && nd->right != nullptr)
            return find_in(key, nd->right);
        else if (!Comp{}(key,nd->first) && nd->left != nullptr)
            return find_in(key, nd->left);
        else 
            return nullptr;
    } 
}

template <typename Key, typename T, class Comp, class Allocator>
Tree<Key,T,Comp,Allocator>::Tree(std::pair<Key,T> pair)
{
    push_in(root, pair, nullptr);
    _size = 1;
}

template <typename Key, typename T, class Comp, class Allocator>
Tree<Key,T,Comp,Allocator>::Tree(Tree<Key,T,Comp,Allocator>&& other)
{
    root = other.root;
    _size = other._size;
    other.root = nullptr;
}

template <typename Key, typename T, class Comp, class Allocator>
bool Tree<Key,T,Comp,Allocator>::operator==( Tree<Key,T,Comp,Allocator>& other )
{
    return root == other.root;
}

template <typename Key, typename T, class Comp, class Allocator>
bool Tree<Key,T,Comp,Allocator>::operator!=( Tree<Key,T,Comp,Allocator>& other ) 
{
    return root != other.root;
}

template <typename Key, typename T, class Comp, class Allocator>
Node<Key,T>* Tree<Key,T,Comp,Allocator>::push (const std::pair<Key,T>& pair)
{
    return push_in(root, pair, nullptr).second;
}

template <typename Key, typename T, class Comp, class Allocator>
Node<Key,T>* Tree<Key,T,Comp,Allocator>::del(Key key)
{
    Node<Key,T>* null = nullptr;
    return del_in(root, key, null);
}

template <typename Key, typename T, class Comp, class Allocator>
Node<Key,T>* Tree<Key,T,Comp,Allocator>::find (const Key& key)
{
    return find_in (key, root);
}

template <typename Key, typename T, class Comp, class Allocator>
Node<Key,T>* Tree<Key,T,Comp,Allocator>::find_min() const
{
    Node<Key,T>* tmp = root;
    Node<Key,T>* min = tmp;

    if (min != nullptr)
    {
        do {

            if (!Comp{}(tmp->first, min->first))
                min = tmp;
            tmp = tmp->left;

        } while (tmp != nullptr);
    }

    return min;
}

template <typename Key, typename T, class Comp, class Allocator>
Node<Key,T>* Tree<Key,T,Comp,Allocator>::find_max() const
{
    Node<Key,T>* tmp = root;
    Node<Key,T>* max = tmp;

    if (max != nullptr)
    {
        do {

            if (Comp{}(tmp->first, max->first))
                max = tmp;
            tmp = tmp->right;

        } while (tmp != nullptr);
    }

    return max;
}

template <typename Key, typename T, class Comp, class Allocator>
Tree<Key,T,Comp,Allocator>::~Tree()
{
    if (root != nullptr)
    {
        Node<Key,T>* null = nullptr;
        root = destruct_in(root, null);
    }
}

/*
template class Bidir_const_iter
    <int, std::string, std::greater<int>, std::allocator<Node<int, std::string>>>;
    */
template struct Node<int, std::string>;
template class Tree
    <int, std::string, std::greater<int>, std::allocator<Node<int, std::string>>>;
