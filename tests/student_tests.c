
#include <stddef.h>
#include "unit_tests.h"
#include "hw7.h"

//Brandon Moy
//116187802


// Helper functions for running my tests

matrix_sf* make_test_matrix(char name, unsigned int row, unsigned int col, int vals[]) {
    matrix_sf *mat = malloc(sizeof(matrix_sf) + (row * col * sizeof(int)));
    mat->name = name;
    mat->num_rows = row;
    mat->num_cols = col;
    for (unsigned int i = 0; i < row * col; i++) {
        mat->values[i] = vals[i];
    }
    return mat;
}

matrix_sf* make_dummy_matrix(char name) {
    matrix_sf *mat = malloc(sizeof(matrix_sf) + (1 * sizeof(int)));
    mat->name = name;
    mat->num_rows = 1;
    mat->num_cols = 1;
    mat->values[0] = 0; 
    return mat;
}

bst_sf* setup_test_bst() {
    int v1[] = {1, 2, 3, 4}; 
    int v2[] = {5, 6, 7, 8}; 
    bst_sf *root = insert_bst_sf(make_test_matrix('A', 2, 2, v1), NULL);
    root = insert_bst_sf(make_test_matrix('B', 2, 2, v2), root);
    return root;
}

void write_script(char *filename, char *content) {
    FILE *fp = fopen(filename, "w");
    fprintf(fp, "%s", content);
    fclose(fp);
}


// 10 tests for add_mats_sf

// 1. simple matrix addition of two numbers
Test(matrix_add, scalar) {
    int v1[] = {42}, v2[] = {-12};
    matrix_sf *a = make_test_matrix('A', 1, 1, v1), *b = make_test_matrix('B', 1, 1, v2);
    matrix_sf *res = add_mats_sf(a, b);
    cr_assert_eq(res->values[0], 30);
    free(a); free(b); free(res);
}

// 2. matrix addition with a 0 matrix
Test(matrix_add, identity) {
    int v1[] = {1, 2, 3, 4}, v2[] = {0, 0, 0, 0};
    matrix_sf *a = make_test_matrix('A', 2, 2, v1), *b = make_test_matrix('B', 2, 2, v2);
    matrix_sf *res = add_mats_sf(a, b);
    for(int i=0; i<4; i++) cr_assert_eq(res->values[i], v1[i]);
    free(a); free(b); free(res);
}

// 3. testing the communative property of matrix addition
Test(matrix_add, communative) {
    int v1[] = {1, 5, 2, 8}, v2[] = {3, -2, 4, 1};
    matrix_sf *a = make_test_matrix('A', 2, 2, v1), *b = make_test_matrix('B', 2, 2, v2);
    matrix_sf *res1 = add_mats_sf(a, b), *res2 = add_mats_sf(b, a);
    for(int i=0; i<4; i++) cr_assert_eq(res1->values[i], res2->values[i]);
    free(a); free(b); free(res1); free(res2);
}

//4. testing matrix addition with all negative values
Test(matrix_add, all_negatives) {
    int v1[] = {-1, -1}, v2[] = {-5, -10};
    matrix_sf *a = make_test_matrix('A', 2, 1, v1), *b = make_test_matrix('B', 2, 1, v2);
    matrix_sf *res = add_mats_sf(a, b);
    cr_assert_eq(res->values[0], -6); cr_assert_eq(res->values[1], -11);
    free(a); free(b); free(res);
}

// 5. Testing marix addition with an all 1's matrix
Test(matrix_add, one_vector) {
    int v1[] = {1, 2, 3, 4, 5}, v2[] = {1, 1, 1, 1, 1};
    matrix_sf *a = make_test_matrix('A', 5, 1, v1), *b = make_test_matrix('B', 5, 1, v2);
    matrix_sf *res = add_mats_sf(a, b);
    for(int i=0; i<5; i++) cr_assert_eq(res->values[i], i+2);
    free(a); free(b); free(res);
}

// 6. Testing matrix addition with an all 5's matrix
Test(matrix_add, five_vector) {
    int v1[] = {10, 20, 30, 40, 50}, v2[] = {5, 5, 5, 5, 5};
    matrix_sf *a = make_test_matrix('A', 1, 5, v1), *b = make_test_matrix('B', 1, 5, v2);
    matrix_sf *res = add_mats_sf(a, b);
    cr_assert_eq(res->values[4], 55);
    free(a); free(b); free(res);
}

//7. Testing matrix addition with inverse matrixes
Test(matrix_add, inverse) {
    int v1[] = {100, -50}, v2[] = {-100, 50};
    matrix_sf *a = make_test_matrix('A', 1, 2, v1), *b = make_test_matrix('B', 1, 2, v2);
    matrix_sf *res = add_mats_sf(a, b);
    cr_assert_eq(res->values[0], 0); cr_assert_eq(res->values[1], 0);
    free(a); free(b); free(res);
}

// 8. Testing matrix addition with a 3x4
Test(matrix_add, rectangular_3x4) {
    int v1[12], v2[12]; 
    for(int i=0; i<12; i++) { v1[i] = i; v2[i] = i*2; }
    matrix_sf *a = make_test_matrix('A', 3, 4, v1), *b = make_test_matrix('B', 3, 4, v2);
    matrix_sf *res = add_mats_sf(a, b);
    cr_assert_eq(res->values[11], 33);
    free(a); free(b); free(res);
}

// 9. Matrix addition with repeated values
Test(matrix_add, repeated_sixseven) {
    int v1[] = {6, 6, 6, 6}, v2[] = {7, 7, 7, 7};
    matrix_sf *a = make_test_matrix('A', 2, 2, v1), *b = make_test_matrix('B', 2, 2, v2);
    matrix_sf *res = add_mats_sf(a, b);
    for(int i=0; i<4; i++) cr_assert_eq(res->values[i], 13);
    free(a); free(b); free(res);
}

// 10. Matrix addition with large positive and negative
Test(matrix_add, large_negative) {
    int v1[] = {1000000}, v2[] = {-2000000};
    matrix_sf *a = make_test_matrix('A', 1, 1, v1), *b = make_test_matrix('B', 1, 1, v2);
    matrix_sf *res = add_mats_sf(a, b);
    cr_assert_eq(res->values[0], -1000000);
    free(a); free(b); free(res);
}

// mult_mats_sf 10 test cases

// 1. Multiplication with identity matrix
Test(matrix_mult, identity) {
    int v1[] = {1, 2, 3, 4}, v_id[] = {1, 0, 0, 1};
    matrix_sf *a = make_test_matrix('A', 2, 2, v1), *i = make_test_matrix('I', 2, 2, v_id);
    matrix_sf *res = mult_mats_sf(a, i);
    for(int j=0; j<4; j++) cr_assert_eq(res->values[j], v1[j]);
    free(a); free(i); free(res);
}

//2. Mattrix multiplication with a zero matrix
Test(matrix_mult, zero) {
    int v1[] = {1, 2, 3, 4}, v_zero[] = {0, 0, 0, 0};
    matrix_sf *a = make_test_matrix('A', 2, 2, v1), *z = make_test_matrix('Z', 2, 2, v_zero);
    matrix_sf *res = mult_mats_sf(a, z);
    for(int j=0; j<4; j++) cr_assert_eq(res->values[j], 0);
    free(a); free(z); free(res);
}

//3. Matrix multiplication with a 2x3 and 3x1 -> 2x1
Test(matrix_mult, rect_2x3_3x1) {
    int v1[] = {1, 2, 3, 4, 5, 6}, v2[] = {1, 1, 1};
    matrix_sf *a = make_test_matrix('A', 2, 3, v1), *b = make_test_matrix('B', 3, 1, v2);
    matrix_sf *res = mult_mats_sf(a, b);
    cr_assert_eq(res->num_rows, 2); cr_assert_eq(res->num_cols, 1);
    cr_assert_eq(res->values[0], 6); cr_assert_eq(res->values[1], 15);
    free(a); free(b); free(res);
}

//4. Matrix multiplcation with dot product
Test(matrix_mult, dot_product) {
    int v1[] = {1, 2, 3}, v2[] = {1, 2, 3};
    matrix_sf *a = make_test_matrix('A', 1, 3, v1), *b = make_test_matrix('B', 3, 1, v2);
    matrix_sf *res = mult_mats_sf(a, b);
    cr_assert_eq(res->values[0], 14);
    free(a); free(b); free(res);
}

//5. Matrix multiplication with 
Test(matrix_mult, outer_product) {
    int v1[] = {1, 2, 3}, v2[] = {1, 2, 3};
    matrix_sf *a = make_test_matrix('A', 3, 1, v1), *b = make_test_matrix('B', 1, 3, v2);
    matrix_sf *res = mult_mats_sf(a, b);
    cr_assert_eq(res->num_rows, 3); cr_assert_eq(res->num_cols, 3);
    cr_assert_eq(res->values[4], 4);
    cr_assert_eq(res->values[8], 9);
    free(a); free(b); free(res);
}

//6. scalar matrix multiplication
Test(matrix_mult, scalar) {
    int v1[] = {10}, v2[] = {20};
    matrix_sf *a = make_test_matrix('A', 1, 1, v1), *b = make_test_matrix('B', 1, 1, v2);
    matrix_sf *res = mult_mats_sf(a, b);
    cr_assert_eq(res->values[0], 200);
    free(a); free(b); free(res);
}

// 7. Testing the communative property for matrix multiplication
Test(matrix_mult, non_communative) {
    int v1[] = {1, 2, 3, 4}, v2[] = {5, 6, 7, 8};
    matrix_sf *a = make_test_matrix('A', 2, 2, v1), *b = make_test_matrix('B', 2, 2, v2);
    matrix_sf *res_ab = mult_mats_sf(a, b), *res_ba = mult_mats_sf(b, a);
    cr_assert_neq(res_ab->values, res_ba->values);
    free(a); free(b); free(res_ab); free(res_ba);
}

//8. Testing matrix multipication with
Test(matrix_mult, chain_dim) {
    int v1[] = {1,1,1,1, 1,1,1,1}, v2[] = {2,2, 2,2, 2,2, 2,2};
    matrix_sf *a = make_test_matrix('A', 2, 4, v1), *b = make_test_matrix('B', 4, 2, v2);
    matrix_sf *res = mult_mats_sf(a, b);
    for(int i=0; i<4; i++) cr_assert_eq(res->values[i], 8);
    free(a); free(b); free(res);
}


//9. negative values
Test(matrix_mult, negatives) {
    int v1[] = {-1, 2}, v2[] = {3, -4};
    matrix_sf *a = make_test_matrix('A', 1, 2, v1), *b = make_test_matrix('B', 2, 1, v2);
    matrix_sf *res = mult_mats_sf(a, b);
    cr_assert_eq(res->values[0], -11);
    free(a); free(b); free(res);
}

//10. Large values
Test(matrix_mult, large_vals) {
    int v1[] = {1000, 1000}, v2[] = {1000, 1000};
    matrix_sf *a = make_test_matrix('A', 1, 2, v1), *b = make_test_matrix('B', 2, 1, v2);
    matrix_sf *res = mult_mats_sf(a, b);
    cr_assert_eq(res->values[0], 2000000);
    free(a); free(b); free(res);
}

// transpose_mat_sf 10 test cases

//1. Transpose of a 2x2
Test(matrix_trans, square_2x2) {
    int v[] = {1, 2, 3, 4};
    matrix_sf *a = make_test_matrix('A', 2, 2, v);
    matrix_sf *res = transpose_mat_sf(a);
    cr_assert_eq(res->values[1], 3); cr_assert_eq(res->values[2], 2);
    free(a); free(res);
}

//2. 3x3
Test(matrix_trans, square_3x3) {
    int v[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    matrix_sf *a = make_test_matrix('A', 3, 3, v);
    matrix_sf *res = transpose_mat_sf(a);
    cr_assert_eq(res->values[1], 4); cr_assert_eq(res->values[3], 2);
    free(a); free(res);
}

//3. One row to one column
Test(matrix_trans, row_to_col) {
    int v[] = {1, 2, 3, 4, 5};
    matrix_sf *a = make_test_matrix('A', 1, 5, v);
    matrix_sf *res = transpose_mat_sf(a);
    cr_assert_eq(res->num_rows, 5); cr_assert_eq(res->num_cols, 1);
    free(a); free(res);
}

//4. One column to one row
Test(matrix_trans, col_to_row) {
    int v[] = {10, 20, 30};
    matrix_sf *a = make_test_matrix('A', 3, 1, v);
    matrix_sf *res = transpose_mat_sf(a);
    cr_assert_eq(res->num_rows, 1); cr_assert_eq(res->num_cols, 3);
    free(a); free(res);
}

//5. Scalar test
Test(matrix_trans, scalar) {
    int v[] = {777};
    matrix_sf *a = make_test_matrix('A', 1, 1, v);
    matrix_sf *res = transpose_mat_sf(a);
    cr_assert_eq(res->values[0], 777);
    free(a); free(res);
}

//6. Long matrix
Test(matrix_trans, long) {
    int v[] = {1, 2, 3, 4, 5, 6};
    matrix_sf *a = make_test_matrix('A', 6, 1, v);
    matrix_sf *res = transpose_mat_sf(a);
    cr_assert_eq(res->num_rows, 1); cr_assert_eq(res->num_cols, 6);
    free(a); free(res);
}

//7. Wide matrix
Test(matrix_trans, wide) {
    int v[] = {1, 2, 3, 4, 5, 6};
    matrix_sf *a = make_test_matrix('A', 2, 3, v);
    matrix_sf *res = transpose_mat_sf(a);
    cr_assert_eq(res->values[1], 4); cr_assert_eq(res->values[3], 5);
    free(a); free(res);
}

//8. Identity matrix
Test(matrix_trans, identity) {
    int v_id[] = {1, 0, 0, 1};
    matrix_sf *i = make_test_matrix('I', 2, 2, v_id);
    matrix_sf *res = transpose_mat_sf(i);
    for(int j=0; j<4; j++) cr_assert_eq(res->values[j], v_id[j]);
    free(i); free(res);
}

//9. zero matrix
Test(matrix_trans, zero) {
    int v_z[] = {0, 0, 0, 0, 0, 0};
    matrix_sf *z = make_test_matrix('Z', 3, 2, v_z);
    matrix_sf *res = transpose_mat_sf(z);
    for(int j=0; j<6; j++) cr_assert_eq(res->values[j], 0);
    free(z); free(res);
}

//10. Symmetric matrix
Test(matrix_trans, symmetric) {
    int v[] = {1, 2, 2, 1};
    matrix_sf *a = make_test_matrix('A', 2, 2, v);
    matrix_sf *res = transpose_mat_sf(a);
    for(int j=0; j<4; j++) cr_assert_eq(res->values[j], v[j]);
    free(a); free(res);
}

// bst_insert 10 test cases

Test(bst_insert, root) {
    matrix_sf *m = make_dummy_matrix('M');
    bst_sf *root = insert_bst_sf(m, NULL);
    cr_assert_not_null(root);
    cr_assert_eq(root->mat->name, 'M');
    free_bst_sf(root);
}

Test(bst_insert, insert_left) {
    bst_sf *root = insert_bst_sf(make_dummy_matrix('M'), NULL);
    root = insert_bst_sf(make_dummy_matrix('A'), root);
    cr_assert_not_null(root->left_child);
    cr_assert_eq(root->left_child->mat->name, 'A');
    free_bst_sf(root);
}

Test(bst_insert, insert_right) {
    bst_sf *root = insert_bst_sf(make_dummy_matrix('M'), NULL);
    root = insert_bst_sf(make_dummy_matrix('Z'), root);
    cr_assert_not_null(root->right_child);
    cr_assert_eq(root->right_child->mat->name, 'Z');
    free_bst_sf(root);
}

Test(bst_insert, spine_left) {
    bst_sf *root = insert_bst_sf(make_dummy_matrix('C'), NULL);
    root = insert_bst_sf(make_dummy_matrix('B'), root);
    root = insert_bst_sf(make_dummy_matrix('A'), root);
    cr_assert_not_null(root->left_child->left_child);
    free_bst_sf(root);
}

Test(bst_insert, spine_right) {
    bst_sf *root = insert_bst_sf(make_dummy_matrix('A'), NULL);
    root = insert_bst_sf(make_dummy_matrix('B'), root);
    root = insert_bst_sf(make_dummy_matrix('C'), root);
    cr_assert_not_null(root->right_child->right_child);
    free_bst_sf(root);
}

Test(bst_insert, zigzag_left) {
    bst_sf *root = insert_bst_sf(make_dummy_matrix('M'), NULL);
    root = insert_bst_sf(make_dummy_matrix('G'), root);
    root = insert_bst_sf(make_dummy_matrix('J'), root);
    cr_assert_not_null(root->left_child->right_child);
    cr_assert_eq(root->left_child->right_child->mat->name, 'J');
    free_bst_sf(root);
}

Test(bst_insert, zigzag_right) {
    bst_sf *root = insert_bst_sf(make_dummy_matrix('A'), NULL);
    root = insert_bst_sf(make_dummy_matrix('P'), root);
    root = insert_bst_sf(make_dummy_matrix('L'), root);
    cr_assert_not_null(root->right_child->left_child);
    cr_assert_eq(root->right_child->left_child->mat->name, 'L');
    free_bst_sf(root);
}

Test(bst_insert, balanced_order) {
    bst_sf *root = insert_bst_sf(make_dummy_matrix('M'), NULL);
    root = insert_bst_sf(make_dummy_matrix('A'), root);
    root = insert_bst_sf(make_dummy_matrix('Z'), root);
    cr_assert_not_null(root->left_child);
    cr_assert_not_null(root->right_child);
    free_bst_sf(root);
}

Test(bst_insert, alpha_boundary) {
    bst_sf *root = insert_bst_sf(make_dummy_matrix('B'), NULL);
    root = insert_bst_sf(make_dummy_matrix('A'), root);
    cr_assert_eq(root->left_child->mat->name, 'A');
    free_bst_sf(root);
}

Test(bst_insert, deep_tree) {
    bst_sf *root = NULL;
    char names[] = {'E', 'D', 'C', 'B', 'A'};
    for(int i=0; i<5; i++) root = insert_bst_sf(make_dummy_matrix(names[i]), root);
    bst_sf *curr = root;
    for(int i=0; i<4; i++) curr = curr->left_child;
    cr_assert_eq(curr->mat->name, 'A');
    free_bst_sf(root);
}

//bst_find 10 test cases

Test(bst_find, find_in_null) {
    cr_assert_null(find_bst_sf('A', NULL));
}

Test(bst_find, find_root) {
    bst_sf *root = insert_bst_sf(make_dummy_matrix('M'), NULL);
    matrix_sf *res = find_bst_sf('M', root);
    cr_assert_not_null(res);
    cr_assert_eq(res->name, 'M');
    free_bst_sf(root);
}

Test(bst_find, find_left_leaf) {
    bst_sf *root = insert_bst_sf(make_dummy_matrix('M'), NULL);
    root = insert_bst_sf(make_dummy_matrix('A'), root);
    matrix_sf *res = find_bst_sf('A', root);
    cr_assert_eq(res->name, 'A');
    free_bst_sf(root);
}

Test(bst_find, find_right_leaf) {
    bst_sf *root = insert_bst_sf(make_dummy_matrix('A'), NULL);
    root = insert_bst_sf(make_dummy_matrix('Z'), root);
    matrix_sf *res = find_bst_sf('Z', root);
    cr_assert_eq(res->name, 'Z');
    free_bst_sf(root);
}

Test(bst_find, missing_smaller) {
    bst_sf *root = insert_bst_sf(make_dummy_matrix('B'), NULL);
    cr_assert_null(find_bst_sf('A', root));
    free_bst_sf(root);
}

Test(bst_find, missing_larger) {
    bst_sf *root = insert_bst_sf(make_dummy_matrix('Y'), NULL);
    cr_assert_null(find_bst_sf('Z', root));
    free_bst_sf(root);
}

Test(bst_find, missing_between) {
    bst_sf *root = insert_bst_sf(make_dummy_matrix('M'), NULL);
    root = insert_bst_sf(make_dummy_matrix('A'), root);
    root = insert_bst_sf(make_dummy_matrix('Z'), root);
    cr_assert_null(find_bst_sf('G', root));
    free_bst_sf(root);
}

Test(bst_find, find_in_zigzag) {
    bst_sf *root = insert_bst_sf(make_dummy_matrix('M'), NULL);
    root = insert_bst_sf(make_dummy_matrix('G'), root);
    root = insert_bst_sf(make_dummy_matrix('J'), root);
    matrix_sf *res = find_bst_sf('J', root);
    cr_assert_eq(res->name, 'J');
    free_bst_sf(root);
}

Test(bst_find, find_deep) {
    bst_sf *root = NULL;
    for(char c='A'; c<='E'; c++) root = insert_bst_sf(make_dummy_matrix(c), root);
    matrix_sf *res = find_bst_sf('E', root);
    cr_assert_eq(res->name, 'E');
    free_bst_sf(root);
}

Test(bst_find, empty_search) {
    bst_sf *root = NULL;
    cr_assert_null(find_bst_sf('X', root));
}

// bst_free 10 test cases

Test(bst_free, free_null) {
    free_bst_sf(NULL);
}

Test(bst_free, free_single) {
    bst_sf *root = insert_bst_sf(make_dummy_matrix('A'), NULL);
    free_bst_sf(root);
}

Test(bst_free, free_left_spine) {
    bst_sf *root = NULL;
    root = insert_bst_sf(make_dummy_matrix('C'), root);
    root = insert_bst_sf(make_dummy_matrix('B'), root);
    root = insert_bst_sf(make_dummy_matrix('A'), root);
    free_bst_sf(root);
}

Test(bst_free, free_right_spine) {
    bst_sf *root = NULL;
    root = insert_bst_sf(make_dummy_matrix('A'), root);
    root = insert_bst_sf(make_dummy_matrix('B'), root);
    root = insert_bst_sf(make_dummy_matrix('C'), root);
    free_bst_sf(root);
}

Test(bst_free, free_balanced) {
    bst_sf *root = insert_bst_sf(make_dummy_matrix('M'), NULL);
    root = insert_bst_sf(make_dummy_matrix('A'), root);
    root = insert_bst_sf(make_dummy_matrix('Z'), root);
    free_bst_sf(root);
}

Test(bst_free, free_zigzag) {
    bst_sf *root = insert_bst_sf(make_dummy_matrix('M'), NULL);
    root = insert_bst_sf(make_dummy_matrix('G'), root);
    root = insert_bst_sf(make_dummy_matrix('J'), root);
    free_bst_sf(root);
}

Test(bst_free, free_alt_zigzag) {
    bst_sf *root = insert_bst_sf(make_dummy_matrix('A'), NULL);
    root = insert_bst_sf(make_dummy_matrix('Z'), root);
    root = insert_bst_sf(make_dummy_matrix('G'), root);
    free_bst_sf(root);
}

Test(bst_free, free_large) {
    bst_sf *root = NULL;
    for(char c='A'; c<='Z'; c++) root = insert_bst_sf(make_dummy_matrix(c), root);
    free_bst_sf(root);
}

Test(bst_free, free_after_search) {
    bst_sf *root = insert_bst_sf(make_dummy_matrix('M'), NULL);
    find_bst_sf('M', root);
    free_bst_sf(root);
}

Test(bst_free, stability_check) {
    for(int i=0; i<10; i++) {
        bst_sf *root = insert_bst_sf(make_dummy_matrix('X'), NULL);
        free_bst_sf(root);
    }
}

// create_matrix_sf 10 test cases

// 1. Basic 1x1
Test(create_mat, basic_1x1) {
    char *expr = "1 1 [ 5 ; ]";
    matrix_sf *m = create_matrix_sf('A', expr);
    cr_assert_eq(m->num_rows, 1);
    cr_assert_eq(m->num_cols, 1);
    cr_assert_eq(m->values [0], 5);
    free(m);
}

// 2. Negative Value
Test(create_mat, negative) {
    char *expr = "1 1 [ -10 ; ]";
    matrix_sf *m = create_matrix_sf('A', expr);
    cr_assert_eq(m->values [0], -10);
    free(m);
}

// 3. Excess Spacing
Test(create_mat, excess_spacing) {
    char *expr = "   2   1   [ 10 ; 20 ; ]";
    matrix_sf *m = create_matrix_sf('B', expr);
    cr_assert_eq(m->num_rows, 2);
    cr_assert_eq(m->values [1], 20);
    free(m);
}

// 4. No Spacing around Semicolon
Test(create_mat, semicolon_spacing) {
    char *expr = "1 2 [1 2;]";
    matrix_sf *m = create_matrix_sf('C', expr);
    cr_assert_eq(m->num_cols, 2);
    cr_assert_eq(m->values [1], 2);
    free(m);
}

// 5. Multi-digit Numbers
Test(create_mat, multi_digit) {
    char *expr = "1 1 [ 12345 ; ]";
    matrix_sf *m = create_matrix_sf('D', expr);
    cr_assert_eq(m->values [0], 12345);
    free(m);
}

// 6. 2x2 Identity 
Test(create_mat, identity_format) {
    char *expr = "2 2 [ 1 0 ; 0 1 ; ]";
    matrix_sf *m = create_matrix_sf('I', expr);
    cr_assert_eq(m->values [3], 1);
    free(m);
}

// 7. 3x1
Test(create_mat, column_3x1) {
    char *expr = "3 1 [ 1 ; 2 ; 3 ; ]";
    matrix_sf *m = create_matrix_sf('V', expr);
    cr_assert_eq(m->num_rows, 3);
    cr_assert_eq(m->values [2], 3);
    free(m);
}

// 8. 1x3
Test(create_mat, row_1x3) {
    char *expr = "1 3 [ 10 20 30 ; ]";
    matrix_sf *m = create_matrix_sf('R', expr);
    cr_assert_eq(m->num_cols, 3);
    cr_assert_eq(m->values [2], 30);
    free(m);
}

// 9. Weird spacing
Test(create_mat, weird_spacing) {
    char *expr = "1 2 [   50       60;]";
    matrix_sf *m = create_matrix_sf('M', expr);
    cr_assert_eq(m->values [1], 60);
    free(m);
}

// 10.  2x4
Test(create_mat, rect_2x4) {
    char *expr = "2 4 [ 1 2 3 4 ; 5 6 7 8 ; ]";
    matrix_sf *m = create_matrix_sf('Z', expr);
    cr_assert_eq(m->num_rows, 2);
    cr_assert_eq(m->num_cols, 4);
    cr_assert_eq(m->values [7], 8);
    free(m);
}

// infix2postifx 10 test cases

// 1. One operation
Test(postfix, one_op) {
    char *res = infix2postfix_sf("A");
    cr_assert_str_eq(res, "A");
    free(res);
}

// 2. Addition
Test(postfix, simple_add) {
    char *res = infix2postfix_sf("A+B");
    cr_assert_str_eq(res, "AB+");
    free(res);
}

// 3. Multiplication
Test(postfix, simple_mult) {
    char *res = infix2postfix_sf("A*B");
    cr_assert_str_eq(res, "AB*");
    free(res);
}

// 4. Transpose
Test(postfix, trans) {
    char *res = infix2postfix_sf("A'");
    cr_assert_str_eq(res, "A'");
    free(res);
}

// 5. Add and mult
Test(postfix, add_mult) {
    char *res = infix2postfix_sf("A+B*C");
    cr_assert_str_eq(res, "ABC*+");
    free(res);
}

// 6. Tranpose and mult
Test(postfix, trans_mult) {
    char *res = infix2postfix_sf("A'*B");
    cr_assert_str_eq(res, "A'B*");
    free(res);
}

// 7. Parentheses
Test(postfix, parentheses) {
    char *res = infix2postfix_sf("(A+B)*C");
    cr_assert_str_eq(res, "AB+C*");
    free(res);
}

// 8. Mix of operands
Test(postfix, mix) {
    char *res = infix2postfix_sf("(A+B)'*C");
    cr_assert_str_eq(res, "AB+'C*");
    free(res);
}

// 9. Spaces
Test(postfix, spaces) {
    char *res = infix2postfix_sf(" A  +  B' ");
    cr_assert_str_eq(res, "AB'+");
    free(res);
}

// 10. Nested parentheses with transpose
Test(postfix, nested_trans) {
    char *res = infix2postfix_sf("((A+B)*C)'");
    cr_assert_str_eq(res, "AB+C*'");
    free(res);
}

//evaluate_expr_sf 10 test cases 

Test(evaluate, single_lookup) {
    bst_sf *root = setup_test_bst();
    matrix_sf *res = evaluate_expr_sf('Z', "A", root);
    cr_assert_eq(res->values [0], 1);
    cr_assert_eq(res->name, 'Z');
    free_bst_sf(root); free(res);
}

Test(evaluate, simple_addition) {
    bst_sf *root = setup_test_bst();
    matrix_sf *res = evaluate_expr_sf('Z', "A+B", root);
    cr_assert_eq(res->values [0], 6); // 1+5
    free_bst_sf(root); free(res);
}

Test(evaluate, transpose) {
    bst_sf *root = setup_test_bst();
    matrix_sf *res = evaluate_expr_sf('Z', "A'", root);
    cr_assert_eq(res->values [1], 3); // index 1 is Row0,Col1. A' has A there.
    free_bst_sf(root); free(res);
}

Test(evaluate, mult) {
    bst_sf *root = setup_test_bst();
    matrix_sf *res = evaluate_expr_sf('Z', "A*B", root);
    cr_assert_eq(res->values [0], 19);
    free_bst_sf(root); free(res);
}

Test(evaluate, mult_add) {
    bst_sf *root = setup_test_bst();
    matrix_sf *res = evaluate_expr_sf('Z', "A+A*B", root);
    cr_assert_eq(res->values [0], 20); 
    free_bst_sf(root); free(res);
}

Test(evaluate, parens) {
    bst_sf *root = setup_test_bst();
    matrix_sf *res = evaluate_expr_sf('Z', "(A+A)*B", root);
    cr_assert_eq(res->values [0], 38); 
    free_bst_sf(root); free(res);
}

Test(evaluate, harder_transpose) {
    bst_sf *root = setup_test_bst();
    matrix_sf *res = evaluate_expr_sf('Z', "(A+B)'", root);
    cr_assert_eq(res->values [1], 10); 
    free_bst_sf(root); free(res);
}

Test(evaluate, mult_same) {
    bst_sf *root = setup_test_bst();
    matrix_sf *res = evaluate_expr_sf('Z', "A+A+A", root);
    cr_assert_eq(res->values [0], 3);
    free_bst_sf(root); free(res);
}

Test(evaluate, space_complex) {
    bst_sf *root = setup_test_bst();
    matrix_sf *res = evaluate_expr_sf('Z', "  ( A + B ) ' * A ", root);
    cr_assert_not_null(res);
    free_bst_sf(root); free(res);
}

Test(evaluate, double_transpose) {
    bst_sf *root = setup_test_bst();
    matrix_sf *res = evaluate_expr_sf('Z', "A''", root);
    cr_assert_eq(res->values [1], 2); 
    free_bst_sf(root); free(res);
}

Test(script, single_line) {
    write_script("test1.txt", "A = 1 1 [ 10 ; ]\n");
    matrix_sf *res = execute_script_sf("test1.txt");
    cr_assert_eq(res->values [0], 10);
    free(res);
}

Test(script, simple_eval) {
    write_script("test2.txt", "A = 1 1 [ 5 ; ]\nB = A + A\n");
    matrix_sf *res = execute_script_sf("test2.txt");
    cr_assert_eq(res->values [0], 10);
    cr_assert_eq(res->name, 'B');
    free(res);
}

Test(script, comments) {
    write_script("test3.txt", "# This is a comment\nA = 1 1 [ 1 ; ]\n# Another one\nB = A\n");
    matrix_sf *res = execute_script_sf("test3.txt");
    cr_assert_eq(res->values [0], 1);
    free(res);
}

Test(script, whitespace) {
    write_script("test4.txt", "   A   =   1   1   [   7   ;   ]   \n");
    matrix_sf *res = execute_script_sf("test4.txt");
    cr_assert_eq(res->values [0], 7);
    free(res);
}

Test(script, complex) {
    write_script("test5.txt", "A = 1 1 [ 2 ; ]\nB = 1 1 [ 3 ; ]\nC = A * B + A\n");
    matrix_sf *res = execute_script_sf("test5.txt");
    cr_assert_eq(res->values [0], 8); 
    free(res);
}

Test(script, transpose_chain) {
    write_script("test6.txt", "A = 2 1 [ 1 ; 2 ; ]\nB = A'\n");
    matrix_sf *res = execute_script_sf("test6.txt");
    cr_assert_eq(res->num_rows, 1);
    cr_assert_eq(res->num_cols, 2);
    free(res);
}

Test(script, reuse_matrix) {
    write_script("test7.txt", "A = 1 1 [ 1 ; ]\nB = A + A\nC = B + A\n");
    matrix_sf *res = execute_script_sf("test7.txt");
    cr_assert_eq(res->values [0], 3);
    free(res);
}

Test(script, multi_line_rectangular) {
    write_script("test8.txt", "A = 2 2 [ 1 0 ; 0 1 ; ]\nB = 2 2 [ 5 6 ; 7 8 ; ]\nC = A * B\n");
    matrix_sf *res = execute_script_sf("test8.txt");
    cr_assert_eq(res->values [0], 5);
    free(res);
}

Test(script, empty_lines) {
    write_script("test9.txt", "\n\nA = 1 1 [ 9 ; ]\n\n\n");
    matrix_sf *res = execute_script_sf("test9.txt");
    cr_assert_eq(res->values [0], 9);
    free(res);
}

Test(script, return_last_named) {
    write_script("test10.txt", "A = 1 1 [ 1 ; ]\nB = 1 1 [ 2 ; ]\n");
    matrix_sf *res = execute_script_sf("test10.txt");
    cr_assert_eq(res->name, 'B');
    free(res);
}