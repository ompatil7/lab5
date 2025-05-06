#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>   // For srand() and rand()
#include <ctime>     // For time()

using namespace std;

int main() {
    // Seed the random number generator.
    srand(static_cast<unsigned int>(time(0)));

    // Create a large array of integers. Here we use 1,000,000 elements.
    const int n = 32000000;
    vector<int> arr(n);
    
    // Fill the array with random numbers between 0 and 999.
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000000;
    }
    
    // --------- Sequential Reduction ----------
    int seq_sum = 0;
    int seq_min = arr[0];
    int seq_max = arr[0];
    
    double seq_start = omp_get_wtime();  // Start time for sequential work
    for (int i = 0; i < n; i++) {
        int value = arr[i];
        seq_sum += value;
        if (value < seq_min) seq_min = value;
        if (value > seq_max) seq_max = value;
    }
    double seq_end = omp_get_wtime();
    double seq_time = seq_end - seq_start;
    double seq_avg = static_cast<double>(seq_sum) / n;
    
    // --------- Parallel Reduction with OpenMP ----------
    int par_sum = 0;
    int par_min = arr[0];
    int par_max = arr[0];
    
    // Determine the maximum number of threads available.
    int num_threads = 20;
    omp_set_num_threads(num_threads);
    // Create a vector to count how many iterations each thread processes.
    vector<int> thread_counts(num_threads, 0);
    
    double par_start = omp_get_wtime();  // Start time for parallel work
    
    // Parallelize the loop with reductions.
    #pragma omp parallel for reduction(+: par_sum) reduction(min: par_min) reduction(max: par_max)
    for (int i = 0; i < n; i++) {
        int value = arr[i];
        par_sum += value;
        par_min = (value < par_min) ? value : par_min;
        par_max = (value > par_max) ? value : par_max;
        
        // Record which thread processed this iteration.
        int tid = omp_get_thread_num();
        // Update the count atomically to avoid race conditions.
        #pragma omp atomic
        thread_counts[tid] += 1;
    }
    
    double par_end = omp_get_wtime();
    double par_time = par_end - par_start;
    double par_avg = static_cast<double>(par_sum) / n;
    
    // --------- Print the Results ---------
    cout << "Sequential Reduction Results:" << endl;
    cout << "  Sum:     " << seq_sum << endl;
    cout << "  Min:     " << seq_min << endl;
    cout << "  Max:     " << seq_max << endl;
    cout << "  Average: " << seq_avg << endl;
    cout << "  Time (sequential): " << seq_time << " seconds" << endl << endl;
    
    cout << "Parallel Reduction Results:" << endl;
    cout << "  Sum:     " << par_sum << endl;
    cout << "  Min:     " << par_min << endl;
    cout << "  Max:     " << par_max << endl;
    cout << "  Average: " << par_avg << endl;
    cout << "  Time (parallel): " << par_time << " seconds" << endl << endl;
    
    cout << "Thread Work Distribution (Number of iterations processed):" << endl;
    for (int i = 0; i < num_threads; i++) {
        cout << "  Thread " << i << " processed " << thread_counts[i] << " iterations." << endl;
    }
    
    /*
    Input Description:
    ------------------
    - The program creates a large array (n = 1,000,000) of random integers between 0 and 999.
    
    Compilation Command:
    --------------------
    g++ -fopenmp your_program.cpp -o your_program.exe
    
    Execution:
    -----------
    Run the executable to see the sequential vs. parallel performance and thread workload distribution.
    */
    
    return 0;
}
