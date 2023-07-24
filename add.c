#include "bigfloat.h"

/*
 * Create BigFloat and return the pointer.
 */

typedef struct bigfloat {
  char digits[PRECISION];
  char decimal;
  char negative;
} BigFloat;


BigFloat *create(char *str) {
  int i;
  BigFloat *res;
  res = malloc(sizeof(BigFloat));
  res->decimal = 1;
  for (i = 0; i < PRECISION; i++) {
    res->digits[i] = 0;
  }
  res->negative = 0;

  BigFloat *b = res;
  
  int i = 0;
  int index = 0;
  if (str[0] == '-') {
    b->negative = 1;
    i = 1;
  } else {
    b->negative = 0;
  }
  for (; i < strlen(str) && index < PRECISION; i++) {
    if (str[i] == '.') {
      b->decimal = (b->negative) ? i - 1 : i;
    } else {
      b->digits[index++] = str[i] - '0';
    }
  }
  return res;
}