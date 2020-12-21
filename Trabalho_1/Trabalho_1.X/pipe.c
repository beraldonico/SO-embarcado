#include "pipe.h"
#include "mm.h"

//Iniciate pipe with dinamic size
void pipe_create(pipe_t* p, u_int size){
	p->buffer        = SRAMalloc(size);
	p->buffer_size   = size;
	p->pos_read      = 0;
	p->pos_write     = 0;    
}

//Read on pipe
u_char pipe_read(pipe_t* p){
	DISABLE_INTERRUPTS();
	u_char data = p->buffer[p->pos_read];
	p->pos_read = (p->pos_read + 1) % (p->buffer_size);
	ENABLE_INTERRUPTS();
	return data;
}

//Write in pipe
//Override existing data
void pipe_write(pipe_t* p, u_char data){
	DISABLE_INTERRUPTS();
	p->buffer[p->pos_write] = data;
	p->pos_write = (p->pos_write + 1) % p->buffer_size;
	ENABLE_INTERRUPTS();
}

