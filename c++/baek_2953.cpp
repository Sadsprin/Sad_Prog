#include <iostream>
using namespace std;

int main() {
	int arr[5] = { 0, }, a, b, c, d, e = 0,f=0;
	
	for (int i = 0; i < 5; i++) {
		cin >> a >> b >> c >> d;

		arr[i] = a + b + c + d;

		if (e <= arr[i]) {
			e = arr[i]; f = i + 1;
		}


	}


	cout << f <<" "<< e;
}