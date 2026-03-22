#include <stdint.h>
#include <stdlib.h>
#include "chunk.h"
#include "memory.h"
#include "value.h"

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->linesCount = 0;
    chunk->linesCapacity = 0;
    initValueArray(&chunk->constants);
    chunk->lines = NULL;
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->count++;

    if (chunk->linesCount > 0 &&
        chunk->lines[chunk->linesCount - 1].line == line) {
        chunk->lines[chunk->linesCount - 1].count++;
        return;
    }

    if (chunk->linesCapacity < chunk->linesCount + 1) {
        int oldCapacity = chunk->linesCapacity;
        chunk->linesCapacity = GROW_CAPACITY(oldCapacity);
        chunk->lines = GROW_ARRAY(LineInfo, chunk->lines, oldCapacity,
            chunk->linesCapacity);
    }

    chunk->lines[chunk->linesCount].line = line;
    chunk->lines[chunk->linesCount].count = 1;
    chunk->linesCount++;
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(LineInfo, chunk->lines, chunk->linesCapacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

// Add a value to the constant table and return its index
int addConstant(Chunk* chunk, Value val) {
    writeValueArray(&chunk->constants,  val);
    return chunk->constants.count - 1;
}

// Given index of an instruction, return line where instruction occurs
int getLine(Chunk* chunk, int idx) {
    int instructionsSeen = 0;

    for (int i = 0; i < chunk->linesCount; i++) {
        instructionsSeen += chunk->lines[i].count;
        if (idx < instructionsSeen) {
            return chunk->lines[i].line;
        }
    }

    return -1;
}
