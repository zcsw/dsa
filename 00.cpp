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

    int ai[] = {1, 9, 4, 9, 1, 0, 0, 1};
    myList<int>china(ai, 0, sizeof(ai)/sizeof(ai[0]));
    myList<int>china2(china.first(), 2);
    china2.printAll();


	return 0;
}
