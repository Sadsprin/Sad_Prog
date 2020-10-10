#include <iostream>

using namespace std;

int main() {
	int A , B, arr[42] = { 0, }, u = 0;

	for (int i = 0; i < 10; i++) {
		cin >> A;
		B = A % 42;
		arr[B]++;
			
		}
	
	for (int k = 0; k < 42; k++) {
		if (arr[k] > 0) {
			u++;
		}
	}
	cout << u;

	return 0;


}