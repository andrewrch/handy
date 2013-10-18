CC     = g++
CFLAGS = -std=c++0x -c -g -I /usr/include/ImageMagick -I /usr/local/include/assimp
LDFLAGS = -lGL -lGLEW -lglfw -lboost_program_options -lboost_filesystem -lboost_system -lglog -lMagick++ -L /usr/local/lib -lassimp

all: handy

handy: handy_options.o texture.o mesh.o pipeline.o shader.o handy.o
	$(CC) -o $@ $^ $(LDFLAGS)

handy.o: src/handy.cc
	$(CC) $(CFLAGS) -c -o $@ $<

handy_options.o: src/handy_options.cc
	$(CC) $(CFLAGS) -c -o $@ $<

texture.o: src/texture.cc
	$(CC) $(CFLAGS) -c -o $@ $<

mesh.o: src/mesh.cc
	$(CC) $(CFLAGS) -c -o $@ $<

shader.o: src/shader.cc
	$(CC) $(CFLAGS) -c -o $@ $<
	
pipeline.o: src/pipeline.cc
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f handy *.o
