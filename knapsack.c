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
	int err;

	
	FILE *public_key_fp = fopen("public.key", "r");
	FILE *private_key_fp = fopen("private.key", "r");


	fread_private_key(private_key_fp, private_key, KEYSIZE, &multiplier, &modulo);
	
	err = BN_mod_inverse(modulo_inv, multiplier, modulo, ctx);
	if(err == NULL)
		error_handle("Failed to calculate inverse modulo."); 

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
	fread_key(&infile, multiplier, 1);
	fread_key(&infile, modulo, 1);
	fread_key(&infile, private_key, keysize);
}


void fread_public_key(FILE *infile, BIGNUM **pubkey, int keysize)
{
	fread_key(&infile, pubkey, keysize);
}

/**
*	FILE **fp so that the state of the file pointer can be retained over several calls.
*/
void fread_key(FILE **fp, BIGNUM **key, int keysize)
{
	int const linebuffsize = 1000;
	char linebuffer[linebuffsize];
	char ch;
	int keyidx;
	int buffidx;
	int err;


	keyidx = 0;
	buffidx = 0;
	err = 0;
	while((ch = fgetc(*fp)) != EOF)
	{
		if(buffidx > linebuffsize - 2)
			error_handle("Run out of buffer space for keynumber public key.");

		if(ch == '\n') {

			linebuffer[buffidx] = '\0';

			err = BN_hex2bn((key + keyidx), linebuffer);
			if(err == 0)
				error_handle("Unknown error reading public key.");


			++keyidx;
			buffidx = 0;
			if(keyidx == keysize)
				return;

			continue;
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
