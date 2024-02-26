# It will compile this obj that will be
# used to read and write at the Interce that was supose to be created

obj-m += level03.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
