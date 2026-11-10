#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

int main() {
    int n;
    cout << "Enter matrix size (n x n): ";
    if (!(cin >> n) || n <= 0) {
        n = 4; // Значення за замовчуванням, якщо ввели щось не те
        cout << "Invalid input, using default n = 4\n";
    }

    omp_set_num_threads(4);

    vector<vector<int>> A(n, vector<int>(n));
    vector<vector<int>> B(n, vector<int>(n));

    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = rand() % 100 - 50;
            B[i][j] = rand() % 100 - 50;
        }
    }

    vector<int> C(n, 0);
    vector<int> D(n, 0);

    cout << "           LABORATORY WORK N9           \n";
    cout << " OpenMP Parallel Loop Scheduling        \n";
    cout << "Variant: 1 (Find minimum of each row)\n";
    cout << "Matrix size n = " << n << "\n";

    cout << "Matrix A:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) cout << A[i][j] << "\t";
        cout << "\n";
    }

    cout << "\nMatrix B:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) cout << B[i][j] << "\t";
        cout << "\n";
    }


#pragma omp parallel
    {
#pragma omp for schedule(static, 2)
        for (int i = 0; i < n; i++) {
            int row_min = A[i][0];
            for (int j = 1; j < n; j++) {
                if (A[i][j] < row_min) {
                    row_min = A[i][j];
                }
            }
            C[i] = row_min;

#pragma omp critical
            {
                cout << "[Matrix A - Static(2)] Thread " << omp_get_thread_num()
                    << " processed row " << i << " (Min = " << C[i] << ")\n";
            }
        }

#pragma omp for schedule(dynamic, 1)
        for (int i = 0; i < n; i++) {
            int row_min = B[i][0];
            for (int j = 1; j < n; j++) {
                if (B[i][j] < row_min) {
                    row_min = B[i][j];
                }
            }
            D[i] = row_min;

#pragma omp critical
            {
                cout << "[Matrix B - Dynamic(1)] Thread " << omp_get_thread_num()
                    << " processed row " << i << " (Min = " << D[i] << ")\n";
            }
        }
    }

    cout << "RESULTS (ROW MINIMUMS):\n";

    cout << "Matrix A (static, chunk=2):\n";
    for (int i = 0; i < n; i++) {
        cout << "Row " << i << " -> min = " << C[i] << "\n";
    }

    cout << "\nMatrix B (dynamic, chunk=1):\n";
    for (int i = 0; i < n; i++) {
        cout << "Row " << i << " -> min = " << D[i] << "\n";
    }

    return 0;
}
