#include <iostream>
#include "classes.h"

int main() {
    A a;
    a.check_proxy();
    int values_1[5] = {1, 2, 3, 4, 5};
    a.check_mediator(values_1);
    int values_2[5]= {6, 7, 8, 9, 0};
    a.check_mediator(values_2);
    return 0;
}
