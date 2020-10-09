#include <iostream>
#include <cmath>
using namespace std;

int main() {
	int A = 0, B = 0, C = 0, D = 0, Arr[10] = { 0, };
	cin >> A >> B >> C;

	D = A * B * C;
	
	while (D != 0) {
		Arr[D % 10]++;
		D /= 10;
	}
	for (int i = 0; i < 10; i++) {
		cout << Arr[i] << endl;
	}
	return 0;
}