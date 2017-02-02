//Matthew Soulanille
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define SLICE_SIZE (M_PI/10.0)


double* toCartesian(double polar[], double* cartesian) {
  cartesian[0] = cos(polar[1]) * polar[0];
  cartesian[1] = sin(polar[1]) * polar[0];
  return cartesian;
}

double* toPolar(double pos[], double* polar) {
  //polar[radius, angle]
  polar[0] = sqrt(pos[0]*pos[0]+pos[1]*pos[1]);
  polar[1] = fmod( (atan2(pos[1], pos[0]) + 2*M_PI), (2*M_PI) );
  return polar;
}


double* randomNormal(double* outPos, double scale) {
  // generates a normally distributed (around 0,0) cartesian
  // point
  double r1 = ((double) rand()) / RAND_MAX;
  double r2 = ((double) rand()) / RAND_MAX;
  //printf("%f\n", r1);
  outPos[0] = (sqrt(-2*log(r1)) * cos(2*M_PI*r2)) * scale;
  outPos[1] = (sqrt(-2*log(r1)) * sin(2*M_PI*r2)) * scale;
  
  return outPos;
}


int scorePos(double pos[]) {
  //scores a position
  //pos must be cartesian
  double p[2];
  double *polar = toPolar(pos, p);

  // outer circle bullseye. 31.8 mm diameter 
  if (polar[0] < 15.9) {
    // inner circle bullsey. 12.7 mm diameter    
    if (polar[0] < 6.35) {
      return 50;
    }
    else {
      return 25;
    }      
  }
  else {

    
    //printf("scoring polar position: (%f, %f)\n", polar[0], polar[1]);
    int score_areas[] = {13,4,18,1,20,5,12,9,14,11,8,16,7,19,3,17,2,15,10,6};
    int score_address = (int) (polar[1] / SLICE_SIZE); // [0,2pi)->(13...6)

    int score_region = score_areas[score_address];
    //printf("score region: %i\n", score_region);

    int score_multipliers[] = {3,2};

    int score;
    if ( (99 < polar[0]) && (polar[0] < 107) ) {
      score = score_region * score_multipliers[0];
    }
    else if ( (162 < polar[0]) && (polar[0] < 170) ) {
      score = score_region * score_multipliers[1];
    }
    else if (polar[0] > 170) {
      score = 0;
    }
    else {
      score = score_region;
    }
    return score;
  }

}

void debug() {
  //double scoreTest[2] = {-60.106581,-131.189313};
  double scoreTest[2] = {46.978506,92.874600};
  int s = scorePos(scoreTest);
  printf("\n\n");
  printf("score is %i\n", s);
}


int main(int argc, char **argv) {
  //  srand(time(NULL));

  if (argc > 3) {
    // Number of darts to throw
    long count = strtol(argv[1], NULL, 10);
    double scale = strtod(argv[2], NULL);
    // Where to throw them at
    long target = strtol(argv[3], NULL, 10);

    double *targetPos;
    if (target == 1) {
      // middle of 1 slice
      double s = (SLICE_SIZE * 4) - (SLICE_SIZE / 2);

      // middle of *3 area
      double r = 103.0;
      double polar[2] = {r, s};
      double c[2];
      targetPos = toCartesian(polar, c);

    }
    else if (target == 20) {
      // middle of 20 slice
      double s = (SLICE_SIZE * 5) - (SLICE_SIZE / 2);

      // middle of 3* area
      double r = 103;
      double polar[2] = {r, s};
      double c[2];
      targetPos = toCartesian(polar, c);
      //printf("Polar Target:\t(%f,%f)\n", polar[0], polar[1]);
      //printf("Target:\t(%f,%f)\n", targetPos[0], targetPos[1]);
    }
    else {
      printf("target must be 1 or 20\n");
      return 1;
    }
    int idealScore = scorePos(targetPos);
    //printf("Target Score is:\t%i\n", idealScore);

    long x;
    long long totalScore = 0;
    for (x = 0; x < count; x++) {
      double r[2];
      double *randomPoint = randomNormal(r, scale);

      randomPoint[0] += targetPos[0];
      randomPoint[1] += targetPos[1];

      //      printf("(%f,%f)\t", randomPoint[0], randomPoint[1]);

      int score = scorePos(randomPoint);
      totalScore += score;
      //printf("%i\n", score);
    }

    double averageScore = (totalScore / (double) count);
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

