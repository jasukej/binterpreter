#ifndef clox_value_h
#define clox_value_h

#include "common.h"

typedef double Value;

// A dynamic array in which we can lookup constants for a chunk
typedef struct {
    int capacity;
    int count;
    Value* values;
} ValueArray;

void initValueArray(ValueArray* arr);
void freeValueArray(ValueArray* arr);
void writeValueArray(ValueArray* arr, Value value);
void printValue(Value value);

#endif