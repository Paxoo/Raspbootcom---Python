#include "uart.h"
#include "gpio.h"

#define UART0_FR        ((volatile unsigned int*)(MMIO_BASE+0x00201018))
#define UART0_DR        ((volatile unsigned int*)(MMIO_BASE+0x00201000))
#define GPFSEL1         ((volatile unsigned int*)(MMIO_BASE+0x00200004))
#define GPPUD           ((volatile unsigned int*)(MMIO_BASE+0x00200094))
#define GPSET0          ((volatile unsigned int*)(MMIO_BASE+0x0020001C))
#define GPCLR0          ((volatile unsigned int*)(MMIO_BASE+0x00200028))


void main()
{
    char *kernel=(char*)0x81000;
	int size = 0;
	int size_2 = 0;
	
	
	//unsigned int tmp;
	register unsigned int r;
	r = *GPFSEL1;				// get register
    r &= ~((7<<24)|(7<<21)); 	// clear GPIO18, GPIO17
    r |= ((1<<24)|(1<<21));   	// set GPIO18 as output
    *GPFSEL1 = r;				// write register
	
    *GPPUD = 0;     			// disable pullup/down
	r = *GPSET0;				// get register
	r = 0;						// set GPIO18
	*GPSET0 = r;				// write register 
	
    // set up serial console
    uart_init();

    // notify raspbootcom to send the kernel
    uart_send(1);
    uart_send(2);
    uart_send(3);
		
	// read the kernel's size
    size=uart_getc();
    size|=uart_getc()<<8;
    size|=uart_getc()<<16;
    size|=uart_getc()<<24;

	// send negative or positive acknowledge
    if(size<64 || size>1024*1024) {
        // size error
        uart_send('S');
        uart_send('E');
    }else{
		uart_send('O');
		uart_send('K');
		
		size_2 = size;
		
		// read the new kernel	
		int guard = 0;
		
		while(size > 0){
			if(guard == 0){
				// safe Byte in Memory
				if(!(*UART0_FR&0x10)){		
					*kernel = (char)(*UART0_DR);
					guard = 1;
				}
			}else{
				// send Byte from Memory as acknowledgement
				if(!(*UART0_FR&0x20)){		
					*UART0_DR=*kernel++;
					guard = 0;
					size--;
				}
			}
		}	
		
		// Dump Memory 
		kernel=(char*)0x07FC00;
		size_2 = 10000;
		while(size_2 > 0){
			if(!(*UART0_FR&0x20)){
				*UART0_DR=*kernel++;
				size_2--;
			}
		}

		r = *GPSET0;				// get register
		r |= (1<<17);				// set GPIO17
		*GPSET0 = r;				// write register
				
		// jump to the new kernel
		asm volatile ("b 0x081000");
	}
}