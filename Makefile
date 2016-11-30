CC	:= gcc

all:f1 f2 f3 h3 h4 h5 g hmac-sha-256

hmac-sha-256:hmac-sha-256.c
	$(CC) -c -o $@ $^


f2:f2.c hmac-sha-256
	$(CC) -o $@ $^
f1:f1.c hmac-sha-256
	$(CC) -o $@ $^
f3:f3.c hmac-sha-256
	$(CC) -o $@ $^
h3:h3.c hmac-sha-256
	$(CC) -o $@ $^
h4:h4.c hmac-sha-256
	$(CC) -o $@ $^
h5:h5.c hmac-sha-256
	$(CC) -o $@ $^
g:g.c hmac-sha-256
	$(CC) -o $@ $^
clean:
	rm f1 f2 f3 h3 h4 h5 g hmac-sha-256
