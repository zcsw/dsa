#ifndef MYLIST_H
#define MYLIST_H
#include "myListNode.h"
#include <initializer_list>
#include <iostream>
using Rank = int;


template <typename T> class myList{
    private:
        Rank _size; //size
        myListNodePosi<T> header;  //head node
        myListNodePosi<T> trailer;  //tail node


    protected:
        void init();  //list initialization during creation
        int clear(); //clear all nodes
        void copyNodes(myListNodePosi<T> p, int n); //copy n items from the list starting at position p


    public:
        //Construction
        myList(); //default constructor
        myList(std::initializer_list<T> il); //constructing list from an initializer list
		myList(T* p, Rank lo, Rank hi); //constructing list from an array
		myList(myList<T> const &ml); //Deep copy list
		myList(myList<T> const &ml, Rank r, int n); //copy list n items starting at Rank r
		myList(myListNodePosi<T> p, int n); //copy list n items starting at position p
		     //constructing list from myVetor
        void printAll() const; //print all members


        //Destruction
        ~myList();


        //Read-only access interface
        Rank size() const {return _size;};
        bool empty() const {return _size = 0;}
        T& operator[](Rank r) const ; //overload the [] operator
        myListNodePosi<T> first() const {return header->succ;}; //fist node position
        myListNodePosi<T> last() const {return trailer->pred;}; //last node position





        //Write access interface
        myListNodePosi<T> insertAsLast(T const &e); //insertAsLast
        myListNodePosi<T> insertAsFirst(T const &e); //insertAsFirst

        T remove(myListNodePosi<T> p); //delete the node at valid position p and return the deleted node

};




//initialization
template <typename T>
void myList<T>::init() {
	header = new myListNode<T>;
	trailer = new myListNode<T>;

	header->succ = trailer; header->pred = nullptr;
	trailer->pred = header; trailer->succ = nullptr;

	_size = 0;
}


//clear
template <typename T>
int myList<T>::clear(){
    int oldSize = _size;
    while(header -> succ != trailer) remove(first());

    return oldSize;
}


//  copyNodes
template <typename T>
void myList<T>::copyNodes(myListNodePosi<T> p, int n){ //p is not referenced here because there id  no permission to modify it
    init();
    while( n-- ){
        insertAsLast(p -> data);
        p = p -> succ;
    }
}


//print all
template <typename T>
void myList<T>::printAll() const{
	myListNodePosi<T> p = first();
	while(p->succ != nullptr){
		std::cout << p->data << " ";
		p = p->succ;
	}
	std::cout << std::endl;
}



//default constructor
template <typename T>
myList<T>::myList() {
	init();
}




//initializer -> myList
template <typename T>
myList<T>::myList(std::initializer_list<T> il) {
	init();
	for (auto& e : il) {
		insertAsLast(e);
	}
}


//myList -> myList
template <typename T>
myList<T>::myList(myList const &ml){
//    init();
//    myListNodePosi<T> p = ml.first();
//    while (p != nullptr){
//        insertAsLast(p -> data);
//        p = p -> succ;
//    }
    copyNodes(ml.first(), ml.size());
}

//myList[r, r+n) ->myList
template <typename T>
myList<T>::myList(myList<T> const &ml, Rank r, int n){
    myListNodePosi<T> p = ml.first();
    while(r--) p = p->succ;
    copyNodes(p, n);
}


//myList[rank p, p+n) ->myList
template <typename T>
myList<T>::myList(myListNodePosi<T> p, int n){
    copyNodes(p, n);
}






//list construction from array [ )
template <typename T>
myList<T>::myList(T* p, Rank lo, Rank hi) {
	if (lo >= hi) return;
	init();
	while (lo < hi) insertAsLast(p[lo++]);
}


//destruction
template <typename T>
myList<T>::~myList(){
    clear();
    delete header;
    delete trailer;
}





//myListNodePosi<T> operator[](Rank r) const;
template <typename T>
T& myList<T>::operator[] (Rank r) const {
	myListNodePosi<T> p = first();
	for (Rank i = 0; i != r; ++i) {
		p = p->succ;
	}
	return p->data;
}



//append
template <typename T>
myListNodePosi<T> myList<T>::insertAsLast(T const &e) {
	trailer->insertAsPred(e);
	_size++;
	return trailer->pred;
}



template <typename T>
myListNodePosi<T> myList<T>::insertAsFirst(T const &e) {
    header->insertAsSucc(e);
    _size++;
    return header->succ;
}



template <typename T>
T myList<T>::remove(myListNodePosi<T> p){
    T ret = p -> data; //back up for return
    p -> pred -> succ = p -> succ;
    p -> succ -> pred = p -> pred;
    delete p;
    _size--;
    return ret;

}





#endif
