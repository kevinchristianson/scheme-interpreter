CC = clang
CFLAGS = -g

SRCS = linkedlist.c main.c talloc.c tokenizer.c
HDRS = linkedlist.h value.h talloc.h tokenizer.h
OBJS = $(SRCS:.c=.o)

tokenizer: $(OBJS)
	$(CC)  $(CFLAGS) $^  -o $@

%.o : %.c $(HDRS)
	$(CC)  $(CFLAGS) -c $<  -o $@

clean:
	rm *.o
	rm tokenizer

