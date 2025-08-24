#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
  #define environ _environ
#endif

extern char** environ;

int main() {
    char** var;
    for (var = environ; *var != NULL; ++var) {
        printf("%s\n", *var);
    }
    return 0;
}