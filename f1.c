#include "stdio.h"
#include "string.h"

/****************************************************************************/
/***The input_u input_v input_z input_x reference Core_v4.2.pdf page 1570 ***/
#define F1_P_192 1

#if F1_P_192
unsigned char input_u[] = "356b31938421fbbf2fb331c89fd588a69367e9a833f56812";
unsigned char input_v[] = "15207009984421a6586f9fc3fe7e4329d2809ea51125f8ed";
unsigned char input_z[] = "00";
unsigned char input_x[] = "d5cb8454d177733effffb2ec712baeab";
#define INPUT_LEN 48
#elif F1_P_256
unsigned char input_u[] = "20b003d2f297be2c5e2c83a7e9f9a5b9eff49111acf4fddbcc0301480e359de6";
unsigned char input_v[] = "55188b3d32f6bb9a900afcfbeed4e72a59cb9ac2f19d7cfb6b4fdd49f47fc5fd";
unsigned char input_z[] = "00";
unsigned char input_x[] = "d5cb8454d177733effffb2ec712baeab";
#define INPUT_LEN 64
#endif
/****************************************************************************/


int f_1(unsigned char *input_key ,unsigned char *input_msg)
{
	int i;
	unsigned char st_in[256];
	unsigned char key_in[64];
	unsigned char st_in_hash[512];
	unsigned int out_data[8];
	unsigned char out_char[32];
	unsigned char o_key_pad[64];
	unsigned char i_key_pad[64];
	
	memset(st_in ,0 ,256);
	memcpy(st_in ,input_msg ,256);

	memset(key_in ,0 ,64);
	memcpy(key_in ,input_key ,64);	

	for(i = 0 ;i < 64 ;i++) {
		o_key_pad[i] = key_in[i] ^ 0x5c;
		i_key_pad[i] = key_in[i] ^ 0x36;
	}
	
	memset(st_in_hash ,0 ,512);
	for(i = 0 ;i < 64 ;i++) {
		st_in_hash[i] = i_key_pad[i];	
	}
	for( ;i < 329 ;i++) {
		st_in_hash[i] = st_in[i - 64];
	}
	if(strlen(st_in_hash) < 120)
		hash256(st_in_hash ,out_data ,113);
	else
		hash256(st_in_hash ,out_data ,129);

	int_to_char(out_char ,out_data);

	memset(st_in_hash ,0 ,512);
	for(i = 0 ;i < 64 ;i++) {
		st_in_hash[i] = o_key_pad[i];
	}
	for( ;i < 96 ;i++) {
		st_in_hash[i] = out_char[i - 64];
	}
	memset(out_data ,0 ,32);

	hash256(st_in_hash ,out_data ,96);
	int_to_char(out_char ,out_data);

	printf("hmac-sha256:\n");

	for(i = 0 ;i < 4 ;i++) {
		printf("%x" ,out_data[i]);
	}
	printf("\n");
	return 0;
}

int main()
{
        int i;
        unsigned char input_char[512];
        int input_len;
        unsigned char input_hex[256];
	unsigned char input_key_char[128];
	unsigned char input_key_hex[64];

        memset(input_char ,0 ,512);
        memset(input_hex ,0 ,128);
	memset(input_key_char ,0 ,128);
	memset(input_key_hex ,0 ,64);

	memcpy(input_char ,input_u ,INPUT_LEN);
	memcpy(&(input_char[strlen(input_char)]) ,input_v ,INPUT_LEN);
	memcpy(&(input_char[strlen(input_char)]) ,input_z ,2);

	str_to_hex(input_char ,input_hex);

	memcpy(input_key_char ,input_x ,32);
	str_to_hex(input_key_char ,input_key_hex);

	f_1(input_key_hex ,input_hex);
        return 0;
}

