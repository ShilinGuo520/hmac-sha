#include "stdio.h"
#include "string.h"

unsigned int K[64] = {	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5, \
			0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174, \
			0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da, \
			0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967, \
			0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85, \
			0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070, \
			0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3, \
			0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2, };


unsigned int msg[16];


void input_format(unsigned char *st, int len)
{
	int i;
	st[len] = 0x80;
	for(i = len + 1 ;i < 63 ;i++)
		st[i] = 0x00;

	st[63] = len * 8;
}

void input_to_msg(unsigned int *msg ,unsigned char *st ,int len)
{
	int i;
	input_format(st ,len);
	for(i=0 ;i < 16 ;i++) {
		msg[i] = ((st[4*i]<<24)|(st[4*i + 1] << 16)|(st[4*i + 2] << 8)|st[4*i + 3]);
	}
}

unsigned int fun_r1(unsigned int x)
{	//add init
	return (((x >> 17)|(x << 15)) ^ ((x >> 19)|(x << 13)) ^ (x >> 10));
}

unsigned int fun_r0(unsigned int x)
{	//add init
	return (((x >> 7)|(x << 25)) ^ ((x >> 18)|(x << 14)) ^ (x >> 3));
}

unsigned int fun_e1(unsigned int x)
{	//add init
	return (((x >> 6)|(x << 26)) ^ ((x >> 11)|(x << 21)) ^ ((x >> 25)|(x << 7)));
}

unsigned int fun_e0(unsigned int x)
{	//add init
	return (((x >> 2)|(x << 30)) ^ ((x >> 13)|(x << 19)) ^ ((x >> 22)|(x << 10)));
}

unsigned int fun_ch(unsigned int x ,unsigned int y ,unsigned int z)
{	//add init
	return ((x & y) ^ ((~x) & z));
}

unsigned int fun_maj(unsigned int x ,unsigned int y ,unsigned int z)
{	//add init
	return ((x & y) ^ (x & z) ^ (y & z));
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
		if(t < 16) 
			w[t] = msg[t];
		else
			w[t] = fun_r1(w[t-2]) + w[t-7] + fun_r0(w[t-15]) + w[t-16]; //call
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

int main()
{
	unsigned char st[64];
	unsigned int h_in[8];
	unsigned int h_out[8];

	int len;
	
	int i;
	h_in[0] = 0x6a09e667;
	h_in[1] = 0xbb67ae85;
	h_in[2] = 0x3c6ef372;
	h_in[3] = 0xa54ff53a;
	h_in[4] = 0x510e527f;
	h_in[5] = 0x9b05688c;
	h_in[6] = 0x1f83d9ab;
	h_in[7] = 0x5be0cd19;


	scanf("%s",st);
	len = strlen(st);
	input_to_msg(msg, st, len);
	sha_256_hash(h_in ,h_out ,msg);
	printf("sha_256:\n");
	for(i = 0 ;i < 8 ;i++) {
		printf("%x" ,h_out[i]);
	}
	printf("\n");

}


