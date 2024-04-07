#include "error.h"
#include "scanner.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static int is_at_end (int source_len);
static void scan_token();
static char advance();
static Token * init_tokens ();
static void add_token (Token_type type, Literal literal);

//TODO: Create a toString function that returns token info

char * source;
Token * tokens;

static int num_tokens = 1;

static int start = 0;
static int current = 0;
static int line = 1;

Token * scan_tokens (char * buf) {
    init_tokens();

    /* Copying buf into a extern variable source to make it accessible to all functions
    in this file */
    int source_len = strlen(buf);
    if (!(source = (char *)malloc((source_len + 1) * sizeof(char)))) {
        error(0, "Interpreter Allocation Error.");
    }
    strcpy(source, buf);

    while (!is_at_end(source_len)) {
        /* Set new start point as current after a token is added to the list*/
        start = current;

        scan_token();
    }

    return tokens;
}

/* Check if end of source file is reached*/
static int is_at_end (int source_len) {
    return current >= source_len;
}

static void scan_token () {
    char c = advance();

    switch (c) {
        case '(': add_token(LEFT_PAREN, (Literal){NULL}); break;
        case ')': add_token(RIGHT_PAREN, (Literal){NULL}); break;
        case '{': add_token(LEFT_BRACE, (Literal){NULL}); break;
        case '}': add_token(RIGHT_BRACE, (Literal){NULL}); break;
        case ',': add_token(COMMA, (Literal){NULL}); break;
        case '.': add_token(DOT, (Literal){NULL}); break;
        case '-': add_token(MINUS, (Literal){NULL}); break;
        case '+': add_token(PLUS, (Literal){NULL}); break;
        case ';': add_token(SEMICOLON, (Literal){NULL}); break;
        case '*': add_token(STAR, (Literal){NULL}); break;
    }
}

static char advance () {
    return source[current++];
}

/* Add current token to tokens array, token contains information like token type, 
literal, lexeme(text) */
static void add_token (Token_type type, Literal literal) {
    char * lexeme;
    int lexeme_size = current - start + 1;

    if (!(lexeme = (char *)malloc(lexeme_size * sizeof(char)))) {
        error(0, "Interpreter Allocation Error");
    }

    /* Copying text of token into lexeme */
    strncpy(lexeme, &source[start], lexeme_size - 1);
    lexeme[lexeme_size - 1] = '\0';

    Token * new_token = malloc(sizeof(Token));
    if (new_token == NULL) {
        error(0, "Interpreter Allocation Error");
        free(lexeme);
    }

    new_token->type = type;
    new_token->literal = literal;
    new_token->lexeme = lexeme;

    // int num_tokens = sizeof(tokens) / sizeof(Token);
    tokens = realloc(tokens, (num_tokens + 1) * sizeof(Token));
    if (tokens == NULL) {
        error(0, "Interpreter Allocation Error");
        free(new_token);
        free(lexeme);
    }

    tokens[num_tokens] = *new_token;
    num_tokens++;
    free(new_token);
}

static Token * init_tokens () {
    Token * tokens = malloc(sizeof(Token));

    if (tokens == NULL) {
        error(0, "Interpreter Allocation Error");
    }

    return tokens;
}
