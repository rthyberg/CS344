CC=gcc
CFLAGS=-std=gnu89 -Wall -lpthread
ENC=otp_enc
ENCD=otp_enc_d
KEY=keygen
all:$(ENC) $(ENCD) $(KEY)

$(KEY):
	$(CC) -o $(KEY) $(KEY).c $(CFLAGS)
$(ENC):
	$(CC) -o $(ENC) $(ENC).c $(CFLAGS)
$(ENCD):
	$(CC) -o $(ENCD) $(ENCD).c $(CFLAGS)
clean:
	rm $(ENC)
	rm $(ENCD)
