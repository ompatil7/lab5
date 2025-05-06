#include <iostream>
#include <vector>
#include <omp.h>
#include <random>
#include <algorithm>
#include <iomanip>

using namespace std;

// Function to generate random numbers for testing
vector<int> generateRandomNumbers(int size, int min_val, int max_val) {
    vector<int> numbers(size);
    
    // Set up random number generator
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(min_val, max_val);
    
    // Generate random numbers
    for (int i = 0; i < size; i++) {
        numbers[i] = distrib(gen);
    }
    
    return numbers;
}

// Sequential Bubble Sort
void sequentialBubbleSort(vector<int>& arr) {
    int n = arr.size();
    bool swapped;
    
    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        
        // If no swapping occurred in this pass, the array is already sorted
        if (!swapped)
            break;
    }
}

// Parallel Bubble Sort using odd-even transposition
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    bool swapped = true;
    int phase = 0;
    
    while (swapped) {
        swapped = false;
        
        // Odd phase (compare elements at positions 1&2, 3&4, 5&6, etc.)
        if (phase % 2 == 0) {
            #pragma omp parallel
            {
                bool thread_swapped = false;
                int thread_id = omp_get_thread_num();
                
                #pragma omp for
                for (int i = 1; i < n - 1; i += 2) {
                    if (arr[i] > arr[i + 1]) {
                        swap(arr[i], arr[i + 1]);
                        thread_swapped = true;
                    }
                }
                
                #pragma omp critical
                {
                    if (thread_swapped) {
                        swapped = true;
                        // cout << "Thread " << thread_id << " performed swaps in odd phase " << phase << endl;
                    }
                }
            }
        }
        // Even phase (compare elements at positions 0&1, 2&3, 4&5, etc.)
        else {
            #pragma omp parallel
            {
                bool thread_swapped = false;
                int thread_id = omp_get_thread_num();
                
                #pragma omp for
                for (int i = 0; i < n - 1; i += 2) {
                    if (arr[i] > arr[i + 1]) {
                        swap(arr[i], arr[i + 1]);
                        thread_swapped = true;
                    }
                }
                
                #pragma omp critical
                {
                    if (thread_swapped) {
                        swapped = true;
                        // cout << "Thread " << thread_id << " performed swaps in even phase " << phase << endl;
                    }
                }
            }
        }
        
        phase++;
    }
}

// Function to check if an array is sorted
bool isSorted(const vector<int>& arr) {
    for (int i = 0; i < arr.size() - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}

// Function to print part of the array (for demonstration)
void printArraySample(const vector<int>& arr, int max_items = 10) {
    cout << "Array sample: ";
    for (int i = 0; i < min(max_items, static_cast<int>(arr.size())); i++) {
        cout << arr[i] << " ";
    }
    if (arr.size() > max_items) {
        cout << "... (" << arr.size() - max_items << " more elements)";
    }
    cout << endl;
}

int main() {
    // Set the number of threads
    int num_threads = 10;
    omp_set_num_threads(num_threads);
    cout << "Number of threads: " << num_threads << endl;
    
    // Test with different array sizes
    vector<int> array_sizes = {500, 1000, 2000, 5000, 30000};
    
    cout << "+------------+------------------------+------------------------+---------+" << endl;
    cout << "| Array Size | Sequential Time (ms)   | Parallel Time (ms)     | Speedup |" << endl;
    cout << "+------------+------------------------+------------------------+---------+" << endl;
    
    for (int size : array_sizes) {
        // Generate random array
        vector<int> numbers = generateRandomNumbers(size, 1, 10000);
        vector<int> seq_copy = numbers;
        vector<int> par_copy = numbers;
        
        // Measure sequential sort time
        double start_time = omp_get_wtime();
        sequentialBubbleSort(seq_copy);
        double seq_time = (omp_get_wtime() - start_time) * 1000;
        
        // Verify sequential sort
        bool seq_sorted = isSorted(seq_copy);
        
        // Measure parallel sort time
        start_time = omp_get_wtime();
        parallelBubbleSort(par_copy);
        double par_time = (omp_get_wtime() - start_time) * 1000;
        
        // Verify parallel sort
        bool par_sorted = isSorted(par_copy);
        
        // Calculate speedup
        double speedup = seq_time / par_time;
        
        // Print results in table format
        cout << "| " << setw(10) << size
             << " | " << setw(22) << fixed << setprecision(2) << seq_time
             << " | " << setw(22) << fixed << setprecision(2) << par_time
             << " | " << setw(7) << fixed << setprecision(2) << speedup << " |" << endl;
        
        // Print verification
        if (!seq_sorted || !par_sorted) {
            cout << "WARNING: Sorting verification failed!" << endl;
            cout << "Sequential sort correct: " << (seq_sorted ? "Yes" : "No") << endl;
            cout << "Parallel sort correct: " << (par_sorted ? "Yes" : "No") << endl;
        }
    }
    
    cout << "+------------+------------------------+------------------------+---------+" << endl;
    cout << "\nNote: For small arrays, parallel overhead may outweigh benefits." << endl;
    cout << "For larger arrays, the parallel version should show better performance." << endl;
    
    return 0;
}