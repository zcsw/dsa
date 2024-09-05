#ifndef MYLIST_H
#define MYLIST_H
#include "myListNode.h"
#include <initializer_list>
#include <iostream>
using Rank = int;


template <typename T> class myList{
    protected:


        Rank _size = 0;
        myListNodePosi<T> head;
        myListNodePosi<T> tail;
        void init();

        
    public:
        myList(); //默认构造
        myListNodePosi<T> insertAsLast(T e); //插入到最后
        myList(std::initializer_list<T> il); //初始化列表构造list
		myList(T* p, Rank lo, Rank hi);//array构造list
		//myVector构造list
        void printAll();
        //~mylist() { }; //析构
        myListNodePosi<T> operator[](Rank r);
};




//initialization
template <typename T>
void myList<T>::init() {
	head = new myListNode<T>;
	tail = new myListNode<T>;

	head->succ = tail;
	tail->pred = head;

	_size = 0;
}

//打印所有内容
template <typename T>
void myList<T>::printAll() {
	myListNodePosi<T> p = head->succ;
	while(p->succ != nullptr){
		std::cout << p->data << " ";
		p = p->succ;
	}
	std::cout << std::endl;
}



//默认构造
template <typename T>
myList<T>::myList() {
	init();
}

//将元素插入到最后
template <typename T>
myListNodePosi<T> myList<T>::insertAsLast(T e) {
	tail->insertAsPred(e);
	_size++;
	return tail->pred;
}


//初始化列表构造
template <typename T>
myList<T>::myList(std::initializer_list<T> il) {
	init();
	for (auto& e : il) {
		insertAsLast(e);
	}
}


template <typename T>
myListNodePosi<T> myList<T>::operator[](Rank r) {
	myListNodePosi p = head->succ;
	for (Rank i = 0; i != r; ++i) {
		p = p->succ;
	}
	return p;
}

//array构造 [lo, hi)
template <typename T>
myList<T>::myList(T* p, Rank lo, Rank hi) {
	if (lo >= hi) return;
	init();
	while (lo < hi) insertAsLast(p[lo++]);
}


#endif
