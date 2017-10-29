#include "syscall.h"

int main(){
	char str[100];
	ReadString(str, 100);
	PrintString("Chuoi vua nhap: ");
	PrintString(str);
	return 0;
}