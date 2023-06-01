/* 
 * CS:APP Data Lab 
 * 
 * Jerry Shi, jerryshi
 * 
 * bits.c - Source file with your solutions to the project.
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

You will provide your solution to the project by
editing the collection of functions in this source file.

CODING RULES:
 
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


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. The maximum number of ops for each function is given in the
     header comment for each function.

/*
 * STEP 2: Modify the following functions according to the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the btest checker to verify that your solutions produce
 *      the correct answers.
 */


#endif

/************************************************ 
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4 (DONE!)
 */
int absVal(int x) {
  /*~x + 1; if x < 0*/
  /*x; if x > = 0 */
  int x_1 = x>>31; /*sign bit of x = 0x1111... or 0 */
  return ((~x + 1)&x_1) + (x&(~x_1)); /*0x0000... or 0x1111....*/
}

/************************************************ 
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 (DONE!)
 */
int addOK(int x, int y) {
  /* From textbook 128, conditions for overflow:*/
  /* positive overflow IFF x>0 & y>0 but s <= 0*/
  /* negative overflow IFF x<0 & y<0 but s >= 0*/
  int x_1 = x>>31; /* sign bit of x*/
  int y_1 = y>>31;
  int s = x + y;
  int s_1 = s>>31;
  /* x & y share the same sign AND x and s are different */
  return !((!(x_1^y_1))&(x_1^s_1));
}

/************************************************ 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2 (DONE!)
 */
int allEvenBits(int x) {
	int r;
	int e = 0x55 << 8;
	e = e|0x55;
	e = e << 8;
	e = e|0x55;
	e = e << 8;
	e = e|0x55;
	r = x & e;
  return !(e^r);
}

/************************************************
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 (DONE!)
 * when you convert x to -x, sign bit will swap
 * only exceptions are 0 and most negative minimum value
 * out of those two, only for 0 does sign bit stay 0
 */
int bang(int x) {
  int y = ~x + 1;
  int y_1 = y>>31;
  int x_1 = x>>31;
  return ((~(x_1^0))&0x1)&((~(y_1^0))&0x1);
 /*check if both sign bits equal 0*/
}

/************************************************
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4 (DONE!)
 */
int bitCount(int x) {
  int m = (0x11<<8)|0x11; /*0x1111*/
  int m_1 = (m<<16)|m; /*0x11111111*/
  int x_1 = (x&m_1) + ((x>>1)&m_1) + ((x>>2)&m_1) + ((x>>3)&m_1);
  int x_2 = (x_1>>4);
  int x_3 = (x_2>>4);
  int x_4 = (x_3>>4);
  int x_5 = (x_4>>4);
  int x_6 = (x_5>>4);
  int x_7 = (x_6>>4);
  int x_8 = (x_7>>4);
  int n_1 = x_1&0xF;
  int n_2 = x_2&0xF;
  int n_3 = x_3&0xF;
  int n_4 = x_4&0xF;
  int n_5 = x_5&0xF;
  int n_6 = x_6&0xF;
  int n_7 = x_7&0xF;
  int n_8 = x_8&0xF;
  return n_1 + n_2 + n_3 + n_4 + n_5 + n_6 + n_7 + n_8;
}

/************************************************
 * bitNor - ~(x|y) using only ~ and & 
 *   Example: bitNor(0x6, 0x5) = 0xFFFFFFF8
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1 (DONE!)
 */
int bitNor(int x, int y) {
  return (~x)&(~y);
}

/************************************************
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2 (DONE!)
 */
int byteSwap(int x, int n, int m) {
  int m_1 = 0xFF<<(n<<3);
  int m_2 = 0xFF<<(m<<3);
  int x_m = ((x&(~m_1)))&(~m_2);
  /*move byte to least significant bit*/
  int x_m1 = (x&m_1)>>(n<<3);
  int x_m2 = (x&m_2)>>(m<<3);
  /*zero out any leading bits, then swap pos*/
  x_m1 = (x_m1&0xFF)<<(m<<3);
  x_m2 = (x_m2&0xFF)<<(n<<3);
  /*add back bits*/
  return ((x_m|x_m1)|x_m2);
}

