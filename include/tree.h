#ifndef TREE_H
#define TREE_H
#include <vector>
#include <functional>
#include <utility>
#include <string>

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <iomanip>

template <typename, typename, class, class>
class Bidir_const_iter;

template <typename Key, typename T>
struct Node
{
    Key first;
    T second;

    Node(std::pair<Key,T> pair, Node* Top, Node* Left, 
            Node* Right, int Dif, uint64_t h) : 
        first(pair.first), second(pair.second),
        top(Top), left(Left), right(Right),
        dif(Dif), height(h) {}

    private:
    Node* top;
    Node* left;
    Node* right;
    int dif; // right - left
    uint64_t height;

    template <typename, typename, class, class> friend class Tree;
    template <typename, typename, class, class> friend class Bidir_iter;
    template <typename, typename, class, class> friend class Bidir_const_iter;
};

/*
        nd->data = data;
        nd->key = key;
        nd->left = nullptr;
        nd->right = nullptr;
        nd->height = 1;
        nd->top = top;
        nd->dif = 0;
        */


template < 
         typename Key, 
         typename T,
         class Comp = std::greater<Key>,
         class Allocator = std::allocator<Node<Key, T> >
> class Tree
{
    Node<Key,T>* root;
    Allocator alloc = Allocator();

    bool is_leaf (Node<Key,T>* nd);
    bool is_number (char num);
    Node<Key,T>* l_max (Node<Key, T>* nd);
    void height (Node<Key, T>* nd);

    Node<Key,T>* small_l_rot (Node<Key, T>* nd);
    Node<Key,T>* small_r_rot (Node<Key, T>* nd);
    Node<Key,T>* big_l_rot (Node<Key, T>* nd);
    Node<Key,T>* big_r_rot(Node<Key, T>* nd);

    Node<Key,T>* balance (Node<Key, T>* nd, Node<Key,T>* top);
    Node<Key,T>* push_in(Node<Key, T>* nd, std::pair<Key,T> pair, Node<Key, T>* top);
    Node<Key,T>* search_in (Key key, Node<Key,T>* nd);
    void del_in(Node<Key, T>* nd, Key key, Node<Key, T>* top);
    Node<Key,T>* destruct_in (Node<Key, T>* nd, Node<Key, T>* top);

public:

    typedef typename std::allocator_traits<Allocator>   alloc_traits;
    typedef Bidir_const_iter<Key,T,Comp,Allocator>      const_iterator; 
    typedef Node<Key,T>*                                pointer;

    Tree() : root(nullptr) {}
    Tree( Tree<Key,T,Comp,Allocator>& other);
    Tree(std::pair<Key,T> pair);
    Tree(Tree<Key,T,Comp,Allocator>&& other);

    void push (std::pair<Key,T>& pair);
    void del(Key key);
    Node<Key,T>* search (Key key);
    Node<Key,T>* find_max();
    Node<Key,T>* find_min();
    Node<Key,T>* get_root() { return root; };

    bool operator==( Tree<Key,T,Comp,Allocator>& other );
    bool operator!=( Tree<Key,T,Comp,Allocator>& other );
    ~Tree();

};

// really slow one 
template <typename Key, typename T, class Comp, class Allocator>
Tree<Key,T,Comp,Allocator>::Tree(Tree<Key,T,Comp,Allocator>& other)
{
    const_iterator beg (find_max(), this);
    while (beg != nullptr)
    {
        push(std::make_pair(beg.first(), beg.second()));
        beg++;
    }
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

template <typename Key, typename T, class Comp, class Allocator>
Node<Key,T>* Tree<Key,T,Comp,Allocator>::push_in(Node<Key, T>* nd, std::pair<Key,T> pair, Node<Key, T>* top)
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
        alloc_traits::construct(alloc, nd, pair, top, nullptr, nullptr, 0, 1);

        if (root == nullptr)
            root = nd;

        return nd;

    } else if (nd->first == pair.first)
    {
        std::cout << "Similar keys are forbidden!\n";
        return nd;

    } else if (Comp{}(pair.first, nd->first))
    {
        nd->right = push_in(nd->right, pair, nd);
        nd = balance(nd, top);
        return nd;
    } else 
    {
        nd->left = push_in(nd->left, pair, nd);
        nd = balance(nd, top);
        return nd;
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
void Tree<Key,T,Comp,Allocator>::del_in(Node<Key, T>* nd, Key key, Node<Key, T>* top)
{
    if (nd->first == key)
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
                root = nullptr;

            // delete nd;
            alloc_traits::deallocate(alloc, nd, 1);

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

    } else if (key > nd->first && nd->right != nullptr)
    {
        del_in(nd->right, key, nd);
        nd = balance(nd, top);

    } else if (key < nd->first && nd->left != nullptr)
    {
        del_in(nd->left, key, nd);
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
    if (nd->first == key)
        return nd;

    else if (Comp{}(key, nd->first) && nd->right != nullptr)
        search_in(key, nd->right);

    else if (!Comp{}(key,nd->first) && nd->left != nullptr)
        search_in(key, nd->left);
}

template <typename Key, typename T, class Comp, class Allocator>
Tree<Key,T,Comp,Allocator>::Tree(std::pair<Key,T> pair)
{
    push_in(root, pair, nullptr);
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
void Tree<Key,T,Comp,Allocator>::push (std::pair<Key,T>& pair)
{
    push_in(root, pair, nullptr);
}

template <typename Key, typename T, class Comp, class Allocator>
void Tree<Key,T,Comp,Allocator>::del(Key key)
{
    if (root != nullptr)
    {
        Node<Key,T>* null = nullptr;
        del_in(root, key, null);

    } else 
        std::cout << "Empty Tree\n";
}

template <typename Key, typename T, class Comp, class Allocator>
Node<Key,T>* Tree<Key,T,Comp,Allocator>::search (Key key)
{
    return search_in (key, root);
}

template <typename Key, typename T, class Comp, class Allocator>
Node<Key,T>* Tree<Key,T,Comp,Allocator>::find_min()
{
    Node<Key,T>* tmp = root;
    Node<Key,T>* min = tmp;

    do {

        if (!Comp{}(tmp->first, min->first))
            min = tmp;
        tmp = tmp->left;

    } while (tmp != nullptr);

    return min;
}

template <typename Key, typename T, class Comp, class Allocator>
Node<Key,T>* Tree<Key,T,Comp,Allocator>::find_max()
{
    Node<Key,T>* tmp = root;
    Node<Key,T>* max = tmp;

    do {

        if (Comp{}(tmp->first, max->first))
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

#endif
