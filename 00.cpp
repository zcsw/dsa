#include <iostream>
#include "myVector.h"
#include "fibnacci.h"
#include "myList.h"
#include "myListNode.h"

using namespace std;

void dd(int &x){
	x *= 2;
}



int main(){
	myList<int> ml = { 0,1,2,3,4,5,6,7,8 };
	myVector<int> mv = {0,1,2,3,4,5,6,7,8};
	int ia[] = { 0, 2, 4 };
	myList<int> ml2(ia, 2, 3);
	//ml.printAll();
	//mv.printAll();
	ml2.printAll();

	return 0;
}
