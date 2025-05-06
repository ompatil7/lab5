#include <iostream>
#include <vector>
#include <cstdlib>    // For rand() and srand()
#include <ctime>      // For time()
#include <omp.h>
#include <algorithm>  // For std::is_sorted

using namespace std;

// Merge function: merges two sorted halves of arr[left..right]
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    // Create temporary arrays.
    vector<int> L(n1), R(n2);
    
    // Copy data to temp arrays.
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    // Merge the temp arrays back into arr[left..right]
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    
    // Copy any remaining elements of L[]
    while (i < n1)
        arr[k++] = L[i++];
    
    // Copy any remaining elements of R[]
    while (j < n2)
        arr[k++] = R[j++];
}

// Sequential merge sort function.
void sequentialMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Threshold to decide when to stop spawning new tasks.
// For small segments, it's better to sort sequentially.
const int THRESHOLD = 1000;

// Parallel merge sort using OpenMP tasks.
void parallelMergeSort(vector<int>& arr, int left, int right, int depth = 0) {
    if (left < right) {
        if (right - left < THRESHOLD) {
            // For small parts, do sequential merge sort.
            sequentialMergeSort(arr, left, right);
        } else {
            int mid = left + (right - left) / 2;
            // Create tasks for the left and right halves.
            #pragma omp task shared(arr) firstprivate(left, mid, depth)
            {
                parallelMergeSort(arr, left, mid, depth + 1);
            }
            #pragma omp task shared(arr) firstprivate(mid, right, depth)
            {
                parallelMergeSort(arr, mid + 1, right, depth + 1);
            }
            // Wait for both tasks to complete.
            #pragma omp taskwait
            merge(arr, left, mid, right);
        }
    }
}

int main() {
    // Seed the random number generator.
    srand(static_cast<unsigned int>(time(0)));
    
    // Define array size. You can adjust the size for testing.
    const int ARRAY_SIZE = 1000000;  // e.g., 1,000,000 elements
    vector<int> original(ARRAY_SIZE);
    
    // Fill the array with random integers between 0 and 9999.
    for (int i = 0; i < ARRAY_SIZE; i++) {
        original[i] = rand() % 10000;
    }
    
    // Create copies for sequential and parallel sorts.
    vector<int> arrSeq = original;
    vector<int> arrPar = original;
    
    // --------- Sequential Merge Sort ----------
    double seqStart = omp_get_wtime();
    sequentialMergeSort(arrSeq, 0, arrSeq.size() - 1);
    double seqEnd = omp_get_wtime();
    double timeSeq = seqEnd - seqStart;
    
    // --------- Parallel Merge Sort ----------
    double parStart = omp_get_wtime();
    // Begin a parallel region. Use a single thread to start the recursive tasks.
    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            parallelMergeSort(arrPar, 0, arrPar.size() - 1);
        }
    }
    double parEnd = omp_get_wtime();
    double timePar = parEnd - parStart;
    
    // Validate both arrays are sorted and identical.
    bool isSeqSorted = is_sorted(arrSeq.begin(), arrSeq.end());
    bool isParSorted = is_sorted(arrPar.begin(), arrPar.end());
    bool arraysEqual = (arrSeq == arrPar);
    
    // --------- Output the Results ----------
    cout << "Sequential Merge Sort:" << endl;
    cout << "  Sorted: " << (isSeqSorted ? "Yes" : "No") << endl;
    cout << "  Time: " << timeSeq << " seconds" << endl << endl;
    
    cout << "Parallel Merge Sort:" << endl;
    cout << "  Sorted: " << (isParSorted ? "Yes" : "No") << endl;
    cout << "  Time: " << timePar << " seconds" << endl << endl;
    
    cout << "Both arrays " << (arraysEqual ? "are identical." : "differ!") << endl;
    
    /*
    Input Description:
    ------------------
    - An array of randomly generated integers of size ARRAY_SIZE.
    
    Compilation Command:
    --------------------
    g++ -fopenmp parallel_merge_sort.cpp -o parallel_merge_sort.exe
    
    Execution:
    -----------
    Run the executable to see the timings for both sequential and parallel merge sorts.
    Adjust ARRAY_SIZE and THRESHOLD as desired to experiment with performance.
    */
    
    return 0;
}
