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
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
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
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	is in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
    int type = kernel->machine->ReadRegister(2);

    DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

    switch (which) {
    case SyscallException:
      switch(type) {

	case SC_Create:
	DEBUG(dbgSys, "Create" << kernel->machine->ReadRegister(4) << "\n");
	
	int c_address,result_create;
	c_address = (int)kernel->machine->ReadRegister(4);
	
	result_create = SysCreate(c_address);
	
	kernel->machine->WriteRegister(2, result_create);
	/* Modify return point */
	{
		/* set previous programm counter (debugging only)*/
		kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

		/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
		kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

		/* set next programm counter for brach execution */
		kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
	}

	return;
	ASSERTNOTREACHED();
	break;

	case SC_Open:
	DEBUG(dbgSys, "Open" << kernel->machine->ReadRegister(4) << "\n");
	
	int o_address,result_open;
	o_address = (int)kernel->machine->ReadRegister(4);
	
	result_open = SysOpen(o_address);
	
	kernel->machine->WriteRegister(2, result_open);
	/* Modify return point */
	{
		/* set previous programm counter (debugging only)*/
		kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

		/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
		kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

		/* set next programm counter for brach execution */
		kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
	}

	return;
	ASSERTNOTREACHED();
	break;

	case SC_Write:
	DEBUG(dbgSys, "Write" << kernel->machine->ReadRegister(4) << kernel->machine->ReadRegister(5) << kernel->machine->ReadRegister(6) << "\n");
	
	int buf, w_size, result_write;
	OpenFileId w_id;
	
	buf = (int)kernel->machine->ReadRegister(4);
	w_size = (int)kernel->machine->ReadRegister(5);
	w_id = (int)kernel->machine->ReadRegister(6);
	
	result_write = SysWrite(buf,w_size,w_id);
	kernel->machine->WriteRegister(2, result_write);
	/* Modify return point */
	{
		/* set previous programm counter (debugging only)*/
		kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

		/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
		kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

		/* set next programm counter for brach execution */
		kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
	}

	return;
	ASSERTNOTREACHED();
	break;

	case SC_Read:
	DEBUG(dbgSys, "Read" << kernel->machine->ReadRegister(4) << kernel->machine->ReadRegister(5) << kernel->machine->ReadRegister(6) << "\n");
	
	int r_buf, r_size, result_read;
	OpenFileId r_id;
	
	r_buf = (int)kernel->machine->ReadRegister(4);
	r_size = (int)kernel->machine->ReadRegister(5);
	r_id = (int)kernel->machine->ReadRegister(6);
	
	result_read = SysRead(r_buf,r_size,r_id);
	kernel->machine->WriteRegister(2, result_read);
	/* Modify return point */
	{
		/* set previous programm counter (debugging only)*/
		kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

		/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
		kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

		/* set next programm counter for brach execution */
		kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
	}

	return;
	ASSERTNOTREACHED();
	break;

	case SC_Close:
	DEBUG(dbgSys, "Close" << kernel->machine->ReadRegister(4) << "\n");
	int id_close,result_close;
	
	id_close = (int)kernel->machine->ReadRegister(4);

	result_close = SysClose(id_close);
	
	kernel->machine->WriteRegister(2, result_close);
	/* Modify return point */
	{
		/* set previous programm counter (debugging only)*/
		kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

		/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
		kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

		/* set next programm counter for brach execution */
		kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
	}

	return;
	ASSERTNOTREACHED();
	break;

	case SC_Sub:
	int subResult,op1,op2;
	op1= (int)kernel->machine->ReadRegister(4); 
	op2= (int)kernel->machine->ReadRegister(5);
	//kernel->machine->WriteRegister(2,(int)subResult);
	subResult =SysSub(op1,op2); 
	printf("%d-%d=%d\n",op1,op2,subResult);
	kernel->machine->WriteRegister(2, (int)subResult);
	
	/* Modify return point */
	{
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	}

	return;
	
	ASSERTNOTREACHED();

	break;


      case SC_Halt:
	DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

	SysHalt();

	ASSERTNOTREACHED();
	break;



      case SC_Add:
	DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");
	
	/* Process SysAdd Systemcall*/
	int result;
	result = SysAdd(/* int op1 */(int)kernel->machine->ReadRegister(4),
			/* int op2 */(int)kernel->machine->ReadRegister(5));

	DEBUG(dbgSys, "Add returning with " << result << "\n");
	/* Prepare Result */
	kernel->machine->WriteRegister(2, (int)result);
	
	/* Modify return point */
	{
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	}

	return;
	
	ASSERTNOTREACHED();

	break;

      default:
	cerr << "Unexpected system call " << type << "\n";
	break;
      }
      break;
    default:
      cerr << "Unexpected user mode exception" << (int)which << "\n";
      break;
    }
    ASSERTNOTREACHED();
}
