#include "syscall.h"

int main(){
	PrintString("------------Thong Tin Nhom------------------");
	PrintString("MSSV: 1512239 | Ho ten: Vu Thua Khang");
	PrintString("MSSV: 1512241 | Ho ten: Phan Nguyen Dong Kha");
	PrintString("----------Chuong trinh ascii----------------");
	PrintString("Du lieu vao 	: Khong co");
	PrintString("Du lieu ra		: bang ma ascii");
	PrintString("Chuc nang		: In bang ma ascii doi voi nhung ki tu in duoc.");
	PrintString("-----------Chuong trinh sort----------------");
	PrintString("Du lieu vao 	: Cac so nguyen (toi da 100 so) do nguoi dung nhap.");
	PrintString("Du lieu ra		: mang so nguyen duoc sap xep");
	PrintString("Chuc nang		: Dung bubble sort de sap xep mang so nguyen do nguoi dung nhap vao.");
	Halt();
	return 0;
}