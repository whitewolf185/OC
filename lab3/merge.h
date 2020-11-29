#ifndef OC_MERGE_H
#define OC_MERGE_H
#include "vector.h"
void merge(vector* data, int l, int r){
    if(l == r){
        return;
    }

    int m = (l + r) / 2;

    merge(data,l,m);
    merge(data,m+1, r);

    int middle[data->size];
    //create(&middle,0);
    int lhs=l,rhs = m+1;
    int count = 0;
    while(lhs != m+1 && rhs != r+1){
        if(data->data[lhs] < data->data[rhs]){
            //push_back(&middle,data->data[lhs]);
            middle[count] = data->data[lhs];
            ++count;
            lhs++;
        }
        else{
            //push_back(&middle,data->data[rhs]);
            middle[count] = data->data[rhs];
            ++count;
            rhs++;
        }
    }

    if(lhs != m + 1){
        for (int i = lhs; i <= m; ++i) {
//            push_back(&middle,data->data[i]);
            middle[count] = data->data[i];
            ++count;
        }
    }
    else if(rhs != r + 1){
        for (int i = rhs; i <= r; ++i) {
            //push_back(&middle,data->data[i]);
            middle[count] = data->data[i];
            ++count;
        }
    }

//    if(middle.size == 2) {
//        print_all(&middle);
//    }

    for (int i = l; i <= r; ++i) {
        data->data[i] = middle[i-l];
    }
    //destroy(&middle);
}
#endif //OC_MERGE_H
