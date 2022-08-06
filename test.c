#include  <stdio.h>
#include  <string.h>

int main()
{
    char src[20] = "hellooo";
    char dst[12] = "world";

    memmove(src + 2, src, 6);
    printf ("src = %s\n", src);
    // printf ("src = %s\n", src);
}