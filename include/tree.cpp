#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <iomanip>
#include "tree.h"

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

template <typename Key, typename T, class Comp, class Allocator>
Node<Key,T>* Tree<Key,T,Comp,Allocator>::push_in(Tree<Key,T,Comp,Allocator>& tr, Node<Key, T>* nd, T data, Key key, Node<Key, T>* top)
{
    if (nd == nullptr)
    {
        /*
        nd = new Node<Key,T>;
        nd->data = data;
        nd->key = key;
        nd->left = nullptr;
        nd->right = nullptr;
        nd->height = 1;
        nd->top = top;
        nd->dif = 0;
        */

        nd = alloc_traits::allocate(alloc, 1);
        alloc_traits::construct(alloc, nd, key, data, top, nullptr, nullptr, 0, 1);

        if (tr.root == nullptr)
            tr.root = nd;

        return nd;

    } else if (nd->key == key)
    {
        std::cout << "Similar keys are forbidden!\n";
        return nd;

    } else if (Comp{}(key, nd->key))
    {
        nd->right = push_in(tr, nd->right, data, key, nd);
        nd = balance(nd, top);
        return nd;
    } else 
    {
        nd->left = push_in(tr, nd->left, data, key, nd);
        nd = balance(nd, top);
        return nd;
    }
}

template <typename Key, typename T, class Comp, class Allocator>
Node<Key,T>* Tree<Key,T,Comp,Allocator>::l_max (Node<Key, T>* nd)
{
    Node<Key,T>* max = nd;

    do {

        if (Comp{}(nd->key, max->key))
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
void Tree<Key,T,Comp,Allocator>::del_in(Tree<Key,T,Comp,Allocator>& tr, Node<Key, T>* nd, Key key, Node<Key, T>* top)
{
    if (nd->data.first == key)
    {
        if (is_leaf(nd))
        {
            if (top)
            {
                if (top->right == nd)
                    top->right = nullptr;
                else 
                    top->left = nullptr;
            } else 
                tr.root = nullptr;

            // delete nd;
            alloc_traits::deallocate(alloc, nd, 1);

        } else {

            Node<Key,T>* for_ch;
            if (nd->left)
                for_ch = l_max(nd->left);
            else 
                for_ch = nd->right;

            Node<Key,T>* bal = for_ch->top;

            nd->data.second = for_ch->data;
            nd->data.first = for_ch->key;

            if (is_leaf(for_ch))
            {
                if (bal)
                {
                    if (bal->right == for_ch)
                        bal->right = nullptr;
                    else 
                        bal->left = nullptr;
                }

                // delete for_ch;
                alloc_traits::deallocate(alloc, for_ch, 1);

            } else {

                for_ch->left->top = bal;
                if (bal->right == for_ch)
                    bal->right = for_ch->left;
                else 
                    bal->left = for_ch->left;
                // delete for_ch;
                alloc_traits::deallocate(alloc, for_ch, 1);
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
        }

    } else if (key > nd->data.first && nd->right != nullptr)
    {
        del_in(tr, nd->right, key, nd);
        nd = balance(nd, top);

    } else if (key < nd->data.first && nd->left != nullptr)
    {
        del_in(tr, nd->left, key, nd);
        nd = balance(nd, top);
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
        // delete nd;
        alloc_traits::deallocate(alloc, nd, 1);

    if (nd->right != nullptr)
        nd->right = destruct_in(nd->right, nd);
    if (nd->left != nullptr)
        nd->left = destruct_in(nd->left, nd);

    return nullptr;
}

template <typename Key, typename T, class Comp, class Allocator>
Node<Key,T>* Tree<Key,T,Comp,Allocator>::search_in (Key key, Node<Key,T>* nd)
{
    if (nd->data.first == key)
        return nd;

    else if (Comp{}(key, nd->data.first) && nd->right != nullptr)
    {
        search_in(key, nd->right);

    } else if (!Comp{}(key,nd->data.first) && nd->left != nullptr)
    {
        search_in(key, nd->left);
    }
}

template <typename Key, typename T, class Comp, class Allocator>
Tree<Key,T,Comp,Allocator>::Tree(Key k, T data)
{
    push_in(*this, root, data, k, nullptr);
}

template <typename Key, typename T, class Comp, class Allocator>
Tree<Key,T,Comp,Allocator>::Tree(Tree<Key,T,Comp,Allocator>&& other)
{
    root = other.root;
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
void Tree<Key,T,Comp,Allocator>::push (Key key, T data)
{
    push_in(*this, root, data, key, nullptr);
}

template <typename Key, typename T, class Comp, class Allocator>
void Tree<Key,T,Comp,Allocator>::del(Key key)
{
    if (root != nullptr)
    {
        Node<Key,T>* null = nullptr;
        del_in(*this, root, key, null);

    } else 
        std::cout << "Empty Tree\n";
}

template <typename Key, typename T, class Comp, class Allocator>
Node<Key,T>* Tree<Key,T,Comp,Allocator>::search (Key key)
{
    return search_in (key, root);
}

template <typename Key, typename T, class Comp, class Allocator>
Node<Key,T>* Tree<Key,T,Comp,Allocator>::find_min(Key key)
{
    Node<Key,T>* tmp = root;
    Node<Key,T>* min = tmp;

    do {

        if (Comp{}(tmp->data.first, min->data.first))
            min = tmp;
        tmp = tmp->left;

    } while (tmp != nullptr);

    return min;
}

template <typename Key, typename T, class Comp, class Allocator>
Node<Key,T>* Tree<Key,T,Comp,Allocator>::find_max(Key key)
{
    Node<Key,T>* tmp = root;
    Node<Key,T>* max = tmp;

    do {

        if (Comp{}(tmp->data.first, max->data.first))
            max = tmp;
        tmp = tmp->right;

    } while (tmp != nullptr);

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

void temporary()
{
    Tree<int, int> tr;
}
