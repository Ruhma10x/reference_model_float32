#include "bigfloat.h"

/*
 * Create BigFloat and return the pointer.
 */

typedef struct bigfloat {
  char digits[PRECISION];
  char decimal;
  char negative;
} BigFloat;


void add(BigFloat *a, BigFloat *b, BigFloat *res) {
  int i, result;
  int carry = 0;
   if (b->decimal > a->decimal) {
    shiftDownBy(a->digits, PRECISION, b->decimal - a->decimal);
    a->decimal = b->decimal;
  } else if (b->decimal < a->decimal) {
    shiftDownBy(b->digits, PRECISION, a->decimal - b->decimal);
    b->decimal = a->decimal;
  }
}
  int i;
  if (res != NULL) {
    for (i = 0; i < PRECISION; i++) {
      res->digits[i] = 0;
    }
  }
  res->decimal = a->decimal;
  unsigned char additionType = (a->negative + b->negative);
  if (additionType != 1)
  {
    for (i = PRECISION - 1; i >= 0; i--) {
      result = carry;
  /*    result += (a->negative) ? -1 * a->digits[i] : a->digits[i];
      result += (b->negative) ? -1 * b->digits[i] : b->digits[i]; */
      result += a->digits[i] + b->digits[i];
      carry = result / 10;
      res->digits[i] = result % 10;
    }
    if (carry != 0) {
      //shiftDownBy(res->digits, PRECISION, 1);

        int i;
        for (i = PRECISION - 1; i >= 0; i--) {
         if (i - 1 >= 0) {
        res->digits[i] = res->digits[i - 1;
        } else {
         res->digits[i] = 0;
        }
    }

      res->decimal++;
      res->digits[0] = carry;
    }
    trailingZeros(a);

  int i, start;
  for (i = 0; i < PRECISION && !a->digits[i]; i++);
  if (a->decimal - i < 1) {
    i = a->decimal - 1;
  }
  start = i;
  shiftUpBy(a->digits, PRECISION, start);
    int q;
  for (q = 0; q < PRECISION; q++) {
    if (q + i < PRECISION) {
      ar[q] = ar[q + i];
    } else {
      ar[q] = 0;
    }
  }
  a->decimal -= i;    

    trailingZeros(b);
    trailingZeros(res);
    if (additionType == 2)
      res->negative = 1;
  }
  else if (a->negative)
  {
    b->negative = 1;
    subtract(b, a, res);
    b->negative = 0;
  }
  else if (b->negative)
  {
    a->negative = 1;
    subtract(a, b, res);
    a->negative = 0;
  }
}