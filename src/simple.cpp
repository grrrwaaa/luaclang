/*
 *  simple.cpp
 *  clang
 *
 *  Created by Graham Wakefield on 5/13/12.
 *  Copyright 2012 UCSB. All rights reserved.
 *
 */


#include "llvm-c/Analysis.h"
#include "llvm-c/BitReader.h"
#include "llvm-c/BitWriter.h"
#include "llvm-c/Core.h"
#include "llvm-c/ExecutionEngine.h"
#include "llvm-c/Initialization.h"
#include "llvm-c/Transforms/IPO.h"
#include "llvm-c/Transforms/PassManagerBuilder.h"
#include "llvm-c/Transforms/Scalar.h"

int main() {

	LLVMInitializeX86TargetInfo();
    LLVMInitializeX86Target();
    LLVMInitializeX86TargetMC();
	LLVMLinkInJIT();
	
	LLVMContextRef Ctx = LLVMGetGlobalContext();

	LLVMModuleRef M = LLVMModuleCreateWithNameInContext("test", Ctx);

	LLVMBuilderRef B = LLVMCreateBuilder();

	LLVMDumpModule(M);
	
	LLVMExecutionEngineRef EE;
	char * err;
	LLVMCreateJITCompilerForModule(&EE, M, 1, &err);
	printf("%s\n", err);

	LLVMDisposeModule(M);
	
	printf("hello\n");
	return 0;
}