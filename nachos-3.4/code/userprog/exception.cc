// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
	int type = machine->ReadRegister(2);
	int pc;
	
	switch(which)
	{
	    case NoException:
	    {
	    	printf("Everything ok!\n");
	    	break;
	    }
	    case PageFaultException:
	    {
	    	printf("No valid translation found\n");
	    	interrupt->Halt();
	    	break;
	    }
	    case OverflowException:
	    {
	    	printf("Integer overflow in add or sub.\n");
	    	interrupt->Halt();
	    	break;
	    }
	    case ReadOnlyException:
	    {
	    	printf("Write attempted to page marked \"read-only\"\n");
	    	interrupt->Halt();
	    	break;
	    }
	    case BusErrorException:
	    {
	    	printf("Translation resulted in an invalid physical address\n");
	    	interrupt->Halt();
	    	break;
	    }
	    case AddressErrorException:
	    {
	    	printf("Unaligned reference or one that was beyond the end of the address space\n");
	    	interrupt->Halt();
	    	break;
	    }
	    case IllegalInstrException:
	    {
	    	printf("Unimplemented or reserved instr.\n");
	    	interrupt->Halt();
	    	break;
	    }
	    case SyscallException:
	    {
			switch(type)
			{
				case SC_Halt:
				{
					DEBUG('a', "Shutdown, initiated by user program.\n");
		   			interrupt->Halt();
					break;
				}
				case SC_ReadInt:
				{
					DEBUG('a', "Read integer number from console.\n");
					int number = 0;
					int nDigit = 0;
					bool iNum = true;
					int i = 0;
					int temp = 0;
					char* buffer = new char[MAX_INT_LEN];
					int check = 0;
					nDigit = gSynchConsole->Read(buffer, MAX_INT_LEN);
					if(buffer[0] == '-'){
						i = 1;
						check = 1;
					}
					for(;check < nDigit; check++){
						if(buffer[check] != '\n'){
							temp = buffer[check] &NUM_MASK;
							if(temp < 0 || temp > 9)
							{
								iNum = false;
							}
						}
					}
					if(iNum){
						for (; i < nDigit; ++i)
						{
							number = number*10 + (int) (buffer[i] & NUM_MASK);
						}
						
						if(buffer[0] == '-'){
							number = -1* number;
						}
					}
					machine->WriteRegister(2, number);
					delete buffer;
					break;
				}
				case SC_PrintInt:
				{
					DEBUG('a', "Print integer number to console.\n");
					char s[MAX_INT_LEN], neg, tmp;
					neg = '-';
					int i, n, mid, sz;
					i = n = 0;
					DEBUG('a', "Read argument value at r4");
					n = machine->ReadRegister(4);
					if (n < 0)
					{
						gSynchConsole->Write(&neg,1);
						n = -n;
					}
					do {
						s[i++] = n%10 + '0';
					}	while (( n /= 10) > 0);
					sz = i;
					s[sz] = '\n';
					mid = i / 2;
					while (i-->mid)
					{
						tmp = s[sz-i-1];
						s[sz-i-1] = s[i];
						s[i] = tmp;
					}
					gSynchConsole->Write(s, sz);
					break;
				}
				case SC_ReadChar:
				{
					char bufferfer[1];
					int sz = 0;
					sz = gSynchConsole->Read(bufferfer, 1);
					machine->WriteRegister(2, 1);
					break;
				}
				
				case SC_PrintChar:
				{
					char ch;
					ch = (char) machine->ReadRegister(4);
					gSynchConsole->Write(&ch, 1);
					break;
				}
				

				case SC_PrintString:
				{
					DEBUG('a', "PrintString() system call invoked \n");
					int vAdrr = machine->ReadRegister(4);
					char* strConsole;
					strConsole = machine->User2System(vAdrr,3000);
					if (strConsole == NULL)
					{
						machine->WriteRegister(2,-1); // trả về lỗi cho chương
										// trình người dùng
						return;
					}
					int i = 0;
					while (strConsole[i] != '\0')
						i++;
					gSynchConsole->Write(strConsole,i);
					break;
				}
				
				case SC_ReadString:
				{
					char *buffer = new char[MAX_BUFFER_LEN];
					if (buffer == 0) break;
					int bufferAddrUser = machine->ReadRegister(4);
					int length = machine->ReadRegister(5);
					if(length < MAX_BUFFER_LEN)
						length = MAX_BUFFER_LEN;
					int sz = gSynchConsole->Read(buffer, length);
					machine->System2User(bufferAddrUser, sz, buffer);
					delete[] buffer;
					break;
				}
			}//switch
			// Advance program counters.
	    	machine->registers[PrevPCReg] = machine->registers[PCReg];	// for debugging, in case we
							// are jumping into lala-land
	   	 	machine->registers[PCReg] = machine->registers[NextPCReg];
	    	machine->registers[NextPCReg] += 4;
		    break;
	    }
	    default:
	    {
	    	printf("Unexpected user mode exception %d %d\n", which, type);
			ASSERT(FALSE);
			interrupt->Halt();
	    }
	}//switch	
}
