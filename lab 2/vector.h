#ifndef VECTOR_H
#define VECTOR_H

#include "stdlib.h"
#include "stdio.h"

typedef struct vector {
    int *data;//тут надо подправить тип
    int size;
} vector;

void create(vector *v, int size) {
    v->data = (int *) malloc(sizeof(int) * size);//тут надо подправить тип
    v->size = size;
    int i;
    for (i = 0; i < size; ++i) { //если используете в виде данных структуру, желательно убрать этот for или
        v->data[i] = '\0';           //сделать свою инициализацию
    }
}

void push_back(vector *v, const int x) {//тут надо подправить тип
    v->data = (int *) realloc(v->data, sizeof(int) * (v->size + 1));//тут надо подправить тип
    v->data[v->size] = x;
    v->size++;
}

void pop(vector *v) {
    v->data = (int *) realloc(v->data, sizeof(int) * (v->size - 1));//тут надо подправить тип
    v->size--;
}

void print_all(vector *v) {
    int i;
    for (i = 0; i < v->size; ++i) {
        printf("%d ", v->data[i]);
    }
    printf("\n");
}

int size(vector *v) {
    if (v == NULL) {
        return 0;
    }
    return v->size;
}

void destroy(vector *v) {
    v->size = 0;
    free(v->data);
}

#endif

