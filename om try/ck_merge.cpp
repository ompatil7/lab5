#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace chrono;

void merge(vector<int> &arr, int left, int mid, int right)
{
    vector<int> temp;
    int i = left, j = mid + 1;

    while (i <= mid && j <= right)
    {
        // 1
        if (arr[i] <= arr[j])
            temp.push_back(arr[i++]);
        else
            temp.push_back(arr[j++]);
    }

    while (i <= mid)
        temp.push_back(arr[i++]);
    while (j <= right)
        temp.push_back(arr[j++]);

    for (int k = 0; k < temp.size(); k++)
        arr[left + k] = temp[k];
}

void sequentialMergeSort(vector<int> &arr, int left, int right)
{
    if (left < right)
    {
        int mid = (left + right) / 2;
        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void parallelMergeSort(vector<int> &arr, int left, int right)
{
    if (left < right)
    {
        int mid = (left + right) / 2;

#pragma omp parallel sections
        {
#pragma omp section
            parallelMergeSort(arr, left, mid);

#pragma omp section
            parallelMergeSort(arr, mid + 1, right);
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
    sequentialMergeSort(arr, 0, n - 1);
    auto endSeq = high_resolution_clock::now();

    auto startPar = high_resolution_clock::now();
    parallelMergeSort(arrCopy, 0, n - 1);
    auto endPar = high_resolution_clock::now();

    duration<double> timeSeq = endSeq - startSeq;
    duration<double> timePar = endPar - startPar;

    cout << "Time for Sequential Merge Sort: " << timeSeq.count() << " seconds\n";
    cout << "Time for Parallel Merge Sort: " << timePar.count() << " seconds\n";

    return 0;
}