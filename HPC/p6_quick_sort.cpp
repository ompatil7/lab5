
#include <iostream>
#include <vector>
#include <algorithm>  // For swap() and is_sorted()
#include <cstdlib>    // For rand() and srand()
#include <ctime>      // For time()
#include <omp.h>

using namespace std;

// Standard Lomuto partition scheme.
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;  // Index of smaller element
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Sequential Quick Sort implementation.
void sequentialQuickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        sequentialQuickSort(arr, low, pi - 1);
        sequentialQuickSort(arr, pi + 1, high);
    }
}

// Threshold to decide when to create new tasks.
// For small subarrays, the overhead of task creation outweighs benefits.
const int QS_THRESHOLD = 10000;

// Parallel Quick Sort using OpenMP tasks.
void parallelQuickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        // If the partition is small, sort sequentially.
        if ((high - low) < QS_THRESHOLD) {
            sequentialQuickSort(arr, low, high);
        } else {
            int pi = partition(arr, low, high);
            // Spawn tasks for the left and right partitions.
            #pragma omp task shared(arr) firstprivate(low, pi)
            {
                parallelQuickSort(arr, low, pi - 1);
            }
            #pragma omp task shared(arr) firstprivate(high, pi)
            {
                parallelQuickSort(arr, pi + 1, high);
            }
            // Wait for both tasks to complete.
            #pragma omp taskwait
        }
    }
}

int main() {
    // Seed the random number generator.
    srand(static_cast<unsigned int>(time(0)));
    
    // Define array size (adjust as needed for testing).
    const int ARRAY_SIZE = 1000000;  // 1,000,000 elements
    vector<int> original(ARRAY_SIZE);
    
    // Fill the array with random integers.
    for (int i = 0; i < ARRAY_SIZE; i++) {
        original[i] = rand() % 1000000;  // Random numbers between 0 and 999,999
    }
    
    // Create copies of the original array for sequential and parallel sorts.
    vector<int> arrSeq = original;
    vector<int> arrPar = original;
    
    // --------- Sequential Quick Sort ----------
    double startSeq = omp_get_wtime();
    sequentialQuickSort(arrSeq, 0, arrSeq.size() - 1);
    double endSeq = omp_get_wtime();
    double timeSeq = endSeq - startSeq;
    
    // --------- Parallel Quick Sort ----------
    double startPar = omp_get_wtime();
    // Start an OpenMP parallel region with a single thread initiating the sorting.
    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            parallelQuickSort(arrPar, 0, arrPar.size() - 1);
        }
    }
    double endPar = omp_get_wtime();
    double timePar = endPar - startPar;
    
    // Verify that both arrays are sorted and identical.
    bool isSeqSorted = is_sorted(arrSeq.begin(), arrSeq.end());
    bool isParSorted = is_sorted(arrPar.begin(), arrPar.end());
    bool arraysEqual = (arrSeq == arrPar);
    
    // --------- Output the Results ----------
    cout << "Sequential Quick Sort:" << endl;
    cout << "  Sorted: " << (isSeqSorted ? "Yes" : "No") << endl;
    cout << "  Time: " << timeSeq << " seconds" << endl << endl;
    
    cout << "Parallel Quick Sort:" << endl;
    cout << "  Sorted: " << (isParSorted ? "Yes" : "No") << endl;
    cout << "  Time: " << timePar << " seconds" << endl << endl;
    
    cout << "Both arrays " << (arraysEqual ? "are identical." : "differ!") << endl;
    
    /*
    Input Description:
    ------------------
    - An array of randomly generated integers of size ARRAY_SIZE.
    
    Compilation Command:
    --------------------
    g++ -fopenmp parallel_quick_sort.cpp -o parallel_quick_sort.exe
    
    Execution:
    -----------
    Run the executable to see the timings for sequential and parallel quick sort.
    Adjust ARRAY_SIZE and QS_THRESHOLD as desired for your experiments.
    */
    
    return 0;
}
