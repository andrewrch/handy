CC     = g++
CFLAGS = -g
LDFLAGS = -lGLEW -lglfw -lboost_program_options -lglog

all: handy

handy: handy_options.o handy.o
	$(CC) -o $@ $^ $(LDFLAGS)

handy.o: src/handy.cc
	$(CC) $(CFLAGS) -c -o $@ $<

handy_options.o: src/handy_options.cc
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f handy handy_options.o handy.o
