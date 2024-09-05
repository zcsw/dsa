#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <iostream>
#include <algorithm>
#include <initializer_list>
#include <cstdlib>
#include "fibnacci.h"

using Rank = int;
#define DEFAULT_CAPACITY 3

template <typename T> class myVector{
	private: 
		Rank _size; 
		Rank _capacity; 
		T* _elem;
		void copyFrom(T const* A, Rank lo, Rank hi);
		void expand();
		void shrink();
	protected:
		void merge(Rank lo, Rank mi, Rank hi);

	public:
	//构造和析构函数
		myVector(Rank c = DEFAULT_CAPACITY);
		myVector(std::initializer_list<T> il);
		myVector(T const* A, Rank lo, Rank hi);
		myVector(myVector<T> const &A);
		myVector(myVector<T> const &A, Rank lo, Rank hi);
		~myVector();
	//返回private属性
		Rank size(){return _size;}
		Rank capacity(){return _capacity;}
		T* elem(){return _elem;}
		void printAll();
	//重载[]取元素
		T& operator[](Rank i);
		const T& operator[](Rank i) const;
	//插入及删除
		Rank insert(Rank r, const T& e);
		Rank remove(Rank lo, Rank hi);
		T remove(Rank r);
	//无序向量
		Rank find(const T& e, Rank lo, Rank hi) const;
		Rank dedup();
	//遍历
		void traverse(void (*visit) (T &));
		template <typename VST> void traverse(VST visit );
	//判断无序性, 返回逆序数个数
		int disordered()const;
	//有序向量
		int uniquify();
		Rank search(const T& e, Rank lo, Rank hi );
		Rank interpolationSearch(const T& e, Rank lo, Rank hi);
	//排序算法
	void bubbleSort(Rank lo, Rank hi);
	void mergeSort(Rank lo, Rank hi);
};

//myVector实现

//复制函数
template<typename T>
void myVector<T>::copyFrom(T const* A, Rank lo, Rank hi) {
	_elem = new T[_capacity = std::max((hi - lo) << 1, DEFAULT_CAPACITY)]; //分配空间
	for (_size = 0; lo != hi; ++lo, ++_size) {
		_elem[lo] = A[lo];
	}
}

//创建构造
template<typename T>
myVector<T>::myVector(Rank c) {
	_capacity = c;
	_elem = new T[_capacity = c];
	_size = 0;
}

//初始化列表构造
template <typename T>
myVector<T>::myVector(std::initializer_list<T> il) {
	_size = il.size();
	_capacity = std::max(_size, DEFAULT_CAPACITY);
	_elem = new T[_capacity];
	std::copy(il.begin(), il.end(), _elem);
}

//数组区间复制
template <typename T>
myVector<T>::myVector(T const* A, Rank lo, Rank hi) {
	copyFrom(A, lo, hi);
}


//向量区间复制
template <typename T>
myVector<T>::myVector(myVector<T> const& A, Rank lo, Rank hi) {
	copyFrom(A._elem, lo, hi);
}



//向量完整复制
template <typename T>
myVector<T>::myVector(myVector<T> const& A) {
	copyFrom(A._elem, 0, A._size);
}


//析构函数
template<typename T>
myVector<T>::~myVector() {
	delete[] _elem;
}




//打印向量所有元素
template <typename T>
void myVector<T>::printAll() {
	for (Rank i = 0; i != _size; ++i) std::cout << _elem[i] << " ";
	std::cout << std::endl;
}





//数组扩充
template <typename T>
void myVector<T>::expand() {
	if (_size < _capacity) return;
	_capacity = std::max(DEFAULT_CAPACITY, _capacity);
	T* oldElem = _elem;
	_elem = new T[_capacity <<= 1];
	for (Rank i = 0; i != _size; ++i) {
		_elem[i] = oldElem[i];
	}
	delete[] oldElem;
}

template <typename T>
void myVector<T>::shrink() {
	if (_size << 2 >= _capacity) return;
	_capacity = std::max(DEFAULT_CAPACITY, _capacity >> 1);
	T* oldElem = _elem;
	_elem = new T[_capacity];
	for (Rank r = 0; r != _size; ++r) {
		_elem[r] = oldElem[r];
	}
	delete[] oldElem;
}


//重载[]方便获取元素
template <typename T>
T& myVector<T>::operator[](Rank r) { return _elem[r]; } //左右值


template <typename T>
const T& myVector<T>::operator[](Rank r) const { return _elem[r]; } //右值


//a Rank r inserted by a value e
template <typename T>
Rank myVector<T>::insert(Rank r, const T& e) {
	expand(); //expand if nessesary
	for (Rank i = _size; r < i; --i) {
		_elem[i] = _elem[i - 1];
	} //O(n - r)
	_elem[r] = e; //insert e, O(1)
	++_size;
	return r;
}

//remove from Rank low to high
template <typename T>
Rank myVector<T>::remove(Rank lo, Rank hi) {
	Rank n = hi - lo;
	for (Rank r = hi; r != _size; ++r) {
		_elem[r - n] = _elem[r];
	}
	shrink();
	_size -= n;
	return n;
}

// remove single element
template <typename T>
T myVector<T>::remove(Rank r) {
	T e = _elem[r]; //back up as return
	remove(r, r + 1);
	return e;
}

//无序向量的顺序查找O(n)
template <typename T>
Rank myVector<T>::find(const T& e, Rank lo, Rank hi) const {
	while ((hi-- != lo) && (_elem[hi] != e)) {}
	return hi;
}

