//Matthew Soulanille

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "dartsmodule.h"

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
/*
void debug() {
  //double scoreTest[2] = {-60.106581,-131.189313};
  double scoreTest[2] = {46.978506,92.874600};
  int s = scorePos(scoreTest);
  printf("\n\n");
  printf("score is %i\n", s);
}
*/

double darts(long count, double accuracy, int target) {

  //printf("(count, accuracy, target):\t(%ld, %f, %i)\t", count, accuracy, target);
  double *targetPos;
  //printf("TargetPos: (%f, %f)\n",targetPos[0], targetPos[1]);
  double c[2];
  if (target == 1) {

    // middle of 1 slice
    double s = (SLICE_SIZE * 4) - (SLICE_SIZE / 2);
    
    // middle of *3 area
    double r = 103.0;
    double polar[2] = {r, s};

    //printf("[%f, %f]\t", polar[0], polar[1]);
    targetPos = toCartesian(polar, c);
    //printf("{%f, %f}\t", targetPos[0], targetPos[1]);
  }
  else if (target == 20) {
    // middle of 20 slice
    double s = (SLICE_SIZE * 5) - (SLICE_SIZE / 2);
    
    // middle of 3* area
    double r = 103;
    double polar[2] = {r, s};

    targetPos = toCartesian(polar, c);
  }
  else {
    return -10000000.0;
  }
  //printf("TargetPos: (%f, %f)\n",targetPos[0], targetPos[1]);

  long x;
  long long totalScore = 0;
  
  for (x = 0; x < count; x++) {
    double r[2];
    double *randomPoint = randomNormal(r, accuracy);
    
    randomPoint[0] += targetPos[0];
    randomPoint[1] += targetPos[1];
    
    int score = scorePos(randomPoint);
    totalScore += score;

  }
  double averageScore =  (totalScore / (double) count);


  //printf("%f\n", averageScore);
  return averageScore;
 
}



double darts_parallel(long count, double accuracy, int target) {
  // Divides work between multiple threads that run the above darts() function.
  static int num_threads = 8;
  omp_set_num_threads(num_threads);
  static int workunit_size = 10000;


  int workunit_count = (int) (count / workunit_size);

  int last_size = 0;
  if (count % workunit_size > 0) {
      workunit_count ++;
      last_size = count % workunit_size;
  }

  // Array of workunit sizes for calling darts() and calculating weights
  double workunit_sizes[workunit_count];
  int i;

  for (i = 0; i < workunit_count; i++) {
    workunit_sizes[i] = workunit_size;
  }
  if (last_size > 0) {
    workunit_sizes[workunit_count - 1] = last_size;
  }
  
  // Array to write results of darts() to
  double averages[workunit_count];
  
#pragma omp parallel for shared(averages) private(i)
  for (i=0; i < workunit_count; i++) {
    //int tid = omp_get_thread_num();

    averages[i] = darts(workunit_sizes[i], accuracy, target);

    //printf("Thread id %d\n", tid);
  
  }
  double result = 0.0;

  // Assemble the resulting averages
  for (i=0; i < workunit_count; i++) {
    double weight = ((double) workunit_sizes[i]) / count;
    result += averages[i] * weight;
  }

  return result;
}



double darts_optimized(long count, double accuracy, int target) {
  // Not actually faster relative to darts_parallel
  static int num_threads = 8;
  omp_set_num_threads(num_threads);

  double *targetPos;
  double c[2];
  if (target == 1) {
    // middle of 1 slice
    double s = (SLICE_SIZE * 4) - (SLICE_SIZE / 2);
    
    // middle of *3 area
    double r = 103.0;
    double polar[2] = {r, s};

    targetPos = toCartesian(polar, c);
  }
  else if (target == 20) {
    // middle of 20 slice
    double s = (SLICE_SIZE * 5) - (SLICE_SIZE / 2);
    
    // middle of 3* area
    double r = 103;
    double polar[2] = {r, s};

    targetPos = toCartesian(polar, c);
  }
  else {
    return -10000000.0;
  }

  long x;


  long long totalScore = 0;
#pragma omp parallel for reduction(+:totalScore)
  for (x = 0; x < count; x++) {
    double r[2];
    double *randomPoint = randomNormal(r, accuracy);
    
    randomPoint[0] += targetPos[0];
    randomPoint[1] += targetPos[1];
    
    int score = scorePos(randomPoint);
    // for omp reduction
    totalScore += (long long) score;

  }
  //printf("%lld \n", totalScore);
  double averageScore =  (totalScore / (double) count);


  //printf("%f\n", averageScore);
  return averageScore;

}
