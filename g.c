#include "stdio.h"
#include "string.h"

/****************************************************************************/
/***The input_u input_v input_z input_x reference Core_v4.2.pdf page 1570 ***/
//#define G_P192 1

#if G_P192
unsigned char U[] = "15207009984421a6586f9fc3fe7e4329d2809ea51125f8ed";
unsigned char V[] = "356b31938421fbbf2fb331c89fd588a69367e9a833f56812";
unsigned char X[] = "d5cb8454d177733effffb2ec712baeab";
unsigned char Y[] = "a6e8e7cc25a75f6e216583f7ff3dc4cf";
#define U_V_LEN 48
#else
unsigned char U[] = "20b003d2f297be2c5e2c83a7e9f9a5b9eff49111acf4fddbcc0301480e359de6";
unsigned char V[] = "55188b3d32f6bb9a900afcfbeed4e72a59cb9ac2f19d7cfb6b4fdd49f47fc5fd";
unsigned char X[] = "d5cb8454d177733effffb2ec712baeab";
unsigned char Y[] = "a6e8e7cc25a75f6e216583f7ff3dc4cf";
#define U_V_LEN 64
#endif
/****************************************************************************/


int g(unsigned char *input_msg)
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
	
	hash256(st_in ,out_data ,U_V_LEN + 32);

	int_to_char(out_char ,out_data);

	printf("output:\n");
	printf("%x\n" ,out_data[7]%1000000);
	printf("output (decimal):\n%d \n" ,out_data[7]);
	printf("6 digits (decimal):\n%d\n" ,out_data[7]);
	
	return 0;
}

int main()
{
        int i;
        unsigned char input_char[512];
        unsigned char input_hex[256];

        memset(input_char ,0 ,512);
        memset(input_hex ,0 ,128);

	memcpy(input_char ,U ,U_V_LEN);
	memcpy(&(input_char[strlen(input_char)]) ,V ,U_V_LEN);
	memcpy(&(input_char[strlen(input_char)]) ,X ,32);
	memcpy(&(input_char[strlen(input_char)]) ,Y ,32);

	str_to_hex(input_char ,input_hex);

	g(input_hex);
        return 0;
}

