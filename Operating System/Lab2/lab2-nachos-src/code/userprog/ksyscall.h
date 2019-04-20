/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__
#define __USERPROG_KSYSCALL_H__

#include "kernel.h"

void SysHalt()
{
  kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

int SysSub(int op1, int op2)
{
  return op1 - op2;
}

int SysRead(char *buff, int length, int fid)
{
  return kernel->fileSystem->Read(buff, length, fid);
}
int SysWrite(char *buff, int length, int fid)
{
  return kernel->fileSystem->Write(buff, length, fid);
}

// TODO:实现SysExec函数，该函数的作用是，子进程根据fileName从磁盘载入二进制文件到内存，
// 并覆盖原有父进程的内存以及寄存器数据,然后执行用户程序。若fileName文件不存在，
// 则认为该fileName是Linux的内置命令，在Linux上处理完后，结束当前线程.
void SysExec(char *fileName)
{
  kernel->currentThread->setName(fileName);//设定新产生的子进程名
  { //begin: 实验二的代码请写在这里++++++++++++++++++++++++++++++++++++++++++++++++++
	kernel->currentThread->space->RestoreState();//恢复存储空间
    kernel->currentThread->space->reset();//清空内存
    if (kernel->currentThread->space->Load(fileName)) //补全这个if
    {      //载入二进制文件成功
	kernel->currentThread->space->Execute();//运行
    }
    else
    {                                //载入二进制文件失败
      printf("exec cmd by Linux\n"); //把该命令作为Linux的内置命令
      system(fileName);              //交给Linux上处理
      // TODO 结束当前线程.
    }
	kernel->currentThread->Finish();//
  } //end: ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

// TODO:该函数可以用作子进程在被创建后的执行体，需要将子进程自己的寄存器数据和进程空间恢复到机器.
void forked(int arg)
{
  { //begin: 实验二的代码请写在这里++++++++++++++++++++++++++++++++++++++++++++++++++
    kernel->currentThread->RestoreUserState();
    kernel->currentThread->space->RestoreState();
	//恢复寄存器状态与空间
    kernel->machine->WriteRegister(2,arg);//arg已被代入为0
    kernel->machine->PCplusPlus();
    //PC指向下一条指令
	kernel->machine->Run();//开始执行
  } //end: ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

// TODO:实现SysFork函数，该函数的作用是，新建一个Thread对象t，为t新建一个进程空间，
// 将父进程的内存数据拷贝给t（AddrSpace::copyMemory），以及将父进程寄存器数据拷贝给t，
// 给子进程分配栈空间（Thread::Fork）,最后返回子进程的id
int SysFork()
{
  { //begin: 实验二的代码请写在这里+++++++++++++++++++++++++++++++++++++++++++++++++
    
    Thread* child=new Thread("Forked thread");

    child->space=new AddrSpace;//为子进程新建进程空间
    child->parent=kernel->currentThread;//将当前父进程的指针赋值给子进程类内的parent指针
    child->space->copyMemory(child->space, kernel->currentThread->space);//将父进程的内存数据拷贝给child
    child->SaveUserState();//保存当前寄存器状态到child
    child->Fork((VoidFunctionPtr)forked,(void *)0);//调用Fork完成子进程内操作，代入返回值0
    kernel->currentThread->Yield();//为下个进程运行做好调度准备

    return child->getTid();//返回子进程的tid
    return 0;

  } //end: ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

#endif /* ! __USERPROG_KSYSCALL_H__ */
