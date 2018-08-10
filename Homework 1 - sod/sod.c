#include <stdio.h>
#include <stdlib.h>

int main(int ac, char* argv[]) {
    if (ac != 2) {
        printf("sod DIGITS\n");
        return 1;
    }
    
    int upTo = atoi(argv[1]);
    int sum = 0;
    
    for (int i = 1; i <= upTo; i++) {
        sum += i;
    }
    printf("%i\n",sum);
    return 0;
}