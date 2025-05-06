#include <iostream>
#include <vector>
#include <cstdlib>    // For rand() and srand()
#include <ctime>      // For time()
#include <cmath>      // For fabs
#include <omp.h>

using namespace std;

// Sequential matrix-vector multiplication.
// Computes: y = A * x
//   A: matrix of size M x N
//   x: vector of size N
//   y: result vector of size M
void sequentialMatrixVectorMultiply(const vector<vector<double>>& A,
                                      const vector<double>& x,
                                      vector<double>& y,
                                      int M, int N) {
    for (int i = 0; i < M; i++) {
        double sum = 0.0;
        for (int j = 0; j < N; j++) {
            sum += A[i][j] * x[j];
        }
        y[i] = sum;
    }
}

// Parallel matrix-vector multiplication using OpenMP.
// Here the outer loop over the rows is parallelized.
void parallelMatrixVectorMultiply(const vector<vector<double>>& A,
                                    const vector<double>& x,
                                    vector<double>& y,
                                    int M, int N) {
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < M; i++) {
        double sum = 0.0;
        for (int j = 0; j < N; j++) {
            sum += A[i][j] * x[j];
        }
        y[i] = sum;
    }
}

int main() {
    // Set the dimensions for the matrix and vector.
    int M = 1000;  // Number of rows in matrix A (and size of result vector y)
    int N = 1000;  // Number of columns in matrix A (and size of vector x)

    // Initialize random number generator.
    srand(static_cast<unsigned int>(time(0)));

    // Generate matrix A with dimensions M x N.
    vector<vector<double>> A(M, vector<double>(N));
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = static_cast<double>(rand()) / RAND_MAX;
        }
    }
    
    // Generate vector x with N elements.
    vector<double> x(N);
    for (int j = 0; j < N; j++) {
        x[j] = static_cast<double>(rand()) / RAND_MAX;
    }
    
    // Allocate result vectors for both sequential and parallel operations.
    vector<double> y_seq(M, 0.0);
    vector<double> y_par(M, 0.0);
    
    // --------- Sequential Matrix-Vector Multiplication ----------
    double startSeq = omp_get_wtime();
    sequentialMatrixVectorMultiply(A, x, y_seq, M, N);
    double endSeq = omp_get_wtime();
    double timeSeq = endSeq - startSeq;
    
    // --------- Parallel Matrix-Vector Multiplication ----------
    double startPar = omp_get_wtime();
    parallelMatrixVectorMultiply(A, x, y_par, M, N);
    double endPar = omp_get_wtime();
    double timePar = endPar - startPar;
    
    // Print the execution times.
    cout << "Sequential Matrix-Vector Multiplication Time: " << timeSeq << " seconds." << endl;
    cout << "Parallel Matrix-Vector Multiplication Time:   " << timePar << " seconds." << endl;
    
    // Verify that the sequential and parallel results match.
    bool match = true;
    for (int i = 0; i < M; i++) {
        if (fabs(y_seq[i] - y_par[i]) > 1e-9) {  // use tolerance for floating-point comparison
            match = false;
            break;
        }
    }
    cout << "Result matrices " << (match ? "match." : "do not match!") << endl;
    
    /*
    Input/Usage Description:
    ------------------------
    - Matrix A (dimensions M x N) and vector x (size N) are randomly generated.
    - The result vector y (size M) is computed by performing matrix-vector multiplication.
    - Sequential and parallel execution times are measured and compared.
    
    Compilation Command:
    --------------------
    g++ -fopenmp parallel_matrix_vector.cpp -o parallel_matrix_vector.exe
    
    Execution:
    -----------
    Run the executable to see a comparison of the sequential and parallel multiplication times.
    You can adjust M and N for larger or smaller matrices, and use the OMP_NUM_THREADS environment variable
    to control the number of threads.
    */
    
    return 0;
}
