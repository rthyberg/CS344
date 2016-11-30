CC=gcc
CFLAGS=-std=gnu89 -Wall -lpthread
ENC=otp_enc
ENCD=otp_enc_d

all:$(ENC) $(ENCD)

$(ENC):
	$(CC) -o $(ENC) $(ENC).c $(CFLAGS)
$(ENCD):
	$(CC) -o $(ENCD) $(ENCD).c $(CFLAGS)
clean:
	rm $(ENC)
	rm $(ENCD)
