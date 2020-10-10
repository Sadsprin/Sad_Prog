#include <iostream>
using namespace std;

int main() {
	int A, B, arr[1000] = { 0, }, init = 0;
	cin >> A >> B;

	for (int i = 1; i < 45; i++) {
		for (int j = 0; j < i;j++) {
			arr[init] = i;
			init++;
		}
	}
	for (int k = 990; k < 1000; k++) {
		arr[k] = 45;
	}
	int sum = 0;
	for (int k = A - 1; k < B; k++) {
		sum += arr[k];
	}
	
	cout << sum;
	
	return 0;
}
