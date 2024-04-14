#include "../include/error.h"
#include "../include/scanner.h"
#include "../include/hashmap.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

static int is_at_end ();
static void scan_token();
static char advance();
static Token * init_tokens ();
static void add_token (Token_type type, Literal literal);
static int match (char expected);
static char peek ();
static void display_tokens ();
static void string ();
static void number ();
static void parse_double ();
static char peek_next ();
static void identifier ();

//TODO: Create a toString function that returns token info

char * source;
Token * tokens;

static int source_len;

static int num_tokens = 0;

static int start = 0;
static int current = 0;
static int line = 1;

Token * scan_tokens (char * buf) {
    init_hashmap();
    add_to_map("and", AND);
    add_to_map("class", CLASS);
    add_to_map("else", ELSE);
    add_to_map("false", FALSE);
    add_to_map("for", FOR);
    add_to_map("fun", FUN);
    add_to_map("if", IF);
    add_to_map("nil", NIL);
    add_to_map("or", OR);
    add_to_map("print", PRINT);
    add_to_map("return", RETURN);
    add_to_map("super", SUPER);
    add_to_map("this", THIS);
    add_to_map("true", TRUE);
    add_to_map("var", VAR);
    add_to_map("while", WHILE);

    init_tokens();

    /* Copying buf into a extern variable source to make it accessible to all functions
    in this file */
    source_len = strlen(buf);
    if (!(source = (char *)malloc((source_len + 1) * sizeof(char)))) {
        error(0, "Interpreter Allocation Error.");
    }
    strcpy(source, buf);

    while (!is_at_end()) {
        /* Set new start point as current after a token is added to the list*/
        start = current;

        scan_token();
    }

    display_tokens();
    
    return tokens;
}

/* Check if end of source file is reached*/
static int is_at_end () {
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

        case '!':
        add_token(match('=') ? BANG_EQUAL : BANG, (Literal){NULL});
        break;
        case '=':
        add_token(match('=') ? EQUAL_EQUAL : EQUAL, (Literal){NULL});
        break;
        case '<':
        add_token(match('=') ? LESS_EQUAL : LESS, (Literal){NULL});
        break;
        case '>':
        add_token(match('=') ? GREATER_EQUAL : GREATER, (Literal){NULL});
        break;

        case '/':
        if (match('/')) {
            while (peek() != '\n' && !is_at_end()) advance();
        }
        else {
            add_token(SLASH, (Literal){NULL});
        }
        break;

        case ' ':
        case '\r':
        case '\t':
        break;

        case '\n':
        line++;
        break;

        case '"':
        string();
        break;

        default: 
        if (isdigit(c)) {
            number();
        }
        else if (isalpha(c) || c == '_') {
            identifier();
        }
        else {
            error(line, "Unexpected Character."); 
        }
        break;
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

    /* To store current token's data */
    Token * new_token = malloc(sizeof(Token));
    if (new_token == NULL) {
        error(0, "Interpreter Allocation Error");
        free(lexeme);
    }

    new_token->type = type;
    new_token->literal = literal;
    new_token->lexeme = lexeme;
    new_token->line = line;

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

/* Initialize tokens array, so that it is not empty*/
static Token * init_tokens () {
    Token * tokens = malloc(sizeof(Token));

    if (tokens == NULL) {
        error(0, "Interpreter Allocation Error");
    }

    return tokens;
}

/* Combine peek and advance (Only advance if certain character
is next) */
static int match (char expected) {
    char c = peek();
    if (c != '\0' && c != expected) return 0;
    advance();
    return 1;
}

static char peek () {
    if (is_at_end()) return '\0';
    return source[current];
}

static void display_tokens () {
    return;
    for (int i = 0; i < num_tokens; i++) {
        fprintf(stdout, "%s %d\n", tokens[i].lexeme, tokens[i].type);
    }
}

static void string () {
    while (peek() != '"' && !is_at_end()) {
        if (peek() == '\n') line++;
        advance();
    }

    if (is_at_end()) {
        error(line, "Unterminated String.");
        return;
    }

    advance();

    int str_len = current - start - 1;
    char * str;

    if (!(str = (char *)malloc(str_len * sizeof(char)))) {
        error(0, "Interpreter Allocation Error");
        return;
    }

    strncpy (str, &source[start+1], str_len - 1);
    str[str_len] = '\0';

    add_token (STRING, (Literal){.string = str});
}

static void number () {
    while (isdigit(peek())) advance();

    if (peek() == '.' && isdigit(peek_next())) {
        advance();

        while (isdigit(peek())) advance();
    }

    parse_double();
}

static void parse_double () {
    double int_part = 0;
    double decimal_part = 0;
    int after_decimal = 0;
    int num_decimal = 0;

    for (int i = start; i < current; i++) {
        if (source[i] == '.') {
            after_decimal = 1;
            continue;
        }
        else if (after_decimal){
            num_decimal++;
            decimal_part = (decimal_part * 10) + (source[i] - '0');
        }
        else {
            int_part = (int_part * 10) + (source[i] - '0');
        }
    }
    double num = int_part + (decimal_part / pow(10, num_decimal));

    add_token(NUMBER, (Literal){.number = num});
}

static char peek_next () {
    if (is_at_end()) return '\0';
    return source[current+1];
}

static void identifier () {
    while (isalnum(peek()) || peek() == '_') advance();

    char * lexeme;
    int lexeme_size = current - start + 1;

    if (!(lexeme = (char *)malloc(lexeme_size * sizeof(char)))) {
        error(0, "Interpreter Allocation Error");
    }

    strncpy(lexeme, &source[start], lexeme_size - 1);
    lexeme[lexeme_size - 1] = '\0';

    struct bucket * list = search_map(lexeme);

    if (list == NULL) {
        add_token(IDENTIFIER, (Literal)NULL);
    }
    else if (list != NULL) {
        add_token(list->val, (Literal)NULL);
    }
}
