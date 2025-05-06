#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

int main()
{
    int n;
    cout << "Enter n ";
    cin >> n;
    int arr[n];

    for(int  i=0 ; i< n ; i++)
    {
        cout << "Enter " << i << " element : ";
        cin >> arr[i];
    }
    int num_threads = 4;
    omp_set_num_threads(num_threads);

    vector<int> threads_count(num_threads, 0);

    int min_par = arr[0];
    int max_par = arr[0];
    int sum_par = 0;
    #pragma omp parallel for reduction(min: min_par) reduction(max: max_par) reduction(+: sum_par)
    for(int i=0; i<n; i++)
    {
        int value = arr[i];
        sum_par += value;
        if(value < min_par) {
            min_par = value;
        }
        if(value > max_par) {
            max_par = value;
        }
        int tid = omp_get_thread_num();
        #pragma omp atomic
        threads_count[tid] += 1;
    }

    cout << "\n min is " << min_par << endl;
    cout << " max is " << max_par << endl;
    cout << " sum is " << sum_par << endl;
    cout << " avg is " << (sum_par/n) << endl;
    cout << "Thread Work Distribution (Number of iterations processed):" << endl;
    for (int i = 0; i < num_threads; i++) {
        cout << "  Thread " << i << " processed " << threads_count[i] << " iterations." << endl;
    }
}
