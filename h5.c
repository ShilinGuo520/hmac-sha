#include "stdio.h"
#include "string.h"

/****************************************************************************/
/***The input_u input_v input_z input_x reference Core_v4.2.pdf page 1570 ***/
unsigned char R1[] = "d5cb8454d177733effffb2ec712baeab";
unsigned char R2[] = "a6e8e7cc25a75f6e216583f7ff3dc4cf";
unsigned char W[] = "b089c4e39d7c192c3aba3c2109d24c0d";

/****************************************************************************/


int h_5(unsigned char *input_key ,unsigned char *input_msg)
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
	
	hash256(st_in_hash ,out_data ,96);

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

	for(i = 0 ;i < 8 ;i++) {
		printf("%x" ,out_data[i]);
	}
	printf("\n");
	
	printf("SRESmaster:\n%x\n" ,out_data[0]);
	printf("SRESslave:\n%x\n" ,out_data[1]);

	printf("ACO:\n");
        for(i = 2 ;i < 4 ;i++) {
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

	memcpy(input_char ,R1 ,32);
	memcpy(&(input_char[strlen(input_char)]) ,R2 ,32);

	str_to_hex(input_char ,input_hex);

	memcpy(input_key_char ,W ,32);
	str_to_hex(input_key_char ,input_key_hex);

	h_5(input_key_hex ,input_hex);
        return 0;
}

