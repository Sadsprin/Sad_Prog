#include <iostream>
using namespace std;

int main() {
	int max = 0,V=0, x, arr[100] = { 0, }, avg = 0;
	int b = 99;
	for (int i = 0; i < 10; i++) {
		cin >> x;

		arr[(x / 10) - 1]++;
	}
	for (int j = 0; j < 100; j++) {
		avg += arr[j] * (j + 1);

		if (max <= arr[j]) {
			max = (j + 1) ;
			V = max * 10;
		}
	}

	cout << avg << endl << V;
	return 0;
}
