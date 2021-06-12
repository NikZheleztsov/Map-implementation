#ifndef TREE_H
#define TREE_H
#include <vector>
#include <functional>
#include <utility>
#include <string>

template <typename, typename, class, class>
class Bidir_const_iter;

template <typename Key, typename T>
struct Node
{
    Key first;
    T second;

    Node() : first(Key()), second(T()), top(nullptr),
        left(nullptr), right(nullptr), dif(0), height(0) {}

    Node(std::pair<Key,T> pair, Node* Top, Node* Left, 
            Node* Right, int Dif, uint64_t h) : 
        first(pair.first), second(pair.second),
        top(Top), left(Left), right(Right),
        dif(Dif), height(h) {}

    bool operator==(Node<Key,T> nd)
    {
        return (first = nd.first && second == nd.second && 
                top == nd.top && left == nd.left &&
                right == nd.right && dif == nd.dif &&
                height == nd.height);
    }

    private:
    Node* top;
    Node* left;
    Node* right;
    int dif; // right - left
    uint64_t height;

    // in order to limit user access to private members
    template <typename, typename, class, class> friend class Tree;
    template <typename, typename, class, class> friend class Map;
    template <typename, typename, class, class> friend class Bidir_iter;
    template <typename, typename, class, class> friend class Bidir_const_iter;
    template <typename, typename, class, class> friend class Reverse_iter;
    template <typename, typename, class, class> friend class Reverse_const_iter;
};

template < 
         typename Key, 
         typename T,
         class Comp = std::greater<Key>,
         class Allocator = std::allocator<Node<Key, T> >
> class Tree
{
    Node<Key,T>* root = nullptr;
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
    std::pair<Node<Key,T>*, Node<Key,T>*> push_in(Node<Key, T>* nd, const std::pair<Key,T>& pair, Node<Key, T>* top);
    Node<Key,T>* search_in (Key key, Node<Key,T>* nd);
    Node<Key,T>* del_in(Node<Key, T>* nd, Key key, Node<Key, T>* top);
    Node<Key,T>* destruct_in (Node<Key, T>* nd, Node<Key, T>* top);
    Node<Key,T>* find_in (Key key, Node<Key,T>* nd);
    void merge_in (Node<Key,T>* nd, Tree& source);

public:

    typedef typename std::allocator_traits<Allocator>   alloc_traits;
    typedef Bidir_const_iter<Key,T,Comp,Allocator>      const_iterator; 
    typedef Node<Key,T>*                                pointer;
    size_t _size;

    Tree() : root(nullptr), _size(0) {}
    Tree(const Tree<Key,T,Comp,Allocator>& other);
    Tree(std::pair<Key,T> pair);
    Tree(Tree<Key,T,Comp,Allocator>&& other);

    Node<Key,T>* push (const std::pair<Key,T>& pair);
    Node<Key,T> del(Key key);
    Node<Key,T>* find (const Key& key);
    Node<Key,T>* find_max() const;
    Node<Key,T>* find_min() const;
    Node<Key,T>* get_root() const { return root; };
    void merge(Tree& source);

    bool operator==( Tree<Key,T,Comp,Allocator>& other );
    bool operator!=( Tree<Key,T,Comp,Allocator>& other );
    Tree& operator=( Tree<Key,T,Comp,Allocator>& other );
    ~Tree();

    template <typename, typename, class, class> friend class Map;
};

#endif
