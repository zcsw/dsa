#include <iostream>
#include "myVector.h"
#include "fibnacci.h"
#include "myList.h"
#include "myListNode.h"
#include "BinTree.h"

using namespace std;

void dd(int &x){
	x =  x + (x+1)/26;
}

int main(){

    // int ai[] = {1949, 10 ,1};
    // myList<int>china(ai, 0, sizeof(ai)/sizeof(ai[0]));
    // china.traverse(dd);
    // china.printAll();
    BinTree<int> bt;
    if ( bt.empty() ) cout << "empty" << endl;


	return 0;
}
