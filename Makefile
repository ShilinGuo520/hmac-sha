CC	:= gcc

hmac-sha-256:hmac-sha-256.c
	$(CC) -o $@ $^

clean:
	rm hmac-sha-256
