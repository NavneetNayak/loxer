#ifndef ERROR_H
#define ERROR_H

void frontend_error(const char *message);
void error(int line, char *message);
void report(int line, char *where, char *message);

#endif 
