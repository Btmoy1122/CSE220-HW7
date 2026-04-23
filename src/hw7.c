#include "hw7.h"

bst_sf* insert_bst_sf(matrix_sf *mat, bst_sf *root) {
    if (root == NULL){
        bst_sf *new_node = malloc(sizeof(bst_sf));
        if (new_node == NULL) return NULL;

        new_node->mat = mat;
        new_node->left_child = NULL;
        new_node->right_child = NULL;
        return new_node;
    }

    if (mat->name < root->mat->name){
        root->left_child = insert_bst_sf(mat, root->left_child);
    }
    else{
        root->right_child = insert_bst_sf(mat, root->right_child);
    }
    return root;

}

matrix_sf* find_bst_sf(char name, bst_sf *root) {
    if (root == NULL) return NULL;

    if (root->mat->name == name){
        return root->mat;
    }

    if (name < root->mat->name){
        return find_bst_sf(name, root->left_child);
    }
    else{
        return find_bst_sf(name, root->right_child);
    }

}

void free_bst_sf(bst_sf *root) {
    if (root == NULL) return;
    
    free_bst_sf(root->left_child);
    free_bst_sf(root->right_child);

    if (root->mat != NULL){
        free(root->mat);
    }

    free(root);

}

//Add two matrixes
matrix_sf* add_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    unsigned int rows = mat1->num_rows;
    unsigned int cols = mat1->num_cols;

    matrix_sf *res = malloc(sizeof(matrix_sf) + (rows * cols * sizeof(int)));

    if (res == NULL) return NULL;

    res->name = '?';
    res->num_rows = rows;
    res->num_cols = cols;

    for (unsigned int i = 0; i< (rows * cols); i++){
        res->values[i] = mat1->values[i] + mat2->values[i];
    }
    return res;

}

matrix_sf* mult_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
   unsigned int m = mat1->num_rows;
   unsigned int n = mat1->num_cols; // cols of mat1 and rows of mat2
   unsigned int p = mat2->num_cols;

   //result matrix will have dimensions m by p (matrix1 rows by matrix 2 cols)
   matrix_sf *res = malloc(sizeof(matrix_sf) + (m * p * sizeof(int)));

   if (res == NULL) return NULL;

   res->name = '?';
   res->num_rows = m;
   res->num_cols = p;

    for (unsigned int i = 0; i< m; i++){
        for (unsigned int j = 0; j < p; j++){
            int sum = 0;
            for (unsigned int k = 0; k < n; k++){
                sum += mat1->values[(i*n) + k] * mat2->values[(k*p) + j];
            }
            res->values[(i * p) + j] = sum;
        }
    }
    return res;
}

matrix_sf* transpose_mat_sf(const matrix_sf *mat) {
    unsigned int rows = mat->num_rows;
    unsigned int cols = mat->num_cols;

    matrix_sf *res = malloc(sizeof(matrix_sf) + (rows * cols * sizeof(int)));

    res->name = '?';
    res->num_rows = cols;
    res->num_cols = rows;
    
    for (unsigned int i = 0; i < cols; i++){
        for (unsigned int j = 0; j < rows; j++){
            //res[i][j] = mat[j][i]
            res->values[(i * rows) + j] = mat->values[(j* cols) + i];

        }
    }

    return res;
    
}

matrix_sf* create_matrix_sf(char name, const char *expr) {
    unsigned int rows, cols;

    //Get dimensions from the two numbers at start of expr/string
    if (sscanf(expr, "%u %u", &rows, &cols) != 2){
        return NULL;
    }

    //Allocate memory for matrix
    matrix_sf *mat = malloc(sizeof(matrix_sf) + (rows * cols * sizeof(int)));

    mat->name = name;
    mat->num_rows = rows;
    mat->num_cols = cols;

    //Create ptr for start of values in exp for our matrix
    char *ptr = strchr(expr, '[');
    if (ptr == NULL){
        free(mat);
        return NULL;
    }
    ptr++;

    //Extract integers from exp for our matrix
    for (unsigned int i = 0; i < (rows * cols); i++){
        //Skip anything not a numbers
        while (*ptr && !isdigit(*ptr) && *ptr !='-'){
            ptr++;
        }
        mat->values[i] = (int)strtol(ptr, &ptr, 10);
    }
    return mat;

}

//Helper function to determine the weight of some operator
int weight(char o) {
    if (o == '\'') return 3;
    else if (o == '*') return 2;
    else if (o == '+') return 1;
    else return 0;
}
char* infix2postfix_sf(char *infix) {
    int len = strlen(infix);
    char *postfix = malloc(len+1);
    if (!postfix) return NULL;

    char stack[len];
    int last = -1;
    int k = 0;

    for (int i = 0; i<len; i++){
        char c = infix[i];

        //Check case by case
        if (isupper(c)){
            postfix[k] = c;
            k++;
        }
        else if(c == '\''){
            postfix[k] = c;
            k++;
        }
        else if (c == '('){
            last+=1;
            stack[last] = c;
        }
        else if (c == ')'){
            while (last > -1 && stack[last] != '('){
                postfix[k] = stack[last];
                k++;
                last--;
            }
            if (last > -1) last--;
        }
        else if (c == '+' || c == '*'){
            while (last > -1 && weight(stack[last]) >= weight(c)){
                postfix[k] = stack[last];
                k++;
                last--;
            }
            last++;
            stack[last] = c;
        }
    }

    // Add remaining operations
    while (last > -1){
        postfix[k] = stack[last];
        k++;
        last--;
    }
    postfix[k] = '\0';
    return postfix;
}

matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root) {
    //Steps
    //1. Convert the given infix exression to postfix
    //2. Create the stack of matrix pointers and do operations
    //3. Final cleanup and free
    
    //1. 
    char *postfix = infix2postfix_sf(expr);
    int len = strlen(postfix);

    //2.
    matrix_sf *stack[len];
    int last = -1;

    for (int i = 0; i < len; i++){
        char c = postfix[i];

        if (isupper(c)){
            matrix_sf *mat = find_bst_sf(c, root);
            matrix_sf *copy = copy_matrix(mat->num_rows, mat->num_cols,mat->values);
            copy->name = mat->name;
            last+=1;
            stack[last] = copy;
        }
        else if (c == '\''){
            matrix_sf *m = stack[last];
            last--;
            matrix_sf *res = transpose_mat_sf(m);
            res->name = '?';
            free(m);
            last+=1;
            stack[last] = res;
        }
        else if(c == '*'){
            matrix_sf *m2 = stack[last];
            last--;
            matrix_sf *m1 = stack[last];
            last--;

            matrix_sf *res = mult_mats_sf(m1, m2);
            res->name = '?';
            free(m1);
            free(m2);
            last+=1;
            stack[last] = res;
        }
        else if (c == '+'){
            matrix_sf *m1 = stack[last];
            last--;
            matrix_sf *m2 = stack[last];
            last--;

            matrix_sf *res = add_mats_sf(m1, m2);
            res->name = '?';
            free(m1);
            free(m2);
            last+=1;
            stack[last] = res;
        }      
    }
    matrix_sf *final = stack[last];
    last--;
    final->name = name;

    free(postfix);
    return final;
}

matrix_sf *execute_script_sf(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) return NULL;

    char line[1024];
    bst_sf *root = NULL;
    matrix_sf *mat = NULL;
    while (fgets(line, sizeof(line), fp)){
        if (line[0] == '\n' || line[0] == '#') continue;
        
        char *name_ptr = line;
        while (*name_ptr && !isupper(*name_ptr)){
            if(*name_ptr == '#') break;
            name_ptr++;
        }
        if (!*name_ptr || *name_ptr == '#') continue;
        char name = *name_ptr;
        

        char *eq = strchr(line, '=');
        if (eq == NULL) continue;

        char *instruction = eq + 1;

        while (isspace(*instruction)){
            instruction++;
        }
        
        if (isdigit(*instruction)){
            mat = create_matrix_sf(name, instruction);
        }
        else{
            mat = evaluate_expr_sf(name, instruction, root);
        }
        root = insert_bst_sf(mat, root);

    }
   fclose(fp);

   if (mat != NULL  && root != NULL){
    bst_sf *cur = root;
    while (cur != NULL){
        if (cur->mat->name == mat->name){
            cur->mat = NULL;
            break;
        }
        if(mat->name < cur->mat->name) cur = cur->left_child;
        else cur = cur->right_child;
    }
   }

   free_bst_sf(root);
   return mat;
    
}

// This is a utility function used during testing. Feel free to adapt the code to implement some of
// the assignment. Feel equally free to ignore it.
matrix_sf *copy_matrix(unsigned int num_rows, unsigned int num_cols, int values[]) {
    matrix_sf *m = malloc(sizeof(matrix_sf)+num_rows*num_cols*sizeof(int));
    m->name = '?';
    m->num_rows = num_rows;
    m->num_cols = num_cols;
    memcpy(m->values, values, num_rows*num_cols*sizeof(int));
    return m;
}

// Don't touch this function. It's used by the testing framework.
// It's been left here in case it helps you debug and test your code.
void print_matrix_sf(matrix_sf *mat) {
    assert(mat != NULL);
    assert(mat->num_rows <= 1000);
    assert(mat->num_cols <= 1000);
    printf("%d %d ", mat->num_rows, mat->num_cols);
    for (unsigned int i = 0; i < mat->num_rows*mat->num_cols; i++) {
        printf("%d", mat->values[i]);
        if (i < mat->num_rows*mat->num_cols-1)
            printf(" ");
    }
    printf("\n");
}
