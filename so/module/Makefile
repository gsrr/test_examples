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
	gcc -o adapter adapter.c -ldl -DUNIT_TEST -luLinux_hal -luLinux_ini -luLinux_hal_tr -L/root/4.5.0/NasX86/Model/TS-X88/build/RootFS/lib
	
unittest : $(CFILES)
	gcc -o $(basename $<) $< -DUNIT_TEST -luLinux_hal -luLinux_ini -luLinux_hal_tr -L/root/4.5.0/NasX86/Model/TS-X88/build/RootFS/lib

clean: $(CFILES)
	rm $(basename $<)
	rm *.so

install:
	mv adapter.so tl_rxx20s.so module/	
	rsync -a module admin@172.17.22.51:/root/
	rsync -a adapter admin@172.17.22.51:/root/
