CC= cc

CFLAGS= -g
LDFLAGS= -lm

TARGETS= astar_8

BHEAPOBJS= astar_8.o

all: $(TARGETS)


astar_8: $(BHEAPOBJS)
	$(CC) $(CFLAGS) -o $@ $(BHEAPOBJS) $(LDFLAGS)

.c.o:
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f $(TARGETS) *.o
run:
	./Astar
