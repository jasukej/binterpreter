#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"

typedef struct {
    Token current;
    Token previous;
    bool hadError;
    bool panicMode;
} Parser;

Parser parser;
Chunk* compilingChunk;

// TODO: Logic on chunk pointer to be returned for user-defined functions
static Chunk* currentChunk() {
    return compilingChunk;
}

static void errorAt(Token* token, const char* message) {
    if (parser.panicMode) return;
    parser.panicMode = true;
    fprintf(stderr, "[line %d] Error", token->line);

    if (token->type==TOKEN_EOF) {
        fprintf(stderr, " at end");
    } else if (token->type == TOKEN_ERROR) {

    } else {
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }

    fprintf(stderr, " at '%.*s'", token->length, token->start);
    parser.hadError = true;
}

static void errorAtCurrent(const char* message) {
    errorAt(&parser.current, message);
}

static void advance() {
    parser.previous = parser.current;

    // Report all errors recorded by the scanner
    for (;;) {
        parser.current = scanToken();
        if (parser.current.type != TOKEN_ERROR) break;

        errorAtCurrent(parser.current.start);
    }
}

static void consume(TokenType type, const char* message) {
    if (parser.current.type == type) {
        advance();
        return;
    }

    errorAtCurrent(message);
}

// Write a byte to our current chunk
static void emitByte(uint8_t byte) {
    writeChunk(currentChunk(), byte, parser.previous.line);
}

// Add the return to the end of a chunk to print the output
static void emitReturn() {
    emitByte(OP_RETURN);
}

// Write two bytes, typically forb writing opcode, then operand
static void emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

static void endCompiler() {
    emitReturn();
}

static void expression() {
    // TODO
}

// Performs single pass compilation to output bytecode
bool compile(const char* source, Chunk* chunk) {
    initScanner(source);
    compilingChunk = chunk;

    parser.hadError = false;
    parser.panicMode = false;

    advance();
    expression();
    consume(TOKEN_EOF, "Expect end of expression.");
    endCompiler();
    return !parser.hadError;
}
