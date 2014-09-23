#include "knapsack.h"

#define KEYSIZE 8

int main()
{
	BIGNUM *private_key[KEYSIZE] = {NULL};
	BIGNUM *multiplier = NULL;
	BIGNUM *modulo = NULL;
	BIGNUM *multiplier_inv = BN_new();
	//	The variable 'ctx' is reused by certain BN_functions.
	//	Whenever it is required for a function use this variable.
	BN_CTX *ctx = BN_CTX_new();


	
	FILE *private_key_fp = fopen("private.key", "r");
	FILE *cipher_text_fp = stdin;


	fread_private_key(private_key_fp, private_key, KEYSIZE, &multiplier, &modulo);
	
	if(! BN_mod_inverse(multiplier_inv, multiplier, modulo, ctx))
		error_handle("Failed to calculate inverse modulo."); 


	decrypt(cipher_text_fp, private_key, KEYSIZE, multiplier_inv, modulo, ctx);


	//	Requires some BN_free and BN_CTX_free statments if this block is reused

	return 0;
}
