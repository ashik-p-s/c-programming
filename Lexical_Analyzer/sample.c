#include <stdio.h>

int main()
{
    int a = 10;
    float b = 12.5;
    char ch = 'A';

    if(a >= 10 && b != 0)
    {
        a += 5;
        a++;
    }
    else
    {
        a--;
    }

    printf("Hello Ashi");
    return 0;
}