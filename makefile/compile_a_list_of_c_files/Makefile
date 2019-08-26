

CFILES:=$(shell ls -d lib/* | grep .c)
$(info CFILES = $(CFILES)) 
OBJECT_FILE = $(CFILES:%.c=%.o)
$(info OBJECT_FILE = $(OBJECT_FILE)) 

all: $(OBJECT_FILE)
	gcc main.c $(OBJECT_FILE) -o main

clean:
	rm $(OBJECT_FILE)
	rm main
