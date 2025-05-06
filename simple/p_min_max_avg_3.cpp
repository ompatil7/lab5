#include <iostream>
#include <vector>
#include <limits>   // For numeric_limits
#include <omp.h>

using namespace std;

int main() {
    // Sample input: a vector of integers.
    // You can change these numbers or extend the array as needed.
    vector<int> arr = {12, 7, 9, 3, 15, 8, 4, 20, 11, 6};
    int n = arr.size();

    // omp_set_num_threads(9); to set maxiumum number of threads to use
 
   // Initialize variables for the reduction.
    // Use a sum variable for the total sum.
    int sum = 0;
    // For minimum, initialize with the maximum possible integer.
    int minVal = numeric_limits<int>::max();
    // For maximum, initialize with the minimum possible integer.
    int maxVal = numeric_limits<int>::min();

    // int t_counts = 1; to count number of threads
   
    // Parallel region with OpenMP reduction clauses:
    // - reduction(+: sum) will sum up elements.
    // - reduction(min: minVal) will compute the minimum element.
    // - reduction(max: maxVal) will compute the maximum element.
    #pragma omp parallel for reduction(+: sum) reduction(min: minVal) reduction(max: maxVal)
    for (int i = 0; i < n; i++) {
        int value = arr[i];
        sum += value;
        int t_id = omp_get_thread_num();
        // cout << "Thread : " << t_id << endl;
        
        // if(t_counts < t_id) t_counts = t_id; count number of threads


        // The reduction clauses automatically handle min and max comparisons,
        // so here we simply "apply" the values.
        // The OpenMP runtime internally performs:
        // minVal = (value < minVal) ? value : minVal;
        // maxVal = (value > maxVal) ? value : maxVal;
        // Note: Writing explicit if-statements here is not necessary.
        // You could also write:
        // if (value < minVal) minVal = value;
        // if (value > maxVal) maxVal = value;
        // However, the reduction clause takes care of combining thread-private variables.
        minVal = (value < minVal) ? value : minVal;
        maxVal = (value > maxVal) ? value : maxVal;
    }
    
    // Compute average as a double value
    double average = static_cast<double>(sum) / n;
    
    // Output the results
    cout << "Results:" << endl;
    cout << "Sum: " << sum << endl;
    cout << "Min: " << minVal << endl;
    cout << "Max: " << maxVal << endl;
    cout << "Average: " << average << endl;
    // cout << "Total threads used " << t_counts+1 << endl; print num threads
    return 0;
}

/*
Input Details:
--------------
The array 'arr' is defined within the code. In this example, the array is:
    {12, 7, 9, 3, 15, 8, 4, 20, 11, 6}

From this array:
- Sum   = 12 + 7 + 9 + 3 + 15 + 8 + 4 + 20 + 11 + 6 = 95
- Min   = 3
- Max   = 20
- Average = 95 / 10 = 9.5

Compilation Command:
--------------------
g++ -fopenmp parallel_BFS_1.cpp -o parallel_BFS_1.exe

Run the executable to see the output.
*/
