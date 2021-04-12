
void putc(int data){
	int *mu_io_reg = 0x3F215040;
	*mu_io_reg = data;
}