/************************************************ 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3 (DONE!)
 */
/* something is false if it is 0 */
/* y * ~!x + z * ~!x if multiplying was allowed*/
int conditional(int x, int y, int z) {
  int n = !x + (~1 + 1); /*n = either 1111.... for nonzero or 0 for zero */
  return (y&n) | (z&(~n)); /* y is returned if nonzero, z is returned if zero*/
}

/************************************************
 * ezThreeFourths - multiplies by 3/4 rounding toward 0,
 *   Should exactly duplicate effect of C expression (x*3/4),
 *   including overflow behavior.
 *   Examples: ezThreeFourths(11) = 8
 *             ezThreeFourths(-9) = -6
 *             ezThreeFourths(1073741824) = -268435456 (overflow)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 3 (DONE!)
 */
int ezThreeFourths(int x) {
  int y = x + x + x; /*multiply by 3*/
  y = y + (3&(y>>31)); /* need to add bias value if y is negative */
  /* without bias value, negative will round away from zero*/
  y = y>>2; /*divide by four*/
  return y;
}

/************************************************ 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2 (DONE!)
 */

int fitsBits(int x, int n) {
  int y = x<<(32+(~n+1)); /*make n bit the most significant bit*/
  y = y>>(32+(~n+1)); /*sign extension will apply*/
  return !(y^x); /*if y=x, then x can be expressed in n bits*/
  /* failed attempt - truncation of unsigned number x is x modulo 2^bits*/
  /* int mod = x>>n; */
  /* return x + (~mod + 1); */
}

/************************************************
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Byte is ordered     3 2 1 0
 *   Want to shift right 24|16|8 0 bits
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2 (DONE!)
 */
int getByte(int x, int n) {
  int y = n<<3; /* 3 x 8 */
  x = x>>y;
  return x&0xFF; /* mask of 00001111*/
}

/************************************************ 
 * greatestBitPos - return a mask that marks the position of the
 *               most significant 1 bit. If x == 0, return 0
 *   Example: greatestBitPos(96) = 0x40
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 70
 *   Rating: 4 
 * Failed attempt:
 *y = ((y>>8)&0xFF);
  pos = pos<<(!!y<<3);
  y = ((y>>8)&0xFF);
  pos = pos<<(!!y<<3);
 */
int greatestBitPos(int x) {
  /* need to do a binary search*/
  int pos = !!x; /*initialize pos, make sure x is not 0*/
  int y = !(x>>16); /*y = 0 if there is a 1; 1 if there is no 1*/
  int m = ~y+1; /*m = 0 if there is a 1; 1 if there is no 1*/
  x = (x&m)|(x>>16); /*shift if there is no 1 in upper half */
  pos = pos<<(!y<<4); /*shift more if y = 0 meaning there is a 1*/

  y = !(x>>8);
  m = ~y+1;
  x = (x&m)|(x>>8);
  pos = pos<<(!y<<3);

  y = !(x>>4);
  m = ~y+1;
  x = (x&m)|(x>>4);
  pos = pos<<(!y<<2);

  y = !(x>>2);
  m = ~y+1;
  x = (x&m)|(x>>2);
  pos = pos<<(!y<<1);

  y = !(x>>1);
  m = ~y+1;
  x = (x&m)|(x>>1);
  pos = pos<<(!y);

  return pos;
}

/************************************************
 * implication - return x -> y in propositional logic - 0 for false, 1
 * for true
 *   Example: implication(1,1) = 1
 *            implication(1,0) = 0
 *            implication(0,1) = 1
 *            implication(0,0) = 1
 *   Legal ops: ! ~ ^ |
 *   Max ops: 5
 *   Rating: 2 (DONE!)
 */
