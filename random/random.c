//Matthew Soulanille
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv) {
  srand(time(NULL)); //seed 'random' number generator with time

  if (argc > 1) {
    clock_t begin = clock(); //start clock

    long count = strtol(argv[1], NULL, 10); //string to long

    int x;
    for (x = 0; x < count; x ++) {
      //make the random numbers
      double r = ((double) (rand()) / RAND_MAX)*100.0;
      printf("%d : %.12f\n", x, r);
    }

    clock_t end = clock(); //stop clock

    //calculate time it took
    double time = (double) (end - begin) / CLOCKS_PER_SEC;
    printf("time : %f\n", time);
  }
  else {
    //Print usage information if no args
    printf("Usage: random \e[4mcount\e[24m \n where \e[4mcount\e[24m is how many numbers to generate.\n");
    return 1;
  }
}
