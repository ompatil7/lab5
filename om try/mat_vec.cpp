#include<bits/stdc++.h>
#include<omp.h>
#include<cstdlib>
#include<ctime>
using namespace std;
// #define MAX 100
void seq_multiply(const vector<vector<int>> a, vector<int> &x, vector<int> &res, int n, int m)
{
    for(int i=0; i<n; i++)
    {
        res[i] = 0;
        for(int j=0; j<m; j++)
        {
            res[i] += a[i][j] * x[j];
        }
    }
}
void par_multiply(const vector<vector<int>> a, vector<int> &x, vector<int> &res, int n, int m)
{
    #pragma omp parallel for
    for(int i=0; i<n; i++)
    {
        res[i] = 0;
        for(int j=0; j<m; j++)
        {
            res[i] += a[i][j] * x[j];
        }
    }
}

int main()
{
    int n, m;
    cout << "Enter n and m : ";
    cin >> n >> m;
    vector<int> resPar(m, 0), resSeq(m, 0);
    // int a[MAX][MAX];
    vector<vector<int>> a(n, vector<int> (m));
    srand(time(0));

    for(int i=0; i<n; i++)
    {
        for(int j=0; j<m; j++)
        {
            a[i][j] = rand() % 100000;
        } 
    }

    vector<int> x(m);
    for(int i=0; i<m; i++)
    {
        x[i] = rand() % 1000;
    }

    cout<< "OP " << endl;
    auto start = chrono::high_resolution_clock::now();
    seq_multiply(a, x, resSeq, n, m);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> dur_seq = end - start;

    cout << "Seq time " << dur_seq.count() << endl;

    cout<< "OP " << endl;
    auto start2 = chrono::high_resolution_clock::now();
    par_multiply(a, x, resPar, n, m);
    auto end2 = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> dur_seq2 = end2 - start2;

    cout << "Par time " << dur_seq2.count() << endl;


}