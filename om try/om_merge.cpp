#include<bits/stdc++.h>
#include<ctime>
#include<omp.h>

using namespace std;
using namespace chrono;

void merge(vector<int> &arr, int left, int mid, int right)
{
    vector<int> temp;

    int i = left, j = mid+1;
    while(i <= mid && j<= right)
    {
        if(arr[i] < arr[j]) {
            temp.push_back(arr[i++]);
        }
        else {
            temp.push_back(arr[j++]);
        }
    }

    while(i <= mid) {
        temp.push_back(arr[i++]);
    }

    while(j <= right) {
        temp.push_back(arr[j++]);
    }

    for(int k=0; k<temp.size(); k++)
    {
        arr[left + k] = temp[k];
    }
}


void seq_merge(vector<int> &arr, int left, int right)
{
    if(left < right) {
        int mid = (left+right) / 2;
        seq_merge(arr, left, mid);
        seq_merge(arr, mid+1, right);
        merge(arr, left, mid, right);
    }
}

void par_merge(vector<int> &arr, int left, int right)
{
    if(left < right) {
        int mid = (left+right) / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            par_merge(arr, left, mid);

            #pragma omp section
            par_merge(arr, mid+1, right);
        }
        merge(arr, left, mid, right);
    }
}



int main()
{
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    srand(time(0));

    vector<int> arr(n), arrCopy(n);
    for (int i = 0; i < n; i++)
    {
        arr[i] = rand() % 1000;
        arrCopy[i] = arr[i];
    }

    auto startSeq = high_resolution_clock::now();
    seq_merge(arr, 0, n - 1);
    auto endSeq = high_resolution_clock::now();

    auto startPar = high_resolution_clock::now();
    par_merge(arrCopy, 0, n - 1);
    auto endPar = high_resolution_clock::now();

    duration<double> timeSeq = endSeq - startSeq;
    duration<double> timePar = endPar - startPar;

    cout << "Time for Sequential Merge Sort: " << timeSeq.count() << " seconds\n";
    cout << "Time for Parallel Merge Sort: " << timePar.count() << " seconds\n";

    return 0;
}