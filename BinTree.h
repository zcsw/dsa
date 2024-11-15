#include "BinNode.h" //引入二叉树节点类
template <typename T> class BinTree { //二叉树模板类
private:
    static Rank removeAt(BinNodePosi<T> x);
protected:
    Rank _size; BinNodePosi<T> _root; //规模、根节点
public:
    BinTree() : _size( 0 ), _root( nullptr ) {} //构造方法
    ~BinTree() { if ( 0 < _size ) remove( _root ); } //析构方法
    BinTree( BinTree<T> const & ); //复制方法
    Rank size() const { return _size; } //规模
    bool empty() const { return !_root; } //判空
    BinNodePosi<T> root() const { return _root; } //树根
    BinNodePosi<T> insert( T const& ); //插入根节点
    BinNodePosi<T> insert( T const&, BinNodePosi<T> ); //插入左孩子
    BinNodePosi<T> insert( BinNodePosi<T>, T const& ); //插入右孩子
    BinNodePosi<T> attach( BinTree<T>, BinNodePosi<T> ); //接入左子树
    BinNodePosi<T> attach( BinNodePosi<T>, BinTree<T> ); //接入右子树
    Rank remove ( BinNodePosi<T> ); //子树删除
    BinTree<T>* secede ( BinNodePosi<T> ); //子树分离
    template <typename VST> //操作器
    void travLevel( VST& visit ) { if ( _root ) _root->travLevel( visit ); } //层次遍历
    template <typename VST> //操作器
    void travPre( VST& visit ) { if ( _root ) _root->travPre( visit ); } //先序遍历
    template <typename VST> //操作器
    void travIn( VST& visit ) { if ( _root ) _root->travIn( visit ); } //中序遍历
    template <typename VST> //操作器
    void travPost( VST& visit ) { if ( _root ) _root->travPost( visit ); } //后序遍历
    bool operator<( BinTree<T> const& t ) //比较器（其余自行补充）
        { return _root && t._root && ( *_root < *(t._root) ); }
    bool operator==( BinTree<T> const& t ) //判等器
        { return _root && t._root && ( _root == t._root ); }
}; //BinTree


template <typename T>
BinNodePosi<T> BinTree<T>::insert(T const & e, BinNodePosi<T> x)
{
    x->insertLc(e);
    _size++;
    updateHeightAbove(x);
    return x->lc;
}

template <typename T>
BinNodePosi<T> BinTree<T>::insert(BinNodePosi<T> x, T const & e)
{
    x->insertRc(e);
    _size++;
    updateHeightAbove(x);
    return x->rc;
}

template <typename T>
BinNodePosi<T> BinTree<T>::attach(BinTree<T> S, BinNodePosi<T> x)
{
    insert(S._root, x);
    _size += S._size();
    x -> lc -> parent = this;
    x -> updateHeightAbove();
    S._size = 0;
    S._root = nullptr;
    return x;
}

template <typename T>
BinNodePosi<T> BinTree<T>::attach(BinNodePosi<T> x, BinTree<T> S)
{
    insert(x, S._root);
    _size += S._size();
    x -> rc -> parent = this;
    x -> updateHeightAbove();
    S._size = 0;
    S._root = nullptr;
    return x;
}


template <typename T>
Rank BinTree<T>::remove(BinNodePosi<T> x)
{
    // cut connection x an d x's parent
    auto p = x -> parent;
    if( p->lc == x) {
        p->lc = nullptr; 
    }else{
        p->rc = nullptr;
    }
    p->updateHeightAbove();
    // above this function, logical but pysical delete tree x.
    // next, we relese space of tree x
    Rank n = removeAt(x);
    _size -= n;
    return n;
}

template <typename T>
Rank BinTree<T>::removeAt(BinNodePosi<T> x)
{
    if ( !x ) return 0;
    Rank n = 1 + removeAt(x->lc) + removeAt(x->rc);
    return n;
}

template <typename T>
BinTree<T> * BinTree<T>::secede(BinNodePosi<T> x)
{
    auto p = x -> parent;
    if( p->lc == x) {
        p->lc = nullptr; 
    }else{
        p->rc = nullptr;
    }
    p->updateHeightAbove();
    BinTree<T> * S = new BinTree<T>;
    S->_root = x;
    S->_size = x -> size();
    _size -= S->_size;

    return S;
}