//无序向量的去重O(n^2)
template <typename T>
Rank myVector<T>::dedup() {
	Rank oldSize = _size;
	for (Rank r = 1; r != _size; ) {
		if (-1 == find(_elem[r], 0, r)) {
			++r;
		}
		else {
			remove(r);
		}
	}
	return oldSize - _size;
}

//通过函数指针遍历
template <typename T>
void myVector<T>::traverse(void (*visit) (T&)) {
	for (Rank i = 0; i != _size; ++i) visit(_elem[i]);
}
//通过函数对象遍历
template <typename T>
template <typename VST>
void myVector<T>::traverse(VST visit) {
	for (Rank i = 0; i != _size; ++i) visit(_elem[i]);
}

//加一函数
template <typename T>
struct Increase {
	virtual void operator()(T& e) { ++e; }
};

template <typename T>
void increase(myVector<T>& V) {
	V.traverse(Increase<T>());
}

// 检查有序性, 返回逆序个数
template <typename T>
int myVector<T>::disordered() const {
	int n = 0;
	for (Rank i = 1; i != _size; ++i) {
		if (_elem[i - 1] > _elem[i]) ++n;
	}
	return n;
}

// //有序向量唯一化-低效版本 O(n^2)
// template <typename T> 
// int myVector<T>::uniquify(){
// 	int oldSize = _size;
// 	Rank i = 1;
// 	while (i != _size){
// 		_elem[i - 1] == _elem[i] ? remove(i) : ++i;
// 	}
//	shrink()
// 	return oldSize - _size;
// }


// 有序向量唯一化-高效版本 O(n)
template <typename T>
int myVector<T>::uniquify() {
	Rank i = 0, j = 0;
	while (++j != _size) {
		if (_elem[i] != _elem[j]) _elem[++i] = _elem[j];
	}
	_size = i + 1;
	shrink();
	return j - _size;

}

/*
//有序向量二分查找, 返回值为不小于查找值-版本A/3, 1.5*lg(n)
template <typename T>
Rank binSearch(T* S, const T& e, Rank lo, Rank hi){
	while (lo < hi)
	{
		Rank mi = (hi + lo) >> 2;
		if (e < S[mi]) hi = mi;
		else if (S[mi] < e) lo = mi;
		else return mi;
	}
	return -1;
}
*/

/*
// 有序向量二分查找-版本B/3
template <typename T>
Rank binSearch(T* S, const T& e, Rank lo, Rank hi){
	while (hi - lo > 1){
		Rank mi = (lo + hi) >> 2;
		if (e < S[mi]) hi = mi;
		else lo = mi;
	}
	return S[lo] = e ? e : -1;
}
*/


// 有序向量二分查找-版本C/3 O(1.00 log n)
template <typename T>
Rank binSearch(T* S, const T& e, Rank lo, Rank hi) {
	while (lo < hi) {
		Rank mi = (lo + hi) >> 1;
		e < S[mi] ? hi = mi : lo = mi + 1;
	}
	return lo - 1;
}

template <typename T>
Rank fibSearch(T* S, const T& e, Rank lo, Rank hi) {
	while (lo < hi) {
		Rank mi = lo + Fib(hi - lo).prev();
		e < S[mi] ? hi = mi : lo = mi + 1;
	}
	return lo - 1;
}


//随机选取一个二分查找
template <typename T>
Rank myVector<T>::search(const T& e, Rank lo, Rank hi) {
	return rand() % 2 ? binSearch(_elem, e, lo, hi) : fibSearch(_elem, e, lo, hi);
}


//插值查找
template <typename T>
Rank myVector<T>::interpolationSearch(const T& e, Rank lo, Rank hi) {
	hi -= 1;
	while (lo < hi) {
		if ((e < _elem[lo]) || (_elem[hi] < e)) return -1;
		if (_elem[lo] == _elem[hi]) return hi;

		Rank mi = lo + (hi - lo + 1) * (e - _elem[lo]) / (_elem[hi] - _elem[lo]);

		if (e < _elem[mi]) hi = mi - 1;
		else if (e == _elem[mi]) {
			while (e == _elem[mi]) ++mi;
			return mi - 1;
		}
		else
			lo = mi + 1;
	}
	return (_elem[hi] == e) ? hi : -1;
}

//起泡排序
template <typename T>
void myVector<T>::bubbleSort(Rank lo, Rank hi) {
	for (bool sorted = false; sorted = !sorted; --hi) {
		for (Rank i = lo; i != hi - 1; ++i) {
			if (_elem[i] > _elem[i + 1]) std::swap(_elem[i], _elem[i + 1]);
		}
	}
}


//归并排序
template <typename T>
void myVector<T>::merge(Rank lo, Rank mi, Rank hi) {
	Rank lb = mi - lo;
	Rank lc = hi - mi;
	T* A = _elem + lo;
	T* B = _elem + mi;
	T* C = new T[lb];
	for (Rank i = 0; i != lb; ++i) C[i] = A[i];
	Rank i = 0, j = 0, k = 0;
	while ((i < lb) && (j < lc)) {
		A[k++] = (C[i] <= B[j]) ? C[i++] : B[j++];
	}
	while (i < lb) A[k++] = C[i++];
	delete[] C;
}

template <typename T>
void myVector<T>::mergeSort(Rank lo, Rank hi) {
	if (hi - lo < 2) return;
	Rank mi = (lo + hi) >> 1;
	mergeSort(lo, mi);
	mergeSort(mi, hi);
	merge(lo, mi, hi);
}


#endif
