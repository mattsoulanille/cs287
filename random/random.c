#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv) {
  srand(time(NULL));

  if (argc > 1) {
    clock_t begin = clock();
    int x;

    printf("argv[1]: %s\n", argv[1]);
    long count = strtol(argv[1], NULL, 10);


    for (x = 0; x < count; x ++) {
      double r = ((double) (rand()) / RAND_MAX)*100.0;
      printf("%d : %.12f\n", x, r);
    }

    clock_t end = clock();
    
    double time = (double) (end - begin) / CLOCKS_PER_SEC;
    printf("time : %f\n", time);
  }
  else {
    printf("Usage: random \e[4mcount\e[24m \n where \e[4mcount\e[24m is how many numbers to generate.\n");
    return 1;
  }
}
