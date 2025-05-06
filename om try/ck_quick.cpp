#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <chrono>
#include <cstdlib>  // for rand() and srand()
#include <ctime>    // for time()

using namespace std;

// Partition function
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Sequential Quick Sort
void sequentialQuickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        sequentialQuickSort(arr, low, pi - 1);
        sequentialQuickSort(arr, pi + 1, high);
    }
}

// Parallel Quick Sort
void parallelQuickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelQuickSort(arr, low, pi - 1);
            #pragma omp section
            parallelQuickSort(arr, pi + 1, high);
        }
    }
}

// Print Array
void printArray(const vector<int>& arr) {
    for (int val : arr) cout << val << " ";
    cout << endl;
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n), arr_copy(n);
    srand(time(0));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
        arr_copy[i] = arr[i];
    }

    auto startSeq = chrono::high_resolution_clock::now();
    sequentialQuickSort(arr, 0, n - 1);
    auto endSeq = chrono::high_resolution_clock::now();

    auto startPar = chrono::high_resolution_clock::now();
    parallelQuickSort(arr_copy, 0, n - 1);
    auto endPar = chrono::high_resolution_clock::now();

    cout << "\n--- Sequential Quick Sort ---\n";
    printArray(arr);
    cout << "Time: " << chrono::duration<double, milli>(endSeq - startSeq).count() << " ms\n";

    cout << "\n--- Parallel Quick Sort ---\n";
    printArray(arr_copy);
    cout << "Time: " << chrono::duration<double, milli>(endPar - startPar).count() << " ms\n";

    return 0;
}