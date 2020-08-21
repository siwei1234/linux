CC = gcc
CFLAGS = -g
OBJS = sysutil.o session.o ftpproto.o privparent.o miniftp.o str.o privsock.o parseconf.o tunable.o hash.o
LIBS = -lcrypt
BIN  = miniftp

$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:clean
clean:
	rm -fr *.o $(BIN)