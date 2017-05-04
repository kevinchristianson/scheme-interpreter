CC = clang
CFLAGS = -g

SRCS = lib/linkedlist.o main.c lib/talloc.o tokenizer.c
HDRS = linkedlist.h value.h talloc.h tokenizer.h
OBJS = $(SRCS:.c=.o)

tokenizer: $(OBJS)
	$(CC)  $(CFLAGS) $^  -o $@

%.o : %.c $(HDRS)
	$(CC)  $(CFLAGS) -c $<  -o $@

clean:
	rm *.o
	rm tokenizer