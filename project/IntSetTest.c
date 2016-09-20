/*
 * File: IntSetTest.c
 * Creator: George Ferguson
 * Created: Wed Sep  7 17:58:24 2016
 * Time-stamp: <Wed Sep  7 17:59:21 EDT 2016 ferguson>
 *
 * Test program for IntSet library.
 */

#include <stdio.h>
#include "IntSet.h"

void
myfunc(int value) {
    printf("myfunc: %d\n", value);
}

int
main(int argc, char **argv) {
    IntSet *set = IntSet_new();

    printf("set contains 2? %d\n", IntSet_contains(set, 2));

    IntSet_add(set, 1);
    IntSet_add(set, 2);
    IntSet_add(set, 3);
    printf("set = ");
    IntSet_print(set);

    printf("set contains 2? %d\n", IntSet_contains(set, 2));

    IntSet_iterate(set, myfunc);

    IntSet *set2 = IntSet_new();
    IntSet_add(set2, 3);
    printf("set2 = ");
    IntSet_print(set2);
    printf("set contains_all set2? %d\n", IntSet_contains_all(set, set2));
    printf("set equals set2? %d\n", IntSet_equals(set, set2));
    IntSet_add(set2, 2);
    IntSet_add(set2, 1);
    printf("set2 = ");
    IntSet_print(set2);
    printf("set equals set2? %d\n", IntSet_equals(set, set2));

    IntSet_free(set);
    IntSet_free(set2);
}
