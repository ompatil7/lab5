
#include <iostream>
#include <vector>
#include <algorithm>  // For swap() and is_sorted()
#include <cstdlib>    // For rand() and srand()
#include <ctime>      // For time()
#include <omp.h>
#include<chrono>

using namespace std;


int partition(vector<int> &arr, int low, int high)
{
    int pivot = arr[high];
    int i = low-1;

    for(int j=low; j<high; j++)
    {
        if(arr[j] <= pivot)
        {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i+1], arr[high]);
    return i+1; 
}

void seq_quick(vector<int> &arr, int low, int high)
{
    if(low < high) {
        int pi = partition(arr, low, high);

        seq_quick(arr, low, pi-1);
        seq_quick(arr, pi+1, high);
    }
}

void par_quick(vector<int> &arr, int low, int high)
{
    if(low < high) {
        int pi = partition(arr, low, high);
        
        #pragma omp parallel sections
        {    
            #pragma omp section
            par_quick(arr, low, pi-1);

            #pragma omp section
            par_quick(arr, pi+1, high);
        }
        
        
            
        
    }
}


int main() {
    // int n;
    // cout << "Enter number of elements: ";
    // cin >> n;
    int n = 1000000;

    vector<int> arr(n), arr_copy(n);
    srand(time(0));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000000;
        arr_copy[i] = arr[i];
    }

    auto startSeq = chrono::high_resolution_clock::now();
    seq_quick(arr, 0, n - 1);
    auto endSeq = chrono::high_resolution_clock::now();

    auto startPar = chrono::high_resolution_clock::now();
    
            par_quick(arr_copy, 0, n - 1);
     
    auto endPar = chrono::high_resolution_clock::now();

    cout << "\n--- Sequential Quick Sort ---\n";
    // printArray(arr);
    cout << "Time: " << chrono::duration<double, milli>(endSeq - startSeq).count() << " ms\n";

    cout << "\n--- PAAARALLEL Quick Sort ---\n";
    // printArray(arr);
    cout << "Time: " << chrono::duration<double, milli>(endPar - startPar).count() << " ms\n";

    

    return 0;
}