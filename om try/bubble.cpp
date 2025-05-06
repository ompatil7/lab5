#include<bits/stdc++.h>
#include<omp.h>
using namespace std;


void seq_bub(vector<int>& arr)
{
    int n = arr.size();

    for(int i=0; i<n-1; i++)
    {
        bool swapped = false;
        for(int j=0; j<n-i-1; j++)
        {
            if(arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }

}

void par_bub(vector<int> &arr)
{
    int n = arr.size();
    
    #pragma omp parallel
    {
        for(int i=0; i<n; i++)
        {
            #pragma omp barrier
            #pragma omp for
            for(int j = i%2; j < n-1; j += 2)
            {
                if(arr[j] > arr[j+1])
                    swap(arr[j], arr[j+1]);
            }
        }
    }
}

void printArray(const vector<int>& arr) {
    for (auto x : arr) cout << x << " ";
    cout << endl;
}

int main()
{
    vector<int> array_sizes = {3032, 50444};

    for(int i=0; i<array_sizes.size(); i++)
    {
        vector<int> arr(array_sizes[i]);
        vector<int> arr2(array_sizes[i]);

        srand(time(0));
        for(int i=0; i<arr.size(); i++)
        {
            arr[i] = rand() % 1000;
            arr2[i] = arr[i];
        }

         // Sequential Sort
        auto startSeq = chrono::high_resolution_clock::now();
        seq_bub(arr);
        auto endSeq = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> durationSeq = endSeq - startSeq;

        
        

        // Output Results
        cout << "\n----- Sequential Bubble Sort -----\n";
        cout << "Time Taken: " << durationSeq.count() << " ms\n";
        // printArray(arr);

        // Parallel Sort
        auto startPar = chrono::high_resolution_clock::now();
        par_bub(arr2);
        auto endPar = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> durationPar = endPar - startPar;
        
        cout << "\n----- Parallel Bubble Sort -----\n";
        cout << "Time Taken: " << durationPar.count() << " ms\n";
        // printArray(arr2);
    }

    


}