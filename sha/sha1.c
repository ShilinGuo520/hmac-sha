#include "stdio.h"
#include "string.h"

unsigned int K[4] = {0x5a827999 ,0x6ed9eba1 ,0x8f1bbcdc ,0xca62c1d6};

void input_format(unsigned char *st_out ,unsigned char *st, int len)
{	
	int i;
	unsigned int int_len;
	if(len < 56) {
		for (i = 0 ;i < len ;i++) {
			st_out[i] = st[i];
		}

		st_out[i++] = 0x80;

		for ( ;i < 60 ; i++) {
			st_out[i] = 0x00;
		}
		int_len = len;
		int_len = int_len * 8;
		st_out[63] = (unsigned char) (int_len);
		st_out[62] = (unsigned char) (int_len >> 8);
		st_out[61] = (unsigned char) (int_len >> 16);
		st_out[60] = (unsigned char) (int_len >> 24);
	} else if (len < 120) {
		for (i = 0 ;i < len ;i++) {
			st_out[i] = st[i];
		}
		st_out[i++] = 0x80;
		int_len = len;
		int_len = int_len * 8;
		st_out[127] = (unsigned char) (int_len);
		st_out[126] = (unsigned char) (int_len >> 8);
		st_out[125] = (unsigned char) (int_len >> 16);
		st_out[124] = (unsigned char) (int_len >> 24); 
	} else if (len < 184) {
                for (i = 0 ;i < len ;i++) {
                        st_out[i] = st[i];
                }   
                st_out[i++] = 0x80;
                int_len = len;
                int_len = int_len * 8;
                st_out[191] = (unsigned char) (int_len);
                st_out[190] = (unsigned char) (int_len >> 8); 
                st_out[189] = (unsigned char) (int_len >> 16);
                st_out[188] = (unsigned char) (int_len >> 24); 
	} else {
                for (i = 0 ;i < len ;i++) {
                        st_out[i] = st[i];
                }   
                st_out[i++] = 0x80;
                int_len = len;
                int_len = int_len * 8;
                st_out[255] = (unsigned char) (int_len);
                st_out[254] = (unsigned char) (int_len >> 8); 
                st_out[253] = (unsigned char) (int_len >> 16);
                st_out[252] = (unsigned char) (int_len >> 24); 
	}
}

void input_to_msg(unsigned int *msg ,unsigned char *st_in ,int len)
{
	int i;
	unsigned char st[256];
	memset(st ,0 ,256);
	input_format(st ,st_in ,len);
	for(i=0 ;i < 64 ;i++) {
		msg[i] = ((st[4*i]<<24)|(st[4*i + 1] << 16)|(st[4*i + 2] << 8)|st[4*i + 3]);
	}
}

#define fun_ch(x ,y ,z) ((x & y) ^ ((~x) & z))
#define fun_maj(x ,y ,z) ((x & y) ^ (x & z) ^ (y & z))
#define parity(x ,y ,z) (x ^ y ^ z)
#define ROTL(n ,x) ((x << n) | (x >> (32-n)))
#define ROTR(n ,x) ((x >> n) | (x << (32-n)))


void sha_1_hash(unsigned int *h_in ,unsigned int *h_out ,unsigned int *msg)
{
	int t ;
	unsigned int w[80] ;
	unsigned int a ,b ,c ,d ,e ;
	unsigned int T;
	for (t = 0 ;t < 16 ;t++) {
		w[t] = msg[t];
	} 
	for ( ;t < 80; t++) {
		w[t] = ROTL(1 ,(w[t-3] ^ w[t-8] ^ w[t-14] ^ w[t-16]));
	}
	a = h_in[0];
	b = h_in[1];
	c = h_in[2];
	d = h_in[3];
	e = h_in[4];
	for (t = 0 ;t < 80 ;t++) {
		if(t < 20)
			T = ROTL(5 ,a) + fun_ch(b ,c ,d) + e + K[t/20] + w[t];
		else if(t < 40)
			T = ROTL(5 ,a) + parity(b ,c ,d) + e + K[t/20] + w[t];
		else if(t < 60)
			T = ROTL(5 ,a) + fun_maj(b ,c ,d) + e + K[t/20] + w[t];
		else
			T = ROTL(5 ,a) + parity(b ,c ,d) + e + K[t/20] + w[t];
		e = d;
		d = c;
		c= ROTL(30 ,b);
		b = a;
		a = T;
	}
	h_out[0] = a + h_in[0];
	h_out[1] = b + h_in[1];
	h_out[2] = c + h_in[2];
	h_out[3] = d + h_in[3];
	h_out[4] = e + h_in[4];

}


