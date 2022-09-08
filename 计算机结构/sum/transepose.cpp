#include<iostream>

using namespace std;

void transpose(int m[], int n, int b) {
	int x, y, i, j;
	int *mm;
	mm = new int[n * n];

	for (x = 0; x < n; x += b) {
		for (y = 0; y < n; y += b) {
			//¿éµÄ·­×ª
			for (i = x; i < x + b && i < n; i++) {
				for (j = y; j < y + b && j < n; j++) {
					mm[i * n + j] = m[j * n + i];
				}
			}
		}
	}

	cout << "Changed Matrix: " << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			cout << mm[i * n + j] << ' ';
		cout << endl;
	}

	delete []mm;
}


int main() {
	int *Matrix;
	int n, b;
	cout << "Please set n and b: ";
	cin >> n >> b;
	Matrix = new int[n * n];

	for (int i = 0; i < n*n; i++) Matrix[i] = i;

	cout << "Initial Matrix: " << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			cout << Matrix[i*n+j] << ' ';
		cout << endl;
	}

	transpose(Matrix, n, b);
	delete[]Matrix;
	return 0;
}