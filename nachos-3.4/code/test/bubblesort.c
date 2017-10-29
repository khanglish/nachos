#include "syscall.h"

int main(){
	int n, i, j, tmp;
	int arr[100];
	PrintString("Nhap mang duoi 100 phan tu:");
	n = ReadInt();
	for (i = 0; i < n; i++) {
		arr[i] = ReadInt();
	} 

	for (i = 0; i < n-1; i++) {
		for (j = i+1; j < n; j++) {
			if (arr[j] < arr[i])
			{
				tmp = arr[j];
				arr[j] = arr[i];
				arr[i] = tmp;
			}
		}
	}
	for (i = 0; i < n; i++) {
		PrintInt(arr[i]);
		PrintChar(' ');
	}
	return 0;

}