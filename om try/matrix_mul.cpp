#include <bits/stdc++.h>
#include<omp.h>
#include<chrono>
#include <cstdlib>   // For srand() and rand()
#include <ctime>     // For time()
using namespace std;
// #define MAX 800

void seq_mul(const vector<vector<int>> &A,const vector<vector<int>> &B, vector<vector<int>> &C, int n, int m, int p)
{
    // Initialize C to 0
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            C[i][j] = 0;

    for(int i=0;i<n;i++)
    {
        for(int j=0; j<m; j++)
        {
            for(int k=0; k<p; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}





void par_mul(const vector<vector<int>> &A, const vector<vector<int>> &B, vector<vector<int>> &C, int n, int m, int p)
{

    // Initialize C to 0
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            C[i][j] = 0;

    #pragma omp parallel for schedule(static)
    for(int i=0;i<n;i++)
    {
        for(int j=0; j<m; j++)
        {
            for(int k=0; k<p; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}


void printMatrix(const vector<vector<int>> M, int n, int m) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j)
            cout << M[i][j] << " ";
        cout << endl;
    }
}


int main()
{
    int n, m, p;

    // int A[MAX][MAX], B[MAX][MAX], c_seq[MAX][MAX], c_par[MAX][MAX] = {};
    

    cout << "Enter dim \n";
    cout << "Enter n : ";
    cin >> n;
    cout << "Enter m : ";
    cin >> m;
    cout << "Enter p : ";
    cin >> p;

    vector<vector<int>> A(n, vector<int>(p));
    vector<vector<int>> B(p, vector<int>(m));
    vector<vector<int>> c_seq(n, vector<int>(m, 0));
    vector<vector<int>> c_par(n, vector<int>(m, 0));

    srand(time(0));
    cout << "Enter mat A : \n";
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<p; j++)
        {
            A[i][j] = rand() % 500000;
        }
    }

    cout << "Enter mat B : \n";
    for(int i=0; i<p; i++)
    {
        for(int j=0; j<m; j++)
        {
            
            B[i][j] = rand() % 500000;
        }
    }

    
    

    // printMatrix(c_seq, n, m);

    auto start = chrono::high_resolution_clock::now();
    seq_mul(A, B, c_seq, n, m, p);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration_seq = end - start;

    auto start2 = chrono::high_resolution_clock::now();
    par_mul(A, B, c_par, n, m, p);
    auto end2 = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration_par = end2 - start2;

    cout << "\n----- Sequential Matrix-Matrix Multiplication Output -----\n";
    // printMatrix(c_seq, n, m);
    cout << "Time Taken: " << duration_seq.count() << " milliseconds\n";

    cout << "\n----- Parallel Matrix-Matrix Multiplication Output -----\n";
    // printMatrix(c_par, n, m);
    cout << "Time Taken: " << duration_par.count() << " milliseconds\n";


    // printMatrix(A, n, p);
    // cout << endl;
    // printMatrix(B, p, m);
}