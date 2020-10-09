#include <iostream>
using namespace std;

int A(int q , int p) {
	int train1;
	train1 = -q + p;
	return train1;
}

int main() {
	int  i, j,temp;
	int train[10] = { 0, };
	
	for (int k = 0; k < 9; k++) {
		cin >> i >> j;
		temp = A(i, j);
		train[k+1]= train[k] + temp;
	
	}
	
	int comp = train[0];
	for (int u = 0; u < 10; u++) {
		if (comp < train[u]) {
			comp = train[u];
		}
	}
	cout << comp;


	return 0;
}