
CFILES:=$(shell ls -d share/* | grep .c)
OBJECT_FILE = $(CFILES:%.c=%.o)
SO_OBJECT_FILE = $(CFILES:%.c=%.so)

$(info OBJECT_FILE = $(OBJECT_FILE)) 
$(info SO_OBJECT_FILE = $(SO_OBJECT_FILE)) 

%.so : %.o
	gcc -shared $< -o $@ 

%.o : %.c
	gcc -fPIC -c -o $@ $<

all : $(SO_OBJECT_FILE)
	gcc -o main main.c -ldl

clean:
	rm -rf share/*.o
	rm -rf share/*.so
	rm main
