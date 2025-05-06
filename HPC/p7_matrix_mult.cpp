#include <bits/stdc++.h>
#include <vector>
#include <cstdlib>    // For rand() and srand()
#include <ctime>      // For time()
#include <omp.h>

using namespace std;

// Sequential matrix multiplication:
// Computes C = A x B, where A is M x K, B is K x N, and C is M x N.
void sequentialMatrixMultiply(const vector<vector<double>>& A,
                              const vector<vector<double>>& B,
                              vector<vector<double>>& C, int M, int K, int N) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int k = 0; k < K; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

// Parallel matrix multiplication using OpenMP.
// Parallelizes the outer loop over rows of A (and C).
void parallelMatrixMultiply(const vector<vector<double>>& A,
                            const vector<vector<double>>& B,
                            vector<vector<double>>& C, int M, int K, int N) {
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int k = 0; k < K; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

int main() {
    // Set the dimensions for the matrices.
    int M = 500;  // Number of rows in A and C.
    int K = 500;  // Number of columns in A and rows in B.
    int N = 500;  // Number of columns in B and C.

    // Seed the random number generator.
    srand(static_cast<unsigned int>(time(0)));

    // Initialize matrices A and B with random values (scaled to [0,1]).
    vector<vector<double>> A(M, vector<double>(K));
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            A[i][k] = static_cast<double>(rand()) / RAND_MAX;
        }
    }
    
    vector<vector<double>> B(K, vector<double>(N));
    for (int k = 0; k < K; k++) {
        for (int j = 0; j < N; j++) {
            B[k][j] = static_cast<double>(rand()) / RAND_MAX;
        }
    }
    
    // Prepare result matrices C_seq and C_par.
    vector<vector<double>> C_seq(M, vector<double>(N, 0));
    vector<vector<double>> C_par(M, vector<double>(N, 0));

    // --------- Sequential Matrix Multiplication ----------
    double seqStart = omp_get_wtime();
    sequentialMatrixMultiply(A, B, C_seq, M, K, N);
    double seqEnd = omp_get_wtime();
    double seqTime = seqEnd - seqStart;
    
    // --------- Parallel Matrix Multiplication ----------
    double parStart = omp_get_wtime();
    parallelMatrixMultiply(A, B, C_par, M, K, N);
    double parEnd = omp_get_wtime();
    double parTime = parEnd - parStart;
    
    // Output timing results.
    cout << "Sequential Matrix Multiplication Time: " << seqTime << " seconds." << endl;
    cout << "Parallel Matrix Multiplication Time:   " << parTime << " seconds." << endl;
    
    // Verify that both resulting matrices are identical.
    bool identical = true;
    for (int i = 0; i < M && identical; i++) {
        for (int j = 0; j < N && identical; j++) {
            // A tolerance is used due to floating-point arithmetic.
            if (abs(C_seq[i][j] - C_par[i][j]) > 1e-9) {
                identical = false;
            }
        }
    }
    
    cout << "Result matrices " << (identical ? "match." : "do not match!") << endl;
    
    /*
    Input/Usage Details:
    --------------------
    - Matrices A (MxK) and B (KxN) are randomly generated (values between 0 and 1).
    - C_seq stores the result of sequential multiplication.
    - C_par stores the result of parallel multiplication.
    
    Compilation Command (with OpenMP support):
    --------------------------------------------
    g++ -fopenmp parallel_matrix_multiply.cpp -o parallel_matrix_multiply.exe
    
    Execution:
    ----------
    Run the executable to see a comparison of the sequential and parallel execution times.
    
    Note:
    -----
    - For larger matrices (increase M, K, or N), you may observe a more significant speedup with parallelization.
    - You can control the number of threads with the OMP_NUM_THREADS environment variable.
    */
    
    return 0;
}
