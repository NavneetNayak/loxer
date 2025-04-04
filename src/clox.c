#include <stdio.h>
#include <stdlib.h>
#include "../include/error.h"
#include "../include/scanner.h"

char * read_file (const char * path);

void run_file (const char * path);
void run_prompt ();
void run (char * source);

/* User input code: */
int main (int argc, char ** argv) {
    if (argc < 2) {
        printf("Usage: clox [script]");
        exit(EXIT_FAILURE);
    }
    else if (argc == 2) {
        run_file(argv[1]);
    }
    else {
        run_prompt();
    }
    
    return EXIT_SUCCESS;
}

/* Reading file code: */
char * read_file (const char * path) {
    FILE * file = NULL;
    if (!(file = fopen(path, "rb"))) {
        fprintf(stderr, "Could not open file. %s\n", path);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0L, SEEK_END);

    size_t file_size = ftell(file);
    rewind(file);

    char * buffer = NULL;
    if (!(buffer = (char *) malloc(file_size + 1))) {
        frontend_error("File buffer allocation error");
    }

    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read < file_size) {
        frontend_error("Could not read file");
    }

    buffer[bytes_read] = '\0';

    fclose(file);
    return buffer;

}

/* Running lox file code: */
void run_file (const char * path) {
    char * source = read_file(path);
    run(source);
}

void run_prompt () {
    //todo
}

void run (char * source) {
    scan_tokens(source);
}
