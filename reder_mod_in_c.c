#include <stdio.h>
#include <stdint.h>

#include <math.h>
#include <float.h>
#include <fenv.h>
#include <stdbool.h>

#pragma fenv_access (on)

typedef struct{
  bool invalid;
  bool divisionByZero;
  bool overflow;
  bool underflow;
  bool inexact;
} flags;

// Structure to represent FP16



void flag_values(){
  flags flags;

  flags.invalid = fetestexcept(FE_INVALID);
  flags.divisionByZero = fetestexcept(FE_DIVBYZERO);
  flags.overflow = fetestexcept(FE_OVERFLOW);
  flags.underflow = fetestexcept(FE_UNDERFLOW);
  flags.inexact = fetestexcept(FE_INEXACT);

printf("IV: %d | DZ: %d | OF: %d | UF: %d | NE: %d", flags.invalid, flags.divisionByZero, flags.overflow, flags.underflow, flags.inexact);

}


uint16_t convertToFP16(float value) {
  uint32_t f = *((uint32_t*)&value);
  uint16_t h_sgn = (uint16_t)((f & 0x80000000u) >> 16);
  uint32_t f_exp = f & 0x7f800000u;
  uint32_t f_sig = f & 0x007fffffu;

  // Exponent overflow/NaN converts to signed inf/NaN
  if (f_exp >= 0x47800000u) {
    if ((f_exp == 0x7f800000u) && (f_sig != 0)) {
      // NaN - propagate the flag in the significand...
      uint16_t ret = (uint16_t)(0x7c00u + (f_sig >> 13));
      ret += (ret == 0x7c00u); // ...but make sure it stays a NaN
      return h_sgn + ret;

    } else {
      // (overflow to) signed inf
      return (uint16_t)(h_sgn + 0x7c00u);
    }
  }

  // Exponent underflow converts to a subnormal half or signed zero
  if (f_exp <= 0x38000000u) {
    // Signed zeros, subnormal floats, and floats with small
    // exponents all convert to signed zero half-floats.
    if (f_exp < 0x33000000u) {
      return h_sgn;
    }
    // Make the subnormal significand
    f_exp >>= 23;
    f_sig += 0x00800000u;
    f_sig >>= (113 - f_exp);
    // Handle rounding by adding 1 to the bit beyond half precision
    //
    // If the last bit in the half significand is 0 (already even),
    // and the remaining bit pattern is 1000...0, then we do not add
    // one to the bit after the half significand. However, the
    // (113 - f_exp) shift can lose up to 11 bits, so the || checks
    // them in the original. In all other cases, we can just add one.
    if (((f_sig & 0x3fffu) != 0x1000u) || (f & 0x07ffu)) {
      f_sig += 0x1000u;
    }
    uint16_t h_sig;
    h_sig = (uint16_t)(f_sig >> 13);
    // If the rounding causes a bit to spill into h_exp, it will
    // increment h_exp from zero to one and h_sig will be zero.
    // This is the correct result.
    return (uint16_t)(h_sgn + h_sig);
  }

  // Regular case with no overflow or underflow
  uint16_t h_exp = (uint16_t)((f_exp - 0x38000000u) >> 13);
  // Handle rounding by adding 1 to the bit beyond half precision
  //
  // If the last bit in the half significand is 0 (already even), and
  // the remaining bit pattern is 1000...0, then we do not add one to
  // the bit after the half significand. In all other cases, we do.
  if ((f_sig & 0x3fffu) != 0x1000u) {
      f_sig += 0x1000u;
  }
  uint16_t h_sig = (uint16_t)(f_sig >> 13);
  // If the rounding causes a bit to spill into h_exp, it will
  // increment h_exp by one and h_sig will be zero. This is the
  // correct result. h_exp may increment to 15, at greatest, in
  // which case the result overflows to a signed inf.
  return (uint16_t)(h_sgn + h_exp + h_sig);
}


// Function to perform addition of FP32 numbers
float add(float a, float b) {
    //fesetround(FE_UPWARD); 
    return a + b;
}

// Function to perform subtraction of FP32 numbers
float subtract(float a, float b) {
    return a - b;
}

// Function to perform multiplication of FP32 numbers
float multiply(float a, float b) {
   


    return a * b;
}

// Function to perform division of FP32 numbers
float divide(float a, float b) {
    float result;
    result = a / b;
    
//    int i;
//     i = fetestexcept(FE_INVALID | FE_DIVBYZERO );
// if (i & FE_INVALID)
//     printf("Invalid flag was raised");
// else if (i & FE_DIVBYZERO)
//     printf("Zero flag was raised");
//     /* division by zero flag was raised */
    return result;
}

// Function to perform square root of FP32 number
float squareRoot(float a) {
  
    return sqrt(a);
}

int main() {

    //Rounding modes
    //To truncate
    
    //To round upwards
    fesetround(FE_UPWARD); 
    //downward 
    //fesetround(FE_DOWNWARD);
    // toward zero
    //fesetround(FE_TOWARDZERO);
    //The default is to round to the nearest value 
    //fesetround(FE_TONEAREST);
    //  double a = 0.1;
    // unsigned int control_word_x87 = 0;
    // int result;
    // Add 0.500000178814 and 3 with default rounding and round upward.
    // // Show original x87 control word and do calculation.
    // result = __control87_2(0, 0, &control_word_x87, 0 );
    // printf( "Original: 0x%.8x\n", control_word_x87 );
    // printf( "%1.1f * %1.1f = %.15e\n", a, a, a * a );




    float a, b;
    printf("Enter two FP32 numbers: ");
    scanf("%f %f", &a, &b);
    printf("a: %.23f, b: %.23f",a,b);

    float c;
    c = INFINITY - INFINITY;
    printf("a: %f, b: %f, %f",a,b,c);
    

    // Perform calculations
    printf("\nResults:\n");
    
    float sum = add(a, b);
    printf("Sum: %.23f\n", sum);
    flag_values();
    feclearexcept(FE_ALL_EXCEPT);
    float difference = subtract(a, b);
    printf("\nDifference: %.23f\n", difference);
    flag_values();
    feclearexcept(FE_ALL_EXCEPT);
    float product = multiply(a, b);
    printf("\nProduct: %.23f\n", product);
    flag_values();
    feclearexcept(FE_ALL_EXCEPT);
    float quotient = divide(a, b);
    printf("\nQuotient: %.23f\n", quotient);
    flag_values();
    feclearexcept(FE_ALL_EXCEPT);
    float sqrtA = squareRoot(a);
    printf("\nSquare root of A: %.23f\n", sqrtA);
    flag_values();
    feclearexcept(FE_ALL_EXCEPT);
    float sqrtB = squareRoot(b);
    printf("\nSquare root of B: %.23f\n", sqrtB);
    flag_values();
    feclearexcept(FE_ALL_EXCEPT);

    // Convert to FP16
    uint16_t fp16A = convertToFP16(a);
    printf("\nFP16 conversion:\n");
    printf("FP16 A: %d\n", fp16A);
    
    uint16_t fp16B = convertToFP16(b);
    printf("FP16 B: %d\n", fp16B);
    

    // Output results
    
    
    
    
    
    
    
    

    return 0;
}
