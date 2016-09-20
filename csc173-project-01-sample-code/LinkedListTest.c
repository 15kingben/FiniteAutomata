/*
 * File: LinkedListTest.c
 * Creator: George Ferguson
 * Created: Wed Sep  7 17:59:30 2016
 * Time-stamp: <Wed Sep  7 18:01:41 EDT 2016 ferguson>
 *
 * Test program for LinkedList library.
 */

#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"

int
main(int argc, char **argv) {
    LinkedList *list = LinkedList_new();
    LinkedList_add_at_end(list, "foo");
    LinkedList_add_at_end(list, "bar");
    LinkedList_add_at_end(list, "baz");
    LinkedList_print_string_list(list);
    LinkedList_add_at_front(list, "Ted");
    // Print with library function
    LinkedList_print_string_list(list);
    // Print with iterator
    LinkedListIterator *iterator = LinkedList_iterator(list);
    while (LinkedListIterator_has_next(iterator)) {
	void *data = LinkedListIterator_next(iterator);
	char *str = (char*)data;
	printf("%s\n", str);
    }
    free(iterator);
    // Test remove in middle
    LinkedList_remove(list, "bar");
    LinkedList_print_string_list(list);
    // Test remove at head
    LinkedList_remove(list, "Ted");
    LinkedList_print_string_list(list);
    // Test remove in end
    LinkedList_remove(list, "baz");
    LinkedList_print_string_list(list);
    // Test remove only element
    void *elt = LinkedList_element_at(list, 0);
    printf("element 0 is \"%s\"\n", (char*)elt);
    LinkedList_remove(list, elt);
    LinkedList_print_string_list(list);
    printf("is_empty: %d\n", LinkedList_is_empty(list));
}
