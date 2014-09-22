#ifndef BN
	#include <openssl/bn.h>
	#define BN 1
#endif

void test();
void error_handle(char const *);
void fread_public_key(FILE *, BIGNUM **, int);
void fread_private_key(FILE *, BIGNUM **, int, BIGNUM **, BIGNUM **);
void fread_key(FILE *, BIGNUM **, int);
void fread_hex_line_into_bignum(FILE *, BIGNUM **);