int implication(int x, int y) {
  return !x|!(x^y);
}

/************************************************ 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3 (DONE!)
 */
int isAsciiDigit(int x) {
  /* first, check if x is <= 0x39*/
  int y = 0x39 + (~x+1);
  /* then, check if 0x30 <= x */
  int z = x + (~0x30+1);
  y = y>>31;
  z = z>>31;
  return (!(y^0)&!(z^0));
}

/************************************************ 
 * isEqual - return 1 if x == y, and 0 otherwise 
 *   Examples: isEqual(5,5) = 1, isEqual(4,5) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2 (DONE!)
 */
int isEqual(int x, int y) {
  return !(x^y);
}

/************************************************
 * isLess - if x < y  then return 1, else return 0 
 *   Example: isLess(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3 (DONE!)
 */
int isLess(int x, int y) {
  /* if x < y, then y - x > 0*/
  int z = y + ~x; /* following logic excludes 0 case */
  int y_1 = y>>31; /* first bit in y*/
  int x_1 = x>>31; /* first bit in x*/
  z = z>>31;
  return (!(y_1^x_1)&(!z)) | ((!y_1)&(x>>31));
  /* need to compare signs since different signs may lead to overflow error*/
  /* if they are different AND x is negative, return TRUE*/
  /* don't need to modify inverse bc it will already return 0 */
  /* want to return !z only when sign of x and y are the same */
}

/************************************************
 * isNonNegative - return 1 if x >= 0, return 0 otherwise 
 *   Example: isNonNegative(-1) = 0.  isNonNegative(0) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 3 (DONE!)
 */
int isNonNegative(int x) {
  x = x>>31; /* only need to return most significant bit*/
  return !x;
}

/************************************************
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
 *   Note that no negative number is a power of 2
 *   A power of 2 has only one 1 bit, and the rest is 0
 *   exception is 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4 (DONE!)
 */
int isPower2(int x) {
  /*all powers of 2 have just one bit of 1 except for 0*/
  /* thus, for powers of two the following evaluates to 0*/
  int y = x&(x+ (~1+1));
  /* eliminate zero case because it will return 1*/
  int x_1 = !(x>>31); /*negated sign bit of x = 1 if x is positive*/
  return (!!x&!(y^0))&x_1; /*recall !(x^y) is same as x==y*/
}

/************************************************
 * isTmin - returns 1 if x is the minimum (two's complement number)
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1 (DONE!)
 */
int isTmin(int x) {
  int y = ~x + 1; /*if x is min value, then this will == x */
  /* need to exclude edge case of x = 0*/
  /* recall that 0 is the only value where !! will equal 0*/
  return !(x^y) & !!x;
}

/************************************************
 * minusOne - return a value of -1, with no input 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1 (DONE!)
 */
int minusOne(void) {
  return ~1+1;
}

/************************************************
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateLeft(0x87654321,4) = 0x76543218
 *   rotate like a ring and tack on at the end
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3 (DONE!) 
 */
int rotateLeft(int x, int n) {
  int y = x<<n;
  /*need to apply mask to ensure sign extension doesn't mess with number*/
  int mask = (1<<n) + (~1+1);
  /* from textbook page 163, creates mask with bit-n leadings zeroes then ones*/
  /* shift x right to preserve leading digits of n bits*/
  x = (x>>(32+(~n+1)))&mask;
  return x|y;
}

/* SKIP THIS ONE
 */
/************************************************
 * satMul2 - multiplies by 2, saturating to Tmin or Tmax if overflow
 *   Examples: satMul2(0x30000000) = 0x60000000
 *             satMul2(0x40000000) = 0x7FFFFFFF (saturate to TMax)
 *             satMul2(0x60000000) = 0x80000000 (saturate to TMin)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 (SKIP THIS ONE!!!!!)
 */
int satMul2(int x) {
  return 2;
}