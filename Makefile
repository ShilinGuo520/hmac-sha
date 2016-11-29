CC	:= gcc

all:f1 f2 hmac-sha-256

hmac-sha-256:hmac-sha-256.c
	$(CC) -o $@ $^
f2:f2.c
	$(CC) -o $@ $^
f1:f1.c
	$(CC) -o $@ $^

clean:
	rm f1 f2 hmac-sha-256
