#include "knapsack.h"

#define KEYSIZE 8

void test()
{
	BIGNUM *public_key[KEYSIZE] = {NULL};
	BIGNUM *private_key[KEYSIZE] = {NULL};
	BIGNUM *multiplier = NULL;
	BIGNUM *modulo = NULL;
	BIGNUM *modulo_inv = BN_new();
	//	The variable 'ctx' is reused by function. 
	//	Whenever it is required for a function use this variable.
	BN_CTX *ctx = BN_CTX_new();

	int i;

	
	FILE *public_key_fp = fopen("public.key", "r");
	FILE *private_key_fp = fopen("private.key", "r");


	fread_public_key(public_key_fp, public_key, KEYSIZE);
	fread_private_key(private_key_fp, private_key, KEYSIZE, &multiplier, &modulo);
	
	if( !BN_mod_inverse(modulo_inv, multiplier, modulo, ctx) )
		error_handle("Failed to calculate inverse modulo."); 












	//	Print public key info

	printf("public key:\n");
	for(i = 0; i < KEYSIZE; ++i) {
		BN_print_fp(stdout, public_key[i]);
		putchar('\n');
	}

	//	Print private key info
	
	printf("private key:\n");
	for(i = 0; i < KEYSIZE; ++i) {
		BN_print_fp(stdout, private_key[i]);
		putchar('\n');
	}
	printf("multiplier:\n");
	BN_print_fp(stdout, multiplier);
		putchar('\n');
	printf("modulo:\n");
	BN_print_fp(stdout, modulo);
		putchar('\n');
	printf("modulo_inv:\n");
	BN_print_fp(stdout, modulo_inv);
		putchar('\n');

	
}

void fread_private_key(FILE *infile, BIGNUM **private_key, int keysize, BIGNUM **multiplier, BIGNUM **modulo)
{
	fread_hex_line_into_bignum(infile, multiplier);
	fread_hex_line_into_bignum(infile, modulo);
	fread_key(infile, private_key, keysize);
}


void fread_public_key(FILE *infile, BIGNUM **pubkey, int keysize)
{
	fread_key(infile, pubkey, keysize);
}


void fread_key(FILE *fp, BIGNUM **key, int keysize)
{
	int idx;

	for(idx = 0; idx < keysize; ++idx) {
		fread_hex_line_into_bignum(fp, &(key[idx]));
	}
}


void fread_hex_line_into_bignum(FILE *fp, BIGNUM **bn)
{
	int const linebuffsize = 1000;
	char linebuffer[linebuffsize];
	char ch;
	int buffidx;
	int err;


	buffidx = 0;
	err = 0;
	while((ch = fgetc(fp)) != EOF)
	{
		if(buffidx > linebuffsize - 2)
			error_handle("Run out of buffer space while reading line.");

		if(ch == '\n') {

			linebuffer[buffidx] = '\0';

			err = BN_hex2bn(bn, linebuffer);
			if(err == 0)
				error_handle("Unknown error readin line into bignum.");


			return;
		}
		linebuffer[buffidx] = ch;

		++buffidx;
	}
}

void error_handle(char const *str)
{
	fprintf(stdout, "Error: %s\n", str);
	exit(-1);
}
