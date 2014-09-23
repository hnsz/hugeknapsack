#include "knapsack.h"

#define KEYSIZE 8

int main()
{
	BIGNUM *public_key[KEYSIZE] = {NULL};

	
	FILE *public_key_fp = fopen("public.key", "r");
	FILE *message_text_fp = stdin;


	fread_public_key(public_key_fp, public_key, KEYSIZE);


	encrypt(message_text_fp, public_key, KEYSIZE);


	//	Requires some BN_free statements if code is to be reused.
	return 0;	
}
