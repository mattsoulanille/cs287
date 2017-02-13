//Matthew Soulanille
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "dartsmodule.c"

// Multithreaded
int main(int argc, char **argv) {
  //  srand(time(NULL));

  if (argc > 3) {
    // Number of darts to throw
    long count = strtol(argv[1], NULL, 10);
    double accuracy = strtod(argv[2], NULL);
    // Where to throw them at
    long target = strtol(argv[3], NULL, 10);

    if ( (target != 1) && (target != 20) ) {
      printf("Target must be 1 or 20\n");
      return -1;
    }

    // Uses multithreaded darts
    double averageScore = darts_parallel(count, accuracy, target, 4);
    printf("Average score:\t%f\n", averageScore);
    return 0;
  }
  else {
    printf("Usage: darts \e[4mcount\e[24m \e[4maccuracy\e[24m \e[4mtarget\e[24m\n\t\e[4mcount\e[24m: How many darts to throw\n\t\e[4maccuracy\e[24m: A double that scales the generated numbers\n\t\e[4mtarget\e[24m: Where to throw them (1 or 20) .\n");
    //debug();
    return -1;

  }
  
  /*
  double p[2];
  double *polar = toPolar(pos, p);
  printf("(%f, %f) is (%f, %f) in polar\n", pos[0], pos[1], polar[0], polar[1]);
  */
  //scorePos(pos);


}

