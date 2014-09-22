#include "knapsack.h"

#define KEYSIZE 8

void test()
{
	BIGNUM *public_key[KEYSIZE] = {NULL};
	BIGNUM *private_key[KEYSIZE] = {NULL};
	BIGNUM *multiplier = NULL;
	BIGNUM *modulo = NULL;
	BIGNUM *multiplier_inv = BN_new();
	//	The variable 'ctx' is reused by certain BN_functions.
	//	Whenever it is required for a function use this variable.
	BN_CTX *ctx = BN_CTX_new();


	
	FILE *public_key_fp = fopen("public.key", "r");
	FILE *private_key_fp = fopen("private.key", "r");
	FILE *cipher_text_fp = stdin;


	fread_public_key(public_key_fp, public_key, KEYSIZE);
	fread_private_key(private_key_fp, private_key, KEYSIZE, &multiplier, &modulo);
	
	if(! BN_mod_inverse(multiplier_inv, multiplier, modulo, ctx))
		error_handle("Failed to calculate inverse modulo."); 


	decrypt(cipher_text_fp, private_key, KEYSIZE, multiplier_inv, modulo, ctx);

	
}
void decrypt(FILE *infile, BIGNUM **private_key, int keysize, BIGNUM *multiplier_inv, BIGNUM *modulo, BN_CTX *ctx)
{
	BIGNUM *cipher_num = NULL;
	BIGNUM *message_num = BN_new();
	char ch;


	while(fread_hex_line_into_bignum(infile, &cipher_num) != EOF)
	{

		BN_mod_mul(message_num, cipher_num, multiplier_inv, modulo, ctx);
		ch = solve_super_knapsack(private_key, keysize, message_num);
		putchar(ch);
	}
	putchar('\n');
}

char solve_super_knapsack(BIGNUM **private_key, int keysize, BIGNUM *sum)
{
	BIGNUM *zero = BN_new();
	BIGNUM *tmp = BN_new();

        int i;
        char mask[] = {1,2,4,8,16,32,64,-128};
        char letter;

	BN_zero(zero);
	letter = 0;
        for(i = keysize; --i >= 0;) {

		BN_sub(tmp, sum, private_key[i]);

                if(BN_cmp(tmp, zero) >= 0) {

                        letter |= mask[i];
                        BN_copy(sum, tmp);
                }
        }

        return letter;
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


int fread_hex_line_into_bignum(FILE *fp, BIGNUM **bn)
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
				error_handle("Unknown error reading hex line into bignum.");


			return 0;
		}
		linebuffer[buffidx] = ch;

		++buffidx;
	}
	return EOF;
}

void error_handle(char const *str)
{
	fprintf(stdout, "Error: %s\n", str);
	exit(-1);
}
