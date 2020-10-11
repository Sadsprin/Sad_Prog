#include <iostream>
using namespace std;

int main() {
	int  k=0, p, m, nm;
	cin >> k;
	int* arr = new int[k]();

	for(int q=0; q<k;q++) {
	cin >> p >> m;
	int* array = new int[m]();
		for (int i = 0; i < p; i++) {
			
			
			cin >> nm;
			array[nm-1] += 1;
			
				if (array[nm-1] > 1) {
					arr[q] += 1;
				}
			
		
		}
		delete[] array;
	}
	for (int n2 = 0; n2 < k; n2++)
	 {
		cout << arr[n2] << endl;
	}
	delete[] arr;
	return 0;
}