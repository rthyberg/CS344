CC=gcc
CFLAGS=-std=gnu89 -Wall -lpthread
ENC=otp_enc
ENCD=otp_enc_d
DECD=otp_dec_d
DEC=otp_dec
KEY=keygen
all:$(ENC) $(ENCD) $(KEY) $(DECD) $(DEC)

$(DEC):
	$(CC) -o $(DEC) $(DEC).c $(CFLAGS)
$(DECD):
	$(CC) -o $(DECD) $(DECD).c $(CFLAGS)
$(KEY):
	$(CC) -o $(KEY) $(KEY).c $(CFLAGS)
$(ENC):
	$(CC) -o $(ENC) $(ENC).c $(CFLAGS)
$(ENCD):
	$(CC) -o $(ENCD) $(ENCD).c $(CFLAGS)
clean:
	rm $(ENC)
	rm $(ENCD)
	rm $(DEC)
	rm $(DECD)
