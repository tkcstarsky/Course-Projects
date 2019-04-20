#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Constant.h>


#include <memory>
using namespace llvm;
LLVMContext theContext;

Value * getNum(int n) {
	return ConstantInt::get(theContext, APInt(32, n));
}//get the const number

int main() {
	// create the module fib_gen
	IRBuilder<> builder(theContext);
	auto theModule = new Module("fib_gen", theContext);
	std::vector<Type*> args(1, Type::getInt32Ty(theContext));
	FunctionType *FT = FunctionType::get(Type::getInt32Ty(theContext), args, false);
	
	/*
		declare the function<int fib(int n)>
	*/
	Function *F = Function::Create(FT, GlobalValue::LinkageTypes::ExternalLinkage, "fib", theModule);
	Value *arg = &(*(F->arg_begin()));	//add var
	arg->setName("n");					//var name "n"

	//BB:entry
	auto entry = BasicBlock::Create(theContext, "entry", F);			//create BB:entry
	builder.SetInsertPoint(entry);
	auto argValue = arg;
	auto ret = builder.CreateAlloca(Type::getInt32Ty(theContext));		//ret get return value
	ret->setName("res");

	auto Ret = BasicBlock::Create(theContext, "result", F);				//create BB:result
	auto then1 = BasicBlock::Create(theContext, "n0", F);				//create BB:n0
	auto else1 = BasicBlock::Create(theContext, "nibr", F);				//create BB:n1br

	auto if_stat = builder.CreateICmpEQ(argValue, getNum(0));			//tmp1 <= if(n==0)?
	if_stat->setName("tmp1");
	builder.CreateCondBr(if_stat, then1, else1);						//create cond branch 

	//BB:n0
	builder.SetInsertPoint(then1);
	builder.CreateAlignedStore(getNum(0), ret, 4);						//ret = 0
	builder.CreateBr(Ret);

	//BB:n1br
	builder.SetInsertPoint(else1);
	if_stat = builder.CreateICmpEQ(argValue, getNum(1));				//tmp2 <= if(n==1)?
	if_stat->setName("tmp2");
	then1 = BasicBlock::Create(theContext, "n1", F);
	else1 = BasicBlock::Create(theContext, "n2", F);
	builder.CreateCondBr(if_stat, then1, else1);						//create cond branch 
	
	//BB:n1
	builder.SetInsertPoint(then1);
	builder.CreateAlignedStore(getNum(1), ret, 4);						//ret = 1
	builder.CreateBr(Ret);

	//BB:n2
	builder.SetInsertPoint(else1);
	auto sub1 = builder.CreateNSWSub(argValue, getNum(1));
	sub1->setName("nn1");
	auto call1 = builder.CreateCall(F, { sub1 });
	call1->setName("back1");
	auto sub2 = builder.CreateNSWSub(argValue, getNum(2));
	sub2->setName("nn2");
	auto call2 = builder.CreateCall(F, { sub2 });
	call2->setName("back2");
	auto add = builder.CreateNSWAdd(call1, call2);
	add->setName("n2res");
	builder.CreateAlignedStore(add, ret, 4);							//ret=F(n-1)+F(n-2)
	builder.CreateBr(Ret);

	//BB:result
	builder.SetInsertPoint(Ret);
	auto result = builder.CreateAlignedLoad(ret, 4);
	result->setName("r");
	builder.CreateRet(result);											//return ret

	/*
		declare the function<int main()>
	*/
	FT = FunctionType::get(Type::getInt32Ty(theContext), false);
	auto M = Function::Create(FT, GlobalValue::LinkageTypes::ExternalLinkage, "main", theModule);
	entry = BasicBlock::Create(theContext, "entry", M);//Create BB:entry

	//BB:entry
	builder.SetInsertPoint(entry);

	auto xp = builder.CreateAlloca(Type::getInt32Ty(theContext));		//allocate x
	xp->setName("x");
	auto ip = builder.CreateAlloca(Type::getInt32Ty(theContext));		//allocate i
	ip->setName("i");
	builder.CreateAlignedStore(getNum(0), xp, 4);						//init x = 0
	builder.CreateAlignedStore(getNum(1), ip, 4);						//init i = 1

	auto forstat = BasicBlock::Create(theContext, "loop", M);			//create all BB
	auto forloop = BasicBlock::Create(theContext, "fibadd", M);
	auto foripp = BasicBlock::Create(theContext, "countadd", M);
	Ret = BasicBlock::Create(theContext, "quit", M);

	builder.CreateBr(forstat);											//go to BB:loop

	//BB:loop
	builder.SetInsertPoint(forstat);
	auto ic = builder.CreateAlignedLoad(ip, 4);
	ic->setName("ic");
	if_stat = builder.CreateICmpSLT(ic, getNum(8));
	if_stat->setName("count");
	builder.CreateCondBr(if_stat, forloop, Ret);

	//BB:fibadd
	builder.SetInsertPoint(forloop);
	auto i1 = builder.CreateAlignedLoad(ip, 4);
	i1->setName("icall");
	auto fibres = builder.CreateCall(F, { i1 });
	fibres->setName("fibres");
	auto x = builder.CreateAlignedLoad(xp, 4);
	x->setName("xcur");
	auto x1 = builder.CreateNSWAdd(x, fibres);
	x1->setName("xres");
	builder.CreateAlignedStore(x1, xp, 4);
	builder.CreateBr(foripp);

	//BB:countadd
	builder.SetInsertPoint(foripp);
	auto i2 = builder.CreateAlignedLoad(ip, 4);
	i2->setName("ip");
	auto i3 = builder.CreateNSWAdd(i2, getNum(1));
	i3->setName("ipp");
	builder.CreateAlignedStore(i3, ip, 4);
	builder.CreateBr(forstat);

	//BB:quit
	builder.SetInsertPoint(Ret);
	auto r = builder.CreateAlignedLoad(xp, 4);
	r->setName("res");
	builder.CreateRet(r);


	//print module
	theModule->print(outs(), nullptr);

	//delete module
	delete theModule;
	return 0;
}
