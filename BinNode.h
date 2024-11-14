#ifndef BINNODE_H
#define BINNODE_H
# include <algorithm>
#include <stack>
using Rank = int;
#endif

enum RBColor { RB_RED, RB_BLACK }; // 红黑树节点颜色
#define stature(p) ((int) (( p ) ? ( p ) -> height : -1) )


template <typename T> struct BinNode;
template <typename T> using BinNodePosi = BinNode<T>*; // 节点位置

template <typename T> struct BinNode {
    BinNodePosi<T> parent, lc, rc; // 父亲、孩子
    T data; Rank height, npl; RBColor color; // 数据、高度、npl、颜色

//construction function
    BinNode(): parent(nullptr), lc(nullptr), rc(nullptr), data(T()), height(0), npl(0), color(RB_RED) {} 
    BinNode(T e, BinNodePosi<T> p = nullptr, BinNodePosi<T> lc = nullptr, BinNodePosi<T> rc = nullptr, int h = 0, int l = 1, RBColor c = RB_RED):
            data(e), parent(p), lc(lc), rc(rc), height(h), npl(l), color(c)
            {
                if (lc) lc->parent = this;
                if (rc) rc->parent = this;
            } 

//operation interface
    Rank size(); Rank updateHeight(); void updateHeightAbove(); // renew size and height

    BinNodePosi<T> insertLc( T const & e){return lc = new BinNode<T> (e, this);} // insert left child
    BinNodePosi<T> insertRc( T const & e){return rc = new BinNode<T> (e, this);}; // insert right child

    BinNodePosi<T> succ(); // （中序遍历意义下）当前节点的直接后继

    template <typename VST> void travLevel( VST & ); // 层次遍历
    template <typename VST> void travPre( VST & ); // 先序遍历
    template <typename VST> void travIn( VST & ); // 中序遍历
    template <typename VST> void travPost( VST & ); // 后序遍历
};



template <typename T>
Rank BinNode<T>::updateHeight(){
    return height = 1 + std::max(stature(lc), stature(rc));
    // using stature() is safer than direct access "return height = 1 + max(lc->height, rc->height);" 
    // because it handles nullptr cases by returning -1 
}

template<typename T>
Rank BinNode<T>::size()
{
    Rank s = 1;
    if (lc) s += lc->size();
    if (rc) s += rc->size();
    return s;
}

template <typename T>
void BinNode<T>::updateHeightAbove()
{
    BinNodePosi<T> p = this;
    while (p){
        p->updateHeight();
        p = p->parent;
    }
}


template<typename T, typename VST>
static void visitAlongVine( BinNodePosi<T> x, VST& visit, std::stack<BinNodePosi<T>>& S ){
    while (x){
        visit(x->data);
        if (x -> rc) S.push(x -> rc);
        x = x -> lc;
    }
}



template<typename T>
template<typename VST>
void BinNode<T>::travPre(VST & visit)
{
    std::stack<BinNodePosi<T>> S;
    BinNodePosi<T> x = this;
    S.push(x);
    while( !( S.empty() ) ){
        x = S.top();
        visitAlongVine(x, visit, S);
        S.pop();
    }
}

