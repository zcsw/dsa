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
        void copyNodes(myListNodePosi<T> p, int n); //copy n items from  p
        void swap(myListNodePosi<T> p, myListNodePosi<T> q); //swap data of p and q
        myListNodePosi<T> merge(myListNodePosi<T> p, Rank n, myList<T> &L, myListNodePosi<T> q, Rank m); //merge
        void mergeSort(myListNodePosi<T>& p, Rank n); //merge sort on n nodes from p
        void selectionSort(myListNodePosi<T> p, Rank n); //selection sort on n nodes from p
        void insertionSort(myListNodePosi<T> p, Rank n); //insert sort on n nodes from p
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
        bool valid(myListNodePosi<T> p) const //validate position p, although there has a bug that p might not be present in the list.
        {return p && (trailer != p) && (header != p);}
        myListNodePosi<T> find(T const &e) const //unordered list find
        {return find (e, _size, trailer);}
        myListNodePosi<T> find(T const &e, Rank n, myListNodePosi<T> p) const; //unordered interval find
        myListNodePosi<T> search(T const &e) //search the biggest index less-or-equal e in the ordered list search 
        {return search(e, _size, trailer);}
        myListNodePosi<T> search(T const &e, Rank n, myListNodePosi<T> p); //search the biggest index less-or-equal e in the ordered interval search 
        myListNodePosi<T> selectMax(myListNodePosi<T> p, Rank n); //find the pointer to the maximum value among n items starting from p
        myListNodePosi<T> selectMax() //find the pointer to the maximum value in the list
        {return selectMax(first(), _size);}

        //Write access interface
        myListNodePosi<T> insertAsLast(T const &e); //insertAsLast
        myListNodePosi<T> insertAsFirst(T const &e); //insertAsFirst
        myListNodePosi<T> insert(myListNodePosi<T> p, const T& e); //insert e as the successor of p
        myListNodePosi<T> insert(const T& e, myListNodePosi<T> p); //insert e as the predecessor of p
        T remove(myListNodePosi<T> p); //delete the node at valid position p and return the deleted node
        void merge(myList<T> &L) //merge of 2 lists
        {merge(first(), _size, L, L.first(), L._size);} //bug
        void sort(myListNodePosi<T> p, Rank n); //randomly sort a sorting algorithm within an inteval
        void sort() //randomly sort a sorting algorithm within a list
        {sort(first(), _size);}
        Rank dedup(); //de-duplication
        Rank uniquify(); //uniquify
        void reverse(); //reverse

        //traverse
        void traverse(void (*visit) (T&)); //traverse by function pointer
        template <typename VST> void traverse( VST& ); //traverse by function object
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

template <typename T>
void myList<T>::swap(myListNodePosi<T> p, myListNodePosi<T> q)
{
    T backUpValue = p -> data;
    p -> data = q -> data;
    q -> data = backUpValue;
}

template <typename T>
myListNodePosi<T> myList<T>::merge(myListNodePosi<T> p, Rank n, myList<T> &L,myListNodePosi<T> q, Rank m)
{
    auto pp = p->pred;
    while ((m > 0) && (p != q)){
        if ((p->data <= q->data)&&(n > 0)){
            p = p->succ;
            n--;
        }else{
            insert(q->data, p);
            q = q->succ;
            L.remove(q->pred);
            m--;
        }
    }
    
    return pp->succ;
}

template <typename T>
void myList<T>::mergeSort(myListNodePosi<T> &p, Rank n)
{
    if (n > 1){
        auto mid = n >> 1;
        auto q = p;
        while (mid > 0){
            q = q->succ;
            mid--;
        }
        mid = n >> 1;
        mergeSort(p, mid);
        mergeSort(q, n - mid);
        p = merge(p, mid, *this, q, n - mid); //removing "p = " causes a bug, but i don't know why. (claude 3.5 summarize my issue:pointer referencing within the recursive process)
    }else 
        return ;
}

template <typename T>
void myList<T>::selectionSort(myListNodePosi<T> p, Rank n)
{
    Rank nn = n;
    myListNodePosi<T> unsortedLastPosi = p;
    while(--nn){
        unsortedLastPosi = unsortedLastPosi -> succ;
    }
    while (n)
    {
        auto tMaxPosi = selectMax(p, n);
        swap(tMaxPosi, unsortedLastPosi);
        unsortedLastPosi = unsortedLastPosi -> pred;
        --n;
    }
    
}

template <typename T>
void myList<T>::insertionSort(myListNodePosi<T> p, Rank n)
{
    for(Rank r = 0; r < n; r++){
        insert(search(p->data, r, p) ,p->data);
        p = p->succ;
        remove(p->pred);
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

// find e starting at p->pred to head
template <typename T>
myListNodePosi<T> myList<T>::find(T const &e, Rank n, myListNodePosi<T> p) const
{
    p = p->pred;
    while ((p != header) && (p->data != e) && (--n != 0)){
        p = p->pred;
    }
    
    return p->data == e ? p : nullptr;
}

template <typename T>
myListNodePosi<T> myList<T>::search(T const &e, Rank n, myListNodePosi<T> p)
{
    p = p->pred;
    while ((p != header) && (p->data > e) && (--n >= 0)){
        p = p->pred;
    }
    return p;
}

template <typename T>
myListNodePosi<T> myList<T>::selectMax(myListNodePosi<T> p, Rank n)
{
    auto ret = p;
    while((p != trailer) && (n != 0)){
        if(ret->data <= p->data) ret = p;
        p = p->succ;
        n--;
    }
    return ret;
}

// append
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
myListNodePosi<T> myList<T>::insert(myListNodePosi<T> p, const T &e)
{
    return p->insertAsSucc(e);
}

template <typename T>
myListNodePosi<T> myList<T>::insert(const T &e, myListNodePosi<T> p)
{
    return p->insertAsPred(e);
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

template <typename T>
void myList<T>::sort(myListNodePosi<T> p, Rank n)
{
    insertionSort(p, n);
}

template <typename T>
Rank myList<T>::dedup()
{
    int ret = 0;
    auto p = first() -> succ;
    while(p != trailer){
        auto q = p->pred;
        while(q != header){
            if(q->data == p->data){
                remove((q = q->pred) ->succ);
                ret++;
            }else{
                q = q->pred;
            }
        }
    p = p->succ;

    }
    return ret;
}

template <typename T>
Rank myList<T>::uniquify()
{
    int ret = 0;
    auto p = first();
    while(p != last()){
        if(p -> succ ->data == p -> data){
            remove((p = p -> succ) -> pred);
            ret++;
        }
        else p = p -> succ;
    }
    return ret;
}

template <typename T>
void myList<T>::reverse()
{
    auto p = first();
    auto q = last();
    while( (p != q) && (p->succ != q) ){
        swap(p, q);
        p = p -> succ;
        q = q -> pred;
    }
}

template <typename T>
void myList<T>::traverse(void (*visit)(T &))
{
    auto p = first();
    for (Rank r = 0; r < _size; r++){
        visit(p->data);
        p = p->succ;
    }
}

template <typename T>
template <typename VST>
void myList<T>::traverse(VST &)
{
    auto p = first();
    for (Rank r = 0; r < _size; r++){
        visit(p->data);
        p = p->succ;
    }
}


#endif
