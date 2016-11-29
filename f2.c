#include "stdio.h"
#include "string.h"


#define fun_r1(x) (((x >> 17)|(x << 15)) ^ ((x >> 19)|(x << 13)) ^ (x >> 10))
#define fun_r0(x) (((x >> 7)|(x << 25)) ^ ((x >> 18)|(x << 14)) ^ (x >> 3))
#define fun_e1(x) (((x >> 6)|(x << 26)) ^ ((x >> 11)|(x << 21)) ^ ((x >> 25)|(x << 7)))
#define fun_e0(x) (((x >> 2)|(x << 30)) ^ ((x >> 13)|(x << 19)) ^ ((x >> 22)|(x << 10)))
#define fun_ch(x , y ,z) ((x & y) ^ ((~x) & z))
#define fun_maj(x , y ,z) ((x & y) ^ (x & z) ^ (y & z))



/***The input_u input_v input_z input_x reference Core_v4.2.pdf page 1570 ***/
#define F2_P_192 1
#if F2_P_192
unsigned char w[] = "fb3ba2012c7e62466e486e229290175b4afebc13fdccee46";
#define W_LEN 48
#else
unsigned char w[] = "ec0234a357c8ad05341010a60a397d9b99796b13b4f866f1868d34f373bfa698";
#define W_LEN 64
#endif
unsigned char n1[] = "d5cb8454d177733effffb2ec712baeab";
unsigned char n2[] = "a6e8e7cc25a75f6e216583f7ff3dc4cf";
unsigned char keyID[] = "62746c6b";
unsigned char A1[] = "56123737bfce";
unsigned char A2[] = "a713702dcfc1";
/*****************************************************************************/

unsigned int K[64] = {	\
0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5, \
0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174, \
0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da, \
0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967, \
0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85, \
0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070, \
0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3, \
0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2, };

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

void sha_256_hash(unsigned int *h_in ,unsigned int *h_out ,unsigned int *msg)
{
	int t;
	unsigned int w[64];
	unsigned int a,b,c,d,e,f,g,h;
	unsigned int T1,T2;

	a = h_in[0];
	b = h_in[1];
	c = h_in[2];
	d = h_in[3];
	e = h_in[4];
	f = h_in[5];
	g = h_in[6];
	h = h_in[7];

	for (t = 0 ;t < 64 ;t++) {
		if(t < 16) {
			w[t] = msg[t];
		} else {
			w[t] = fun_r1(w[t-2]) + w[t-7] + fun_r0(w[t-15]) + w[t-16];
		}
	}
	for (t = 0 ;t < 64 ;t++) {
		T1 = h + fun_e1(e) + fun_ch(e ,f ,g) + K[t] + w[t];
		T2 = fun_e0(a) + fun_maj(a ,b ,c);
		h = g;
		g = f;
		f = e;
		e = d + T1;
		d = c;
		c = b;
		b = a;
		a = T1 + T2;
	}

	h_out[0] = a + h_in[0];
	h_out[1] = b + h_in[1];
	h_out[2] = c + h_in[2];
	h_out[3] = d + h_in[3];
	h_out[4] = e + h_in[4];
	h_out[5] = f + h_in[5];
	h_out[6] = g + h_in[6];
	h_out[7] = h + h_in[7];
	
}

void hash256(unsigned char *in,unsigned int *out,int len)
{
	unsigned char st[256];
	unsigned int h_in[8];
	unsigned int h_out[8];
	unsigned int h_out_2[8];
	unsigned int msg[64];
	int i;

	h_in[0] = 0x6a09e667;
	h_in[1] = 0xbb67ae85;
	h_in[2] = 0x3c6ef372;
	h_in[3] = 0xa54ff53a;
	h_in[4] = 0x510e527f;
	h_in[5] = 0x9b05688c;
	h_in[6] = 0x1f83d9ab;
	h_in[7] = 0x5be0cd19;

	memset(st ,0 ,256);
	memcpy(st ,in ,256);

	printf("%d \n",len);
	if(len < 56) {
		input_to_msg(msg, st, len);
		sha_256_hash(h_in ,h_out ,msg);
		memcpy(out ,h_out ,32);
	} else if (len < 120) {
                input_to_msg(msg, st, len);
                sha_256_hash(h_in ,h_out ,msg);
                sha_256_hash(h_out ,h_out_2 ,&(msg[16]));
		memcpy(out ,h_out_2 ,32);
	} else if (len < 184) {
		input_to_msg(msg, st, len);
                sha_256_hash(h_in ,h_out ,msg);
                sha_256_hash(h_out ,h_out_2 ,&(msg[16]));
		sha_256_hash(h_out_2 ,h_out ,&(msg[32]));
		memcpy(out ,h_out ,32);
	} else {
                input_to_msg(msg, st, len);
                sha_256_hash(h_in ,h_out ,msg);
                sha_256_hash(h_out ,h_out_2 ,&(msg[16]));
                sha_256_hash(h_out_2 ,h_out ,&(msg[32]));
		sha_256_hash(h_out ,h_out_2 ,&(msg[48]));
		memcpy(out ,h_out_2 ,32);
	}
}

void int_to_char(unsigned char *ch, unsigned int *in)
{
	int i;
	for(i = 0 ;i < 32 ;i++) {
		ch[i] = (unsigned char) (in[i/4] >> 8 * ( 3 - (i%4)) );
	}
}

int f_2(unsigned char *input_key ,unsigned char *input_msg)
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

	hash256(st_in_hash ,out_data ,112);
	int_to_char(out_char ,out_data);

	memset(st_in_hash ,0 ,512);
	for(i = 0 ;i < 64 ;i++) {
		st_in_hash[i] = o_key_pad[i];
	}
	for( ;i < 96 ;i++) {
		st_in_hash[i] = out_char[i - 64];
	}
	memset(out_data ,0 ,32);

	hash256(st_in_hash ,out_data, 96);

	int_to_char(out_char ,out_data);

	printf("hmac-sha256:\n");

	for(i = 0 ;i < 4 ;i++) {
		printf("%x" ,out_data[i]);
	}
	printf("\n");
	return 0;
}

void str_to_hex(unsigned char *input_char , unsigned char *input_hex)
{
	int i;
        int input_len = strlen(input_char);
	printf("input_len %d \n",input_len);
        for (i = 0 ;i < input_len ;i++) {
                if(input_char[i] > 57) {
                        input_char[i] = input_char[i] - 87; 
                } else {
                        input_char[i] = input_char[i] - 48; 
                }   
                input_hex[i/2] |= input_char[i] << 4*(1 - i%2);
        }
}

int main()
{
        unsigned char input_char[512];
        int input_len;
        unsigned char input_hex[256];
	unsigned char input_key_char[128];
	unsigned char input_key_hex[64];

        memset(input_char ,0 ,512);
        memset(input_hex ,0 ,128);
	memset(input_key_char ,0 ,128);
	memset(input_key_hex ,0 ,64);

	memcpy(input_char ,n1 ,32);
	memcpy(&(input_char[strlen(input_char)]) ,n2 ,32);
	memcpy(&(input_char[strlen(input_char)]) ,keyID ,8);
	memcpy(&(input_char[strlen(input_char)]) ,A1 ,12);
	memcpy(&(input_char[strlen(input_char)]) ,A2 ,12);

	str_to_hex(input_char ,input_hex);

	memcpy(input_key_char ,w ,W_LEN);
	str_to_hex(input_key_char ,input_key_hex);

	f_2(input_key_hex ,input_hex);
        return 0;
}

