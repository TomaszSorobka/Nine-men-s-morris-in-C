CC = gcc
CFLAGS = -std=c99 -Wall 
GTK =  $(shell pkg-config gtk+-3.0 --cflags)
LIBS = $(shell pkg-config gtk+-3.0 --libs)

NAME = mlynek

SRC = main.c win-fifo.c logikagry.c gtkdesign.c
OBJS = main.o win-fifo.o logikagry.o gtkdesign.o

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(GTK) $(LIBS)

%.o: %.c 
	$(CC) $(CFLAGS) -c -o  $@ $< $(GTK) $(LIBS)

clean: 
	rm -f $(OBJS) $(NAME)