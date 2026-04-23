#include "unit_tests.h"

TestSuite(student_tests, .timeout=TEST_TIMEOUT); 


Test(student_tests, add_negatives_and_positives) {
    // Creating two 2x2 matrices
    // Matrix A: [-1, -2]
    //           [-3, -4]
    matrix_sf *A = copy_matrix(2, 2, (int[]){-1, -2, -3, -4});
    
    // Matrix B: [1, 2]
    //           [3, 4]
    matrix_sf *B = copy_matrix(2, 2, (int[]){1, 2, 3, 4});

    // Call your function
    matrix_sf *res = add_mats_sf(A, B);

    // Expected result is all zeros
    int expected_vals[] = {0, 0, 0, 0};
    expect_matrices_equal(res, 2, 2, expected_vals);

    // Crucial: Free everything for Valgrind!
    free(A);
    free(B);
    free(res);
}

Test(student_tests, multiply_by_identity) {
    // 2x2 Matrix A
    matrix_sf *A = copy_matrix(2, 2, (int[]){5, 10, 15, 20});
    
    // 2x2 Identity Matrix I
    matrix_sf *I = copy_matrix(2, 2, (int[]){1, 0, 0, 1});

    // Call your function
    matrix_sf *res = mult_mats_sf(A, I);

    // The result should be identical to Matrix A
    int expected_vals[] = {5, 10, 15, 20};
    expect_matrices_equal(res, 2, 2, expected_vals);

    // Clean up memory
    free(A);
    free(I);
    free(res);
}
