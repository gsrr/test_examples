CFILES:=$(shell ls * | grep .c)
OBJECT_FILE = $(CFILES:%.c=%.o)
SO_OBJECT_FILE = $(CFILES:%.c=%.so)

$(info CFILES = $(CFILES))
$(info OBJECT_FILE = $(OBJECT_FILE))
$(info SO_OBJECT_FILE = $(SO_OBJECT_FILE))

%.so : %.o
	gcc -shared $< -o $@

%.o : %.c
	gcc -fPIC -c -o $@ $<
	
all : $(SO_OBJECT_FILE)
	gcc -o main main.c -ldl
	
unittest : $(CFILES)
	gcc -o $(basename $<) $< -DUNIT_TEST

clean: $(CFILES)
	rm $(basename $<)
	rm *.so