void hash1(unsigned char *in,unsigned int *out)
{
	unsigned char st[256];
	unsigned int h_in[5];
	unsigned int h_out[5];
	unsigned int h_out_2[5];
	unsigned int msg[64];
	int len;
	int i;

	h_in[0] = 0x67452301;
	h_in[1] = 0xefcdab89;
	h_in[2] = 0x98badcfe;
	h_in[3] = 0x10325476;
	h_in[4] = 0xc3d2e1f0;

	memset(st ,0 ,256);
	memcpy(st ,in ,256);

	len = strlen(st);

	printf("sha-1:\ninput size: %d \n" ,len);
	if(len < 56) {
		input_to_msg(msg, st, len);
		sha_1_hash(h_in ,h_out ,msg);
		for(i = 0 ;i < 5 ;i++) {
			printf("%x" ,h_out[i]);
		}
		printf("\n");
		memcpy(out ,h_out ,5);
	} else if (len < 120) {
                input_to_msg(msg, st, len);
                sha_1_hash(h_in ,h_out ,msg);
                sha_1_hash(h_out ,h_out_2 ,&(msg[16]));
		for(i = 0 ;i < 5 ;i++) {
                	printf("%x" ,h_out_2[i]);
                }
                printf("\n");
		memcpy(out ,h_out_2 ,32);
	} else if (len < 184) {
		printf("184 \n");
		input_to_msg(msg, st, len);
                sha_1_hash(h_in ,h_out ,msg);
                sha_1_hash(h_out ,h_out_2 ,&(msg[16]));
		sha_1_hash(h_out_2 ,h_out ,&(msg[32]));
                for(i = 0 ;i < 5 ;i++) {
                        printf("%x" ,h_out[i]);
                }
                printf("\n");		
		memcpy(out ,h_out ,32);
	} else {
                input_to_msg(msg, st, len);
                sha_1_hash(h_in ,h_out ,msg);
                sha_1_hash(h_out ,h_out_2 ,&(msg[16]));
                sha_1_hash(h_out_2 ,h_out ,&(msg[32]));
		sha_1_hash(h_out ,h_out_2 ,&(msg[48]));
                for(i = 0 ;i < 5 ;i++) {
                        printf("%x" ,h_out_2[i]);
                }
                printf("\n");
		memcpy(out ,h_out_2 ,32);
	}
}


void int_to_char(unsigned char *ch, unsigned int *in)
{
        int i;
        for(i = 0 ;i < 32 ;i++) {
                ch[i] = (unsigned char) (in[i >> 2] >> (( 3 - (i%4)) << 3 ) );
        }
}

void str_to_hex(unsigned char *input_char , unsigned char *input_hex)
{
        int i;
        int input_len = strlen(input_char);
        for (i = 0 ;i < input_len ;i++) {
                if(input_char[i] > 57) {
                        input_char[i] = input_char[i] - 87;
                } else {
                        input_char[i] = input_char[i] - 48;
                }
                input_hex[i/2] |= (input_char[i]) << (4*(1 - i%2));
        }
}


int main()
{	
	int i;
	int len;
	unsigned char char_in[512];
	unsigned char hex_in[512];
	unsigned int _out[32];
	
	memset(char_in ,0 ,512);
	memset(hex_in ,0 ,512);

	scanf("%s" ,char_in);
	str_to_hex(char_in ,hex_in);
	len = strlen(hex_in);
	for (i = 0 ;i < len ;i ++) {
		printf("%x" ,hex_in[i]);
	}
	printf("\n");
	hash1(hex_in ,_out);
	return 0;
}
