CFLAGS=-W -Wall -Isrc -g

all: vt52

vt52: src/sim.o src/cpu.o src/timing.o src/vcd.o
	$(CC) -o $@ $^

clean:
	rm -f vt52 src/*.o
