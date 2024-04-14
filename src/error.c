#include <stdio.h>
#include "../include/error.h"

int had_error;

void error (int line, char * message) {
    //Write 'where' finding code
    report (line, "", message);
}

void report (int line, char * where, char * message) {
    fprintf(stderr, "Error: %d | %s | %s\n", line, where, message);
    had_error = 1; /* Instead of directly exiting, we can reset had_error for run_prompt*/
}