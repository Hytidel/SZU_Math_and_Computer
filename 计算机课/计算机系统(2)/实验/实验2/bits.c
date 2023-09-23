/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
    return ~(~(x & ~y) & ~(y & ~x));
}

/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
    return 1 << 31;
}

//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 2
 */
int isTmax(int x) {
    int y = x + 1;
    x = x + y;
    x = ~x;
    y = !y;
    x = x + y;
    return !x;
}

/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
    int mask = (0xAA << 24) | (0xAA << 16) | (0xAA << 8) | 0xAA;
    x = x & mask;
    return !(x ^ mask);
}

/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
    return ~x + 1;
}

//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
    int lower = ~0x30 + 1;
    int upper = ~0x3a + 1;
    int sgn1 = x + lower >> 31;
    int sgn2 = x + upper >> 31;
    return !sgn1 & sgn2;
}

/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
    int neg_one = ~1 + 1;
    int mask = !x + neg_one;
    return (mask & y) | (~mask & z);
}

/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
    int sgn_x = x >> 31 & 1;
    int sgn_y = y >> 31 & 1;
    int sgn_xor = sgn_x ^ sgn_y;

    int neg_x = ~x + 1;
    int y_minus_x = y + neg_x;
    int sgn_y_minus_x = y_minus_x >> 31 & 1;
    return (sgn_xor & sgn_x) | (!sgn_xor & !sgn_y_minus_x);
}

//4
/* 
 * logicalNeg - implement the !s operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
    int neg_x = ~x + 1;
    x = x | neg_x;
    return (x >> 31) + 1;
}

/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
    // if x < 0, then nothing happens, else, flip
    int sgn = x >> 31;
    x = (sgn & ~x) | (~sgn & x);

    // check if there are bits in every sections
    int b16, b8, b4, b2, b1, b0;
    // check the lowest 16 places
    b16 = !!(x >> 16) << 4;
    x = x >> b16;

    // check the lowest 8 places
    b8 = !!(x >> 8) << 3;
    x = x >> b8;

    // check the lowest 4 places
    b4 = !!(x >> 4) << 2;
    x = x >> b4;

    // check the lowest 2 places
    b2 = !!(x >> 2) << 1;
    x = x >> b2;

    // check the lowest place
    b1 = !!(x >> 1);
    x = x >> b1;

    b0 = x;
    return b16 + b8 + b4 + b2 + b1 + b0 + 1;  // + 1 for the sign bit
}

//float
/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
    unsigned sgn = uf & (0x80 << 24);
    unsigned exp = uf & ((0x7f << 24) + (0x80 << 16));
    unsigned frac = uf & ((0x7f << 16) + (0xff << 8) + 0xff);

    if (!exp) frac = frac << 1;  // Denormalized
    else {  // uf < 0
        int mask = (0x7f << 24) + (0x80 << 16);
        if (exp ^ mask) {  // not all bits are 1 in exp
            exp += 0x80 << 16;  // (exp + 1) means (x 2) for Normalized
            if (!(exp ^ mask))  // all bits are 1 in exp, then set frac to 0 and return inf
                frac = 0;
        }
    }
    return sgn | exp | frac;
}

/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
    int sgn = x >> 31 & 1;
    int exp = 0;
    int frac = 0;

    if (!x) return x;  // the float expression for 0 (Denormalized) is itself
    else if (x == (1 << 31)) exp = 0x9e;
    else {
        if (sgn) x = -x;  // transform x into a positive number

        int pos;  // binary point
        for (pos = 30; !(x >> pos); pos--);
        x = x << (31 - pos);
        exp = pos + 127;  // e = E + bias

        int mask = (0x7f << 16) | (0xff << 8) | 0xff;
        frac = (x >> 8) & mask;

        x = x & 0xff;  // lowest 8 places
        int flag = x > 128 || (x == 128 && (frac & 1));  // round to even numbers
        
        frac = frac + flag;
        if (frac >> 23) {  // carry
            frac = frac & mask;
            exp = exp + 1; 
        }
    }
    return (sgn << 31) | (exp << 23) | frac;
}

/* 
 * float_f2i - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int float_f2i(unsigned uf) {
    unsigned sgn = uf & (0x80 << 24);
    unsigned exp = (uf & ((0x7f << 24) + (0x80 << 16))) >> 23;
    unsigned frac = (uf & ((0x7f << 16) + (0xff << 8) + 0xff)) | (1 << 23);

    if (!exp || exp < 127) return 0;  // zero or Denormalized less than 1
    else if (exp >= 31 + 127) return 1 << 31;  // out of range

    // transform exp and frac into complement
    int E = exp - 127;
    frac = frac >> 23 - E;
    unsigned neg_frac = ~frac + 1;

    unsigned neg_one = ~1 + 1;
    unsigned mask = !sgn + neg_one;
    unsigned neg_not_s = ~(!sgn) + 1;
    frac = (mask & neg_frac) | (neg_not_s & frac);
    return sgn | frac;
}
