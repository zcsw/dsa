#ifndef MYLISTNODE_H
#define MYLISTNODE_H



using Rank = int;
template <typename T> struct myListNode;
template <typename T> using myListNodePosi = myListNode<T>*;

template <typename T> struct myListNode
{
    T data;
    myListNode<T>* pred;
    myListNode<T>* succ;
    //构造
    myListNode() : pred(nullptr), succ(nullptr) {};
    myListNode(T e, myListNodePosi<T> p, myListNodePosi<T> s) { data = e; pred = p; succ = s; };
    //前后插入
    myListNodePosi<T> insertAsPred(const T &e) { myListNodePosi<T> newNode = new myListNode(e, pred, this);  pred->succ = newNode;  pred = newNode; return newNode; };
    myListNodePosi<T> insertAsSucc(const T &e) { myListNodePosi<T> newNode = new myListNode(e, this, succ);  succ->pred = newNode;  succ = newNode; return newNode; };

    //返回值
    T get() { return data; };
};


#endif