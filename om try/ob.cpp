#include<bits/stdc++.h>
#include<omp.h>
#include<chrono>

using namespace std;

int partition(vector<int> &arr, int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;
    for(int j=low; j< high; j++)
    {
        if(arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i+1], arr[high]);
    return i+1;
}
void seq_q(vector<int> &arr, int low, int high)
{
    if(low < high)
    {
        int pi = partition(arr, low, high);
        seq_q(arr, low, pi-1);
        seq_q(arr, pi+1, high);
    }
}

void par_q(vector<int> &arr, int low, int high)
{
    if(low < high)
    {
        int pi = partition(arr, low, high);
        #pragma omp parallel sections
        {
            #pragma omp section
            par_q(arr, low, pi-1);

            #pragma omp section
            par_q(arr, pi+1, high);
        }
    }    
}

int main()
{
    vector<int> as = {5000, 15000, 25000, 35000, 550000};
    
    for(int i=0; i<as.size(); i++)
    {
        vector<int> arr(as[i], 0);
        srand(time(0));
        int n = arr.size();

        for(int i=0; i<n; i++)
        {
            arr[i] = rand() % 1000000;
        }
        vector<int> arr2 = arr; // Copy for parallel sort
        
        auto s1 = chrono::high_resolution_clock::now();
        seq_q(arr, 0, n-1);
        auto e1 = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> d1 = e1-s1;

        // omp_set_nested(1);


        auto s2 = chrono::high_resolution_clock::now();
        par_q(arr2, 0, n-1);
        auto e2 = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> d2 = e2-s2;

        cout << "Size - " << as[i] << endl;
        cout << "Seq time : " << d1.count() << endl;
        cout << "Par time : " << d2.count() << endl;
        cout << "--------------------------------------------------"  << endl;
    }
}