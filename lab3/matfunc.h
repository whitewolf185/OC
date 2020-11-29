#ifndef MATFUNC_H
#define MATFUNC_H
	
struct Matrix {
	int dimn;
	int dimm;
	int **matr;
};

void scanfMatrix(struct Matrix *a, int n, int m) { // Ввод матрицы
	a->dimn = n;
	a->dimm = m;
 	a->matr = (int**)malloc(sizeof(int*)*n);
	for (int i=0; i<n; i++) {
		a->matr[i] = (int*) malloc(sizeof(int)*m);
	}

	for (int i=0; i<n; i++) {
		for (int j=0; j<m; j++) {
			scanf("%d", &a->matr[i][j]);
		}
	}
}

/*int random(int seed) {
    seed * 123456;
}*/

void scanf_With_Fill_Rand_Matrix(struct Matrix *a, int n, int m) { // Ввод с заполнением матрицы
	a->dimn = n;
	a->dimm = m;
	a->matr = (int**)malloc(sizeof(int*)*n);
	for (int i=0; i<n; i++) {
		a->matr[i] = (int*) malloc(sizeof(int)*m);
	}
    srand(time(NULL));
	for (int i=0; i<n; i++) {
		for (int j=0; j<m; j++) {

            int num = rand() % 100;
			a->matr[i][j] = num;
		}
	}
}

void scanf_Matrix_with_Fill_0(struct Matrix* a, int n, int m){
    a->dimm = m;
    a->dimn = n;
    a->matr = (int**)malloc(sizeof(int*)*n);
    for (int i = 0; i < n; ++i) {
        a->matr[i] = (int*)malloc(sizeof(int)*m);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            a->matr[i][j] = 0;
        }
    }
}

void printMatrix(struct Matrix *a) { // Печать матрицы
	printf("\n");
	for (int i=0; i<a->dimn; i++) {
		for (int j=0; j<a->dimm; j++) {
			printf("%d ", a->matr[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void fillMatrix(struct Matrix *a, int num) { // Заполнение матрицы
	for (int i=0; i<a->dimn; i++) {
		for (int j=0; j<a->dimm; j++) {
			a->matr[i][j] = num;
		}
	}
}

void swap (struct Matrix *a, struct Matrix *b) { // Обмен ссылками на матрицы одинаковой размерности
    struct Matrix p; int **pp;
    p = *a; pp = a->matr;
    *a = *b; a->matr = b->matr;
    *b = p; b->matr = pp;
}

#endif