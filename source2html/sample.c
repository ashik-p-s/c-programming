#include <stdio.h>
#include <stdlib.h>

/* Multi-line comment block
   Testing line breaks & HTML escaping: 5 < 10 && 10 > 2
*/

// Single-line comment: checking #preprocessor inside comment handling

#define MAX_BUFFER 1024
#define MESSAGE "Hello, World!\n"

struct Student {
    int id;
    float gpa;
};

int main(int argc, char *argv[]) {
    // Numeric constants (integers, floats, decimals)
    int count = 42;
    float pi = 3.14159;
    
    // Character and string literals with quotes & escapes
    char letter = 'A';
    char escaped_char = '\'';
    char *text = "Testing string with \"escaped quotes\" and HTML tags: <html>";

    // Keyword, identifier, and operator checks
    if (count > 0 && pi < 5.0) {
        printf("Count: %d, Pi: %f\n", count, pi);
    } else {
        printf("Condition failed!\n");
    }

    // Checking dynamic memory allocation keywords
    struct Student *s = (struct Student *)malloc(sizeof(struct Student));
    if (s != NULL) {
        s->id = 101;
        s->gpa = 3.85;
        free(s);
    }

    return 0;
}