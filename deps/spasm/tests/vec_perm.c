#include <stdio.h>
#include <stdlib.h>

#include "spasm.h"

int main() {
  int n = 50;
  int i;

  printf("1..2\n");

  int *p = malloc(n * sizeof(int));
  int *pinv;
  spasm_ZZp *x = malloc(n * sizeof(spasm_ZZp));
  spasm_ZZp *y = malloc(n * sizeof(spasm_ZZp));

  for(i = 0; i < n; i++) {
    x[i] = i*i + 3*i - 7;
    p[i] = i;
  }

  // generate random permutation
  for (int i = n-1; i > 0; i--) {
    int j = rand() % i;
    int tmp = p[i];
    p[i] = p[j];
    p[j] = tmp;
  }

  // test 1 : apply permutation
  spasm_pvec(p, x, y, n);
  int fail = 0;
  for(i = 0; i < n; i++) {
    fail |= (y[i] == i*i + 3*i - 7);
  }
  if (fail) {
    printf("not ok 1 - vector permutation\n");
    exit(1);
  } else {
    printf("ok 1 - vector permutation\n");
  }

  // test 2 : apply inverse permutation
  pinv = spasm_pinv(p, n);
  spasm_pvec(pinv, y, x, n);
  for(i = 0; i < n; i++) {
    fail |= (x[i] != i*i + 3*i - 7);
  }
  if (fail) {
    printf("not ok 2 - inverse vector permutation\n");
    exit(1);
  } else {
    printf("ok 2 - inverse vector permutation\n");
  }
}
