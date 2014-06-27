/* list.c
 * 
 * This is a simple implementation of a list with dynamic capacity in C. I must
 * note that this is just me fiddling around with C. I am certanly not an
 * expert nor do I use C on a daily basis or have ever extensively used it in
 * the past, so there may be some harmful things in this code. Use it with
 * caution. If you feel the need to correct this code somehow, please do so,
 * I'd be grateful.
 *
 * Copyright (c) 2014 Lukas Henkel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int* data;
    unsigned int len;
    unsigned int cap;
} List;

// Allocate a new List with the given capacity.
// Returns NULL if list could not be allocated.
List* List_new(unsigned int capacity) {
    void* listptr = malloc(sizeof(List));
    if(listptr == NULL) {
        return NULL;
    }
    List* list = (List*)listptr;
    list->cap = capacity;
    list->len = 0;
    void* dataptr = malloc(sizeof(int)*capacity);
    if(dataptr == NULL) {
        free(listptr);
        return NULL;
    }
    list->data = (int*)dataptr;
    return list;
}

// Change the capacity of the given list.
// If the new capacity is smaller than the current one, the list will be cut off.
// Returns 0 on error and 1 on success.
int List_change_capacity(List* list, unsigned int new_capacity) {
    int* old_data = list->data;
    void* ptr = malloc(sizeof(int)*new_capacity);
    if(ptr == NULL) {
        return 0;
    }
    list->data = (int*)ptr;
    int len = list->len;
    if(new_capacity < len) {
        len = new_capacity;
    }
    memcpy(list->data, old_data, sizeof(int)*len);
    free(old_data);
    list->cap = new_capacity;
    list->len = len;
    return 1;
}

// Adds an item to the given list.
// May double the capacity if needed.
// Returns 0 on error and 1 on success.
int List_add(List* list, int item) {
    if(list->cap == list->len) {
        int success = List_change_capacity(list, list->cap*2);
        if(!success) {
            return 0;
        }
    }
    list->data[list->len] = item;
    list->len++;
    return 1;
}

// Removes n numer of items at the given index from the given list.
// Pass -1 as the index to remove from the end. 
// Returns 0 on error and 1 on success.
int List_remove(List* list, unsigned int i, unsigned int n) {
    // `at` can't be bigger than our lists length.
    // We also can't remove more elements than our lists holds.
    if(i >= list->len || list->len < n) {
        return 0;
    }
    // We only need this if we are not removing from the end.
    if(i != -1 && list->len != i+n) {
        int* destptr = &(list->data[i]);
        int block_start = i+n;
        int* srcptr = &(list->data[block_start]);
        memcpy(destptr, srcptr, sizeof(int)*(list->len-block_start));
    }
    list->len -= n;
    unsigned int halfcap = list->cap/2;
    if(list->len <= halfcap) {
        List_change_capacity(list, halfcap);
    }
    return 1;
}

// Copies the value found at the given index to the passed pointer.
// Returns 0 on error and 1 on success.
int List_get(List* list, unsigned int i, int* value_ptr) {
    if(i >= list->len) {
        return 0;
    }
    *value_ptr = list->data[i];
    return 1;
}

// Sets the value at the given index.
// Returns 0 on error and 1 on success.
int List_set(List* list, unsigned int i, int new_value) {
    if(i >= list->len) {
        return 0;
    }
    list->data[i] = new_value;
    return 1;
}

// Prints the given list to stdout.
// Example:
//     Length: 4
//     Capacity: 16
//     374
//     2954
//     294
//     2843
void List_print(List* list) {
    printf("Length: %u\n", list->len);
    printf("Capacity: %u\n", list->cap);
    for(int i = 0; i < list->len; i++) {
        printf("%i\n", list->data[i]);
    }
}

// Releases all resources of the given list.
void List_free(List* list) {
    free(list->data);
    free(list);
}

int main() {
    List* list = List_new(128);
    if(list == NULL) {
        printf("Error creating list. Abort.\n");
        return EXIT_FAILURE;
    }
    for(int i = 0; i < 1024; i++) {
        if(!List_add(list, i)) {
            printf("Error adding item. Abort.\n");
            return EXIT_FAILURE;
        }
    }
    List_remove(list, 0, 512);
    int val;
    if(!List_get(list, 0, &val)) {
        printf("Getting ai index 0 failed. Abort.\n");
        return EXIT_FAILURE;
    }
    printf("list[0] == %i\n", val);
    if(!List_set(list, 0, 0)) {
        printf("Setting at index 0 failed. Abort.\n");
        return EXIT_FAILURE;
    }
    if(!List_get(list, 0, &val)) {
        printf("Getting after setting at index 0 failed. Abort.\n");
        return EXIT_FAILURE;
    }
    printf("New value: list[0] == %i\n", val);
    List_print(list);
    List_free(list);
    return EXIT_SUCCESS;
}
