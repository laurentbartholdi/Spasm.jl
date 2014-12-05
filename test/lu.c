#include <stdio.h>
#include <assert.h>
#include "spasm.h"

int main(int argc, char **argv) {
  spasm_triplet *T;
  spasm *A;
  spasm_lu *LU;
  spasm_GFp *x, *y, *u, *v;
  int n, m, test, i, j;

  assert(argc > 1);
  test = atoi(argv[1]);

  T = spasm_load_triplet(stdin, 7);
  A = spasm_compress(T);
  spasm_triplet_free(T);

  n = A->n;
  m = A->m;

  x = malloc(n * sizeof(spasm_GFp));
  y = malloc(m * sizeof(spasm_GFp));
  u = malloc(n * sizeof(spasm_GFp));
  v = malloc(m * sizeof(spasm_GFp));

  LU = spasm_LU(A, SPASM_IDENTITY_PERMUTATION);

  for(i = 0; i < n; i++) {
    for(j = 0; j < n; j++) {
      x[j] = 0;
      u[j] = 0;
    }
    for(j = 0; j < m; j++) {
      y[j] = 0;
      v[j] = 0;
    }
    x[i] = 1;

    spasm_gaxpy(A, x, y);     // y <- x*A
    spasm_gaxpy(LU->L, x, u); // u <- x*L
    spasm_gaxpy(LU->U, u, v); // v <- (x*L)*U

    for(j = 0; j < m; j++) {
      if (y[j] != v[j]) {
	printf("not ok %d - L*U == A (col %d)\n", test, j);
	exit(0);
      }
    }
  }

  printf("ok %d - L*U == A\n", test);

  spasm_csr_free(A);
  spasm_free_LU(LU);
  free(x);
  free(y);
  free(u);
  free(v);
  return 0;
}
