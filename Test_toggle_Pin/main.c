
#define MMIO_BASE       0x3F000000

#define GPFSEL1         ((volatile unsigned int*)(MMIO_BASE+0x00200004))
#define GPPUD           ((volatile unsigned int*)(MMIO_BASE+0x00200094))
#define GPSET0          ((volatile unsigned int*)(MMIO_BASE+0x0020001C))
#define GPCLR0          ((volatile unsigned int*)(MMIO_BASE+0x00200028))


int main(){
	
	register unsigned int r;
	unsigned int tmp;
		
    r = *GPFSEL1;				// get register
    r &= ~(7<<24); 				// clear GPIO18
    r |= (1<<24);   			// set GPIO18 as output
    *GPFSEL1 = r;				// write register
	
    *GPPUD = 0;     			// disable pullup/down

	while(1){
		r = *GPSET0;				// get register
		r |= (1<<18);				// set GPIO18
		*GPSET0 = r;				// write register 	
	
		tmp = 0xFFFF; do{asm volatile("nop");}while(tmp--);

		r = *GPCLR0;				// get register
		r |= (1<<18);				// set GPIO18
		*GPCLR0 = r;				// write register 	
		
		tmp = 0xFFFF; do{asm volatile("nop");}while(tmp--);
	}
	return 0;
}