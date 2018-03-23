
CCPATH = /home/cross_compiler/gcc-linaro-7.2.1-2017.11-i686_aarch64-elf/bin
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles

all: clean kernel8.img

start.o: start.S
	$(CCPATH)/aarch64-elf-gcc $(CFLAGS) -c start.S -o start.o

%.o: %.c
	$(CCPATH)/aarch64-elf-gcc $(CFLAGS) -c $< -o $@

kernel8.img: start.o $(OBJS)
	$(CCPATH)/aarch64-elf-ld -nostdlib -nostartfiles start.o $(OBJS) -T link.ld -o kernel8.elf -Map kernel8.map
	$(CCPATH)/aarch64-elf-objcopy -O binary kernel8.elf kernel8.img

clean:
	rm kernel8.elf *.o >/dev/null 2>/dev/null || true
