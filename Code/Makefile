FFMPEG=/usr/local/ffmpeg
CC=g++

CFLAGS=-g -I $(FFMPEG)/include
LDFLAGS = -L $(FFMPEG)/lib/ -lswscale -lswresample -lavformat -lavdevice -lavcodec -lavutil -lavfilter  -lm 
LDFLAGS+= `pkg-config --libs opencv` 
COMMON+= `pkg-config --cflags opencv`

OBJ=ff_camera.o
EXEC=camera

all:$(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) 
$(OBJ): ff_camera.cpp
	$(CC) $(CFLAGS) -c $< -o $@
 
clean:
	rm -rf $(OBJ) $(EXEC)

