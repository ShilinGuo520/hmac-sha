#include "stdio.h"
#include "string.h"

/****************************************************************************/
/***The input_u input_v input_z input_x reference Core_v4.2.pdf page 1570 ***/
unsigned char keyID[] = "6274646b";
unsigned char A1[] = "56123737bfce";
unsigned char A2[] = "a713702dcfc1";
unsigned char input[] = "6274646b56123737bfcea713702dcfc1";
unsigned char W[] = "c234c1198f3b520186ab92a2f874934e";

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

	hash256(st_in_hash ,out_data ,80);

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
	
	printf("Device Authentication Key:\n");

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

	memcpy(input_char ,keyID ,8);
	memcpy(&(input_char[strlen(input_char)]) ,A1 ,12);
	memcpy(&(input_char[strlen(input_char)]) ,A2 ,12);

	str_to_hex(input_char ,input_hex);

	memcpy(input_key_char ,W ,32);
	str_to_hex(input_key_char ,input_key_hex);

	f_1(input_key_hex ,input_hex);
        return 0;
}

