/*
	Dummy function used to ensure that the library exports symbols
 */
 
#include "llvm-c/Core.h"

void dummy() {
	unsigned u;
	unsigned long long ull;
	LLVMTypeRef Ty;
	LLVMTypeKind typekind;
	LLVMBool b;
	char * str;
	LLVMValueRef Val;
	LLVMContextRef C;
	LLVMModuleRef M;
	LLVMModuleProviderRef MP;
	LLVMUseRef U;
	LLVMBuilderRef B, Builder;
	LLVMBasicBlockRef BB;
	LLVMPassRegistryRef R;
	LLVMAttribute PA;
	LLVMIntPredicate Predicate;
	LLVMRealPredicate RealPredicate;
	LLVMLinkage Linkage;
	LLVMVisibility Viz;
	LLVMPassManagerRef PM;
	LLVMMemoryBufferRef MemBuf;
	LLVMOpcode Op;
	

	LLVMInitializeCore(R);
	LLVMDisposeMessage(str);

	LLVMContextRef LLVMContextCreate();
	LLVMContextRef LLVMGetGlobalContext();
	LLVMContextDispose(C);

	u = LLVMGetMDKindIDInContext(C, str,u);
	u = LLVMGetMDKindID(str, u);

	/* Create and destroy modules. */ 
	/** See llvm::Module::Module. */
	M = LLVMModuleCreateWithName(str);
	M = LLVMModuleCreateWithNameInContext(str, C);
	LLVMDisposeModule(M);

	/** Data layout. See Module::getDataLayout. */
	const char * str1 = LLVMGetDataLayout(M);
	LLVMSetDataLayout(M, str);

	/** Target triple. See Module::getTargetTriple. */
	const char * str2 = LLVMGetTarget(M);
	LLVMSetTarget(M, str);

	/** See Module::dump. */
	LLVMDumpModule(M);

	/** See Module::setModuleInlineAsm. */
	LLVMSetModuleInlineAsm(M, str);

	/** See Module::getContext. */
	C = LLVMGetModuleContext(M);

	/* Operations on integer types */
	Ty = LLVMInt1TypeInContext(C);
	Ty = LLVMInt8TypeInContext(C);
	Ty = LLVMInt16TypeInContext(C);
	Ty = LLVMInt32TypeInContext(C);
	Ty = LLVMInt64TypeInContext(C);
	Ty = LLVMIntTypeInContext(C, u);

	Ty = LLVMInt1Type();
	Ty = LLVMInt8Type();
	Ty = LLVMInt16Type();
	Ty = LLVMInt32Type();
	Ty = LLVMInt64Type();
	Ty = LLVMIntType(u);
	u = LLVMGetIntTypeWidth(Ty);

	/* Operations on real types */
	Ty = LLVMFloatTypeInContext(C);
	Ty = LLVMDoubleTypeInContext(C);
	Ty = LLVMX86FP80TypeInContext(C);
	Ty = LLVMFP128TypeInContext(C);
	Ty = LLVMPPCFP128TypeInContext(C);

	Ty = LLVMFloatType();
	Ty = LLVMDoubleType();
	Ty = LLVMX86FP80Type();
	Ty = LLVMFP128Type();
	Ty = LLVMPPCFP128Type();
	
	typekind = LLVMGetTypeKind(Ty);
	b = LLVMTypeIsSized(Ty);

	/** See llvm::LLVMType::getContext. */
	C = LLVMGetTypeContext(Ty);


	/* Operations on function types */
	Ty = LLVMFunctionType(Ty, &Ty, u, b);
	b = LLVMIsFunctionVarArg(Ty);
	Ty = LLVMGetReturnType(Ty);
	u = LLVMCountParamTypes(Ty);
	LLVMGetParamTypes(Ty, &Ty);

	/* Operations on struct types */
	Ty = LLVMStructTypeInContext(C, &Ty, u, b);
	Ty = LLVMStructType(&Ty, u, b);
	Ty = LLVMStructCreateNamed(C, str);
	const char * str3 = LLVMGetStructName(Ty);
	LLVMStructSetBody(Ty, &Ty, u, b);

	u = LLVMCountStructElementTypes(Ty);
	LLVMGetStructElementTypes(Ty, &Ty);
	b = LLVMIsPackedStruct(Ty);
	b = LLVMIsOpaqueStruct(Ty);

	Ty = LLVMGetTypeByName(M, str);

	/* Operations on array, pointer, and vector types (sequence types) */
	Ty = LLVMArrayType(Ty, u);
	Ty = LLVMPointerType(Ty, u);
	Ty = LLVMVectorType(Ty, u);

	Ty = LLVMGetElementType(Ty);
	u = LLVMGetArrayLength(Ty);
	u = LLVMGetPointerAddressSpace(Ty);
	u = LLVMGetVectorSize(Ty);

	/* Operations on other types */
	Ty = LLVMVoidTypeInContext(C);
	Ty = LLVMLabelTypeInContext(C);
	Ty = LLVMX86MMXTypeInContext(C);

	Ty = LLVMVoidType();
	Ty = LLVMLabelType();
	Ty = LLVMX86MMXType();
	
	/*===-- Values ------------------------------------------------------------===*/


	/* Operations on all values */
	Ty = LLVMTypeOf(Val);
	const char * str4 = LLVMGetValueName(Val);
	LLVMSetValueName(Val, str);
	LLVMDumpValue(Val);
	LLVMReplaceAllUsesWith(Val, Val);
	u = LLVMHasMetadata(Val);
	Val = LLVMGetMetadata(Val, u);
	LLVMSetMetadata(Val, u, Val);

	/* Conversion functions. Return the input value if it is an instance of the
	   specified class, otherwise NULL. See llvm::dyn_cast_or_null<>. */
	#define LLVM_DECLARE_VALUE_CAST1(name) \
	  Val = LLVMIsA##name(Val);
	LLVM_FOR_EACH_VALUE_SUBCLASS(LLVM_DECLARE_VALUE_CAST1)

	/* Operations on Uses */
	U = LLVMGetFirstUse(Val);
	U = LLVMGetNextUse(U);
	Val = LLVMGetUser(U);
	Val = LLVMGetUsedValue(U);

	/* Operations on Users */
	Val = LLVMGetOperand(Val, u);
	LLVMSetOperand(Val, u, Val);
	u = LLVMGetNumOperands(Val);

	/* Operations on constants of any type */
	Val = LLVMConstNull(Ty); /* all zeroes */
	Val = LLVMConstAllOnes(Ty); /* only for int/vector */
	Val = LLVMGetUndef(Ty);
	b = LLVMIsConstant(Val);
	b = LLVMIsNull(Val);
	b = LLVMIsUndef(Val);
	Val = LLVMConstPointerNull(Ty);

	/* Operations on metadata */
	Val = LLVMMDStringInContext(C, str, u);
	Val = LLVMMDString(str, u);
	Val = LLVMMDNodeInContext(C, &Val, u);
	Val = LLVMMDNode(&Val, u);
//	const char * str5 = LLVMGetMDString(Val, &u);
//	u = LLVMGetMDNodeNumOperands(Val);
//	Val = *LLVMGetMDNodeOperand(Val, u);
//	u = LLVMGetNamedMetadataNumOperands(M, str);
//	LLVMGetNamedMetadataOperands(M, str, &Val);

	/* Operations on scalar constants */
	Val = LLVMConstInt(Ty, ull, b);
	const uint64_t * words = 0;
	Val = LLVMConstIntOfArbitraryPrecision(Ty, u, words);
	Val = LLVMConstIntOfString(Ty, str, u);
	Val = LLVMConstIntOfStringAndSize(Ty, str, u, u);
	Val = LLVMConstReal(Ty, u);
	Val = LLVMConstRealOfString(Ty, str);
	Val = LLVMConstRealOfStringAndSize(Ty, str, u);
	u = LLVMConstIntGetZExtValue(Val);
	u = LLVMConstIntGetSExtValue(Val);


	/* Operations on composite constants */
	Val = LLVMConstStringInContext(C, str, u, b);
	Val = LLVMConstStructInContext(C, &Val, u, b);

	Val = LLVMConstString(str, u, b);
	Val = LLVMConstArray(Ty, &Val, u);
	Val = LLVMConstStruct(&Val, u, b);
	Val = LLVMConstNamedStruct(Ty, &Val, u);
	Val = LLVMConstVector(&Val, u);

	/* Constant expressions */
	Op = LLVMGetConstOpcode(Val);
	Val = LLVMAlignOf(Ty);
	Val = LLVMSizeOf(Ty);
	Val = LLVMConstNeg(Val);
	Val = LLVMConstNSWNeg(Val);
	Val = LLVMConstNUWNeg(Val);
	Val = LLVMConstFNeg(Val);
	Val = LLVMConstNot(Val);
	Val = LLVMConstAdd(Val, Val);
	Val = LLVMConstNSWAdd(Val, Val);
	Val = LLVMConstNUWAdd(Val, Val);
	Val = LLVMConstFAdd(Val, Val);
	Val = LLVMConstSub(Val, Val);
	Val = LLVMConstNSWSub(Val, Val);
	Val = LLVMConstNUWSub(Val, Val);
	Val = LLVMConstFSub(Val, Val);
	Val = LLVMConstMul(Val, Val);
	Val = LLVMConstNSWMul(Val, Val);
	Val = LLVMConstNUWMul(Val, Val);
	Val = LLVMConstFMul(Val, Val);
	Val = LLVMConstUDiv(Val, Val);
	Val = LLVMConstSDiv(Val, Val);
	Val = LLVMConstExactSDiv(Val, Val);
	Val = LLVMConstFDiv(Val, Val);
	Val = LLVMConstURem(Val, Val);
	Val = LLVMConstSRem(Val, Val);
	Val = LLVMConstFRem(Val, Val);
	Val = LLVMConstAnd(Val, Val);
	Val = LLVMConstOr(Val, Val);
	Val = LLVMConstXor(Val, Val);
	Val = LLVMConstICmp(Predicate, Val, Val);
	Val = LLVMConstFCmp(RealPredicate, Val, Val);
	Val = LLVMConstShl(Val, Val);
	Val = LLVMConstLShr(Val, Val);
	Val = LLVMConstAShr(Val, Val);
	Val = LLVMConstGEP(Val, &Val, u);
	Val = LLVMConstInBoundsGEP(Val, &Val, u);
	Val = LLVMConstTrunc(Val, Ty);
	Val = LLVMConstSExt(Val, Ty);
	Val = LLVMConstZExt(Val, Ty);
	Val = LLVMConstFPTrunc(Val, Ty);
	Val = LLVMConstFPExt(Val, Ty);
	Val = LLVMConstUIToFP(Val, Ty);
	Val = LLVMConstSIToFP(Val, Ty);
	Val = LLVMConstFPToUI(Val, Ty);
	Val = LLVMConstFPToSI(Val, Ty);
	Val = LLVMConstPtrToInt(Val, Ty);
	Val = LLVMConstIntToPtr(Val, Ty);
	Val = LLVMConstBitCast(Val, Ty);
	Val = LLVMConstZExtOrBitCast(Val,Ty);
	Val = LLVMConstSExtOrBitCast(Val,Ty);
	Val = LLVMConstTruncOrBitCast(Val, Ty);
	Val = LLVMConstPointerCast(Val,  Ty);
	Val = LLVMConstIntCast(Val, Ty, b);
	Val = LLVMConstFPCast(Val, Ty);
	Val = LLVMConstSelect(Val, Val, Val);
	Val = LLVMConstExtractElement(Val, Val);
	Val = LLVMConstInsertElement(Val, Val, Val); 
	Val = LLVMConstShuffleVector(Val, Val, Val);
	Val = LLVMConstExtractValue(Val, &u, u);
	Val = LLVMConstInsertValue(Val, Val, &u, u);
	Val = LLVMConstInlineAsm(Ty, str, str, b, b);
	Val = LLVMBlockAddress(Val, BB);

	/* Operations on global variables, functions, and aliases (globals) */
	M = LLVMGetGlobalParent(Val);
	b = LLVMIsDeclaration(Val);
	Linkage = LLVMGetLinkage(Val);
	LLVMSetLinkage(Val, Linkage);
	const char * str7 = LLVMGetSection(Val);
	LLVMSetSection(Val, str);
	Viz = LLVMGetVisibility(Val);
	LLVMSetVisibility(Val, Viz);
	u = LLVMGetAlignment(Val);
	LLVMSetAlignment(Val, u);

	/* Operations on global variables */
	Val = LLVMAddGlobal(M, Ty, str);
	Val = LLVMAddGlobalInAddressSpace(M, Ty,str, u);
	Val = LLVMGetNamedGlobal(M, str);
	Val = LLVMGetFirstGlobal(M);
	Val = LLVMGetLastGlobal(M);
	Val = LLVMGetNextGlobal(Val);
	Val = LLVMGetPreviousGlobal(Val);
	LLVMDeleteGlobal(Val);
	Val = LLVMGetInitializer(Val);
	LLVMSetInitializer(Val, Val);
	b = LLVMIsThreadLocal(Val);
	LLVMSetThreadLocal(Val, b);
	b = LLVMIsGlobalConstant(Val);
	LLVMSetGlobalConstant(Val, b);

	/* Operations on aliases */
	Val = LLVMAddAlias(M, Ty, Val, str);

	/* Operations on functions */
	Val = LLVMAddFunction(M, str, Ty);
	Val = LLVMGetNamedFunction(M, str);
	Val = LLVMGetFirstFunction(M);
	Val = LLVMGetLastFunction(M);
	Val = LLVMGetNextFunction(Val);
	Val = LLVMGetPreviousFunction(Val);
	LLVMDeleteFunction(Val);
	u = LLVMGetIntrinsicID(Val);
	u = LLVMGetFunctionCallConv(Val);
	LLVMSetFunctionCallConv(Val, u);
	const char * str8 = LLVMGetGC(Val);
	LLVMSetGC(Val, str);
	LLVMAddFunctionAttr(Val, PA);
	PA = LLVMGetFunctionAttr(Val);
	LLVMRemoveFunctionAttr(Val, PA);

	/* Operations on parameters */
	u = LLVMCountParams(Val);
	LLVMGetParams(Val, &Val);
	Val = LLVMGetParam(Val, u);
	Val = LLVMGetParamParent(Val);
	Val = LLVMGetFirstParam(Val);
	Val = LLVMGetLastParam(Val);
	Val = LLVMGetNextParam(Val);
	Val = LLVMGetPreviousParam(Val);
	LLVMAddAttribute(Val, PA);
	LLVMRemoveAttribute(Val, PA);
	PA = LLVMGetAttribute(Val);
	LLVMSetParamAlignment(Val, u);

	/* Operations on basic blocks */
	Val = LLVMBasicBlockAsValue(BB);
	b = LLVMValueIsBasicBlock(Val);
	BB = LLVMValueAsBasicBlock(Val);
	Val = LLVMGetBasicBlockParent(BB);
	Val = LLVMGetBasicBlockTerminator(BB);
	u = LLVMCountBasicBlocks(Val);
	LLVMGetBasicBlocks(Val, &BB);
	BB = LLVMGetFirstBasicBlock(Val);
	BB = LLVMGetLastBasicBlock(Val);
	BB = LLVMGetNextBasicBlock(BB);
	BB = LLVMGetPreviousBasicBlock(BB);
	BB = LLVMGetEntryBasicBlock(Val);

	BB = LLVMAppendBasicBlockInContext(C,
													Val,
													str);
	BB = LLVMInsertBasicBlockInContext(C,
													BB,
													str);

	BB = LLVMAppendBasicBlock(Val, str);
	BB = LLVMInsertBasicBlock(BB, str);
	LLVMDeleteBasicBlock(BB);
	LLVMRemoveBasicBlockFromParent(BB);

	LLVMMoveBasicBlockBefore(BB, BB);
	LLVMMoveBasicBlockAfter(BB, BB);

	Val = LLVMGetFirstInstruction(BB);
	Val = LLVMGetLastInstruction(BB);

	/* Operations on instructions */
	BB = LLVMGetInstructionParent(Val);
	Val = LLVMGetNextInstruction(Val);
	Val = LLVMGetPreviousInstruction(Val);
//	LLVMInstructionEraseFromParent(Val);
//	Op =  LLVMGetInstructionOpcode(Val);
//	Predicate = LLVMGetICmpPredicate(Val);

	/* Operations on call sites */
	LLVMSetInstructionCallConv(Val, u);
	u = LLVMGetInstructionCallConv(Val);
	LLVMAddInstrAttribute(Val, u, PA);
	LLVMRemoveInstrAttribute(Val, u, PA);
	LLVMSetInstrParamAlignment(Val, u, u);

	/* Operations on call instructions (only) */
	b = LLVMIsTailCall(Val);
	LLVMSetTailCall(Val, b);

	/* Operations on switch instructions (only) */
	BB = LLVMGetSwitchDefaultDest(Val);

	/* Operations on phi nodes */
	LLVMAddIncoming(Val, &Val, &BB, u);
	u = LLVMCountIncoming(Val);
	Val = LLVMGetIncomingValue(Val, u);
	BB = LLVMGetIncomingBlock(Val, u);
	
	/*===-- Instruction builders ----------------------------------------------===*/

	Builder = LLVMCreateBuilderInContext(C);
	Builder = LLVMCreateBuilder();
	LLVMPositionBuilder(Builder, BB,
							 Val);
	LLVMPositionBuilderBefore(Builder, Val);
	LLVMPositionBuilderAtEnd(Builder, BB);
	BB = LLVMGetInsertBlock(B);
	LLVMClearInsertionPosition(B);
	LLVMInsertIntoBuilder(B, Val);
	LLVMInsertIntoBuilderWithName(B, Val,
									   str);
	LLVMDisposeBuilder(B);

	/* Metadata */
	LLVMSetCurrentDebugLocation(Builder, Val);
	Val = LLVMGetCurrentDebugLocation(Builder);
	LLVMSetInstDebugLocation(Builder, Val);

	/* Terminators */
	Val = LLVMBuildRetVoid(B);
	Val = LLVMBuildRet(B, Val);
	Val = LLVMBuildAggregateRet(B, &Val, u);
	Val = LLVMBuildBr(B, BB);
	Val = LLVMBuildCondBr(B, Val, BB, BB);
	Val = LLVMBuildSwitch(B, Val, BB, u);
	Val = LLVMBuildIndirectBr(B, Val, u);
	Val = LLVMBuildInvoke(B, Val, &Val, u, BB, BB, str);
	Val = LLVMBuildLandingPad(B, Ty, Val, u,  str);
	Val = LLVMBuildResume(B, Val);
	Val = LLVMBuildUnreachable(B);

	LLVMAddCase(Val, Val,
					 BB);

	LLVMAddDestination(Val, BB);

	LLVMAddClause(Val, Val);

	LLVMSetCleanup(Val, b);

	Val = LLVMBuildAdd(B, Val, Val,
							  str);
	Val = LLVMBuildNSWAdd(B, Val, Val,
								 str);
	Val = LLVMBuildNUWAdd(B, Val, Val,
								 str);
	Val = LLVMBuildFAdd(B, Val, Val,
							   str);
	Val = LLVMBuildSub(B, Val, Val,
							  str);
	Val = LLVMBuildNSWSub(B, Val, Val,
								 str);
	Val = LLVMBuildNUWSub(B, Val, Val,
								 str);
	Val = LLVMBuildFSub(B, Val, Val,
							   str);
	Val = LLVMBuildMul(B, Val, Val,
							  str);
	Val = LLVMBuildNSWMul(B, Val, Val,
								 str);
	Val = LLVMBuildNUWMul(B, Val, Val,
								 str);
	Val = LLVMBuildFMul(B, Val, Val,
							   str);
	Val = LLVMBuildUDiv(B, Val, Val,
							   str);
	Val = LLVMBuildSDiv(B, Val, Val,
							   str);
	Val = LLVMBuildExactSDiv(B, Val, Val,
									str);
	Val = LLVMBuildFDiv(B, Val, Val,
							   str);
	Val = LLVMBuildURem(B, Val, Val,
							   str);
	Val = LLVMBuildSRem(B, Val, Val,
							   str);
	Val = LLVMBuildFRem(B, Val, Val,
							   str);
	Val = LLVMBuildShl(B, Val, Val,
							   str);
	Val = LLVMBuildLShr(B, Val, Val,
							   str);
	Val = LLVMBuildAShr(B, Val, Val,
							   str);
	Val = LLVMBuildAnd(B, Val, Val,
							  str);
	Val = LLVMBuildOr(B, Val, Val,
							  str);
	Val = LLVMBuildXor(B, Val, Val,
							  str);
	Val = LLVMBuildBinOp(B, Op, Val, Val, str);
	Val = LLVMBuildNeg(B, Val, str);
	Val = LLVMBuildNSWNeg(B, Val,
								 str);
	Val = LLVMBuildNUWNeg(B, Val,
								 str);
	Val = LLVMBuildFNeg(B, Val, str);
	Val = LLVMBuildNot(B, Val, str);

	/* Memory */
	Val = LLVMBuildMalloc(B, Ty, str);
	Val = LLVMBuildArrayMalloc(B, Ty,
									  Val, str);
	Val = LLVMBuildAlloca(B, Ty, str);
	Val = LLVMBuildArrayAlloca(B, Ty,
									  Val, str);
	Val = LLVMBuildFree(B, Val);
	Val = LLVMBuildLoad(B, Val,
							   str);
	Val = LLVMBuildStore(B, Val, Val);
	Val = LLVMBuildGEP(B, Val,
							  &Val, u,
							  str);
	Val = LLVMBuildInBoundsGEP(B, Val,
									  &Val, u,
									  str);
	Val = LLVMBuildStructGEP(B, Val,
									u, str);
	Val = LLVMBuildGlobalString(B, str, str);
	Val = LLVMBuildGlobalStringPtr(B, str, str);

	/* Casts */
	Val = LLVMBuildTrunc(B, Val,
								Ty, str);
	Val = LLVMBuildZExt(B, Val,
							   Ty, str);
	Val = LLVMBuildSExt(B, Val,
							   Ty, str);
	Val = LLVMBuildFPToUI(B, Val,
								 Ty, str);
	Val = LLVMBuildFPToSI(B, Val,
								 Ty, str);
	Val = LLVMBuildUIToFP(B, Val,
								 Ty, str);
	Val = LLVMBuildSIToFP(B, Val,
								 Ty, str);
	Val = LLVMBuildFPTrunc(B, Val,
								  Ty, str);
	Val = LLVMBuildFPExt(B, Val,
								Ty, str);
	Val = LLVMBuildPtrToInt(B, Val,
								   Ty, str);
	Val = LLVMBuildIntToPtr(B, Val,
								   Ty, str);
	Val = LLVMBuildBitCast(B, Val,
								  Ty, str);
	Val = LLVMBuildZExtOrBitCast(B, Val,
										Ty, str);
	Val = LLVMBuildSExtOrBitCast(B, Val,
										Ty, str);
	Val = LLVMBuildTruncOrBitCast(B, Val,
										 Ty, str);
	Val = LLVMBuildCast(B, Op, Val,
							   Ty, str);
	Val = LLVMBuildPointerCast(B, Val,
									  Ty, str);
	Val = LLVMBuildIntCast(B, Val, /*Signed cast!*/
								  Ty, str);
	Val = LLVMBuildFPCast(B, Val,
								 Ty, str);

	/* Comparisons */
	Val = LLVMBuildICmp(B, Predicate,
							   Val, Val,
							   str);
	Val = LLVMBuildFCmp(B, RealPredicate,
							   Val, Val,
							   str);

	/* Miscellaneous instructions */
	Val = LLVMBuildPhi(B, Ty, str);
	Val = LLVMBuildCall(B, Val,
							   &Val, u,
							   str);
	Val = LLVMBuildSelect(B, Val,
								 Val, Val,
								 str);
	Val = LLVMBuildVAArg(B, Val, Ty,
								str);
	Val = LLVMBuildExtractElement(B, Val,
										 Val, str);
	Val = LLVMBuildInsertElement(B, Val,
										Val, Val,
										str);
	Val = LLVMBuildShuffleVector(B, Val,
										Val, Val,
										str);
	Val = LLVMBuildExtractValue(B, Val,
									   u, str);
	Val = LLVMBuildInsertValue(B, Val,
									  Val, u,
									  str);

	Val = LLVMBuildIsNull(B, Val,
								 str);
	Val = LLVMBuildIsNotNull(B, Val,
									str);
	Val = LLVMBuildPtrDiff(B, Val,
								  Val, str);


	MP = LLVMCreateModuleProviderForExistingModule(M);

	LLVMDisposeModuleProvider(MP);


	b = LLVMCreateMemoryBufferWithContentsOfFile(str,
													  &MemBuf,
													  &str);
	b = LLVMCreateMemoryBufferWithSTDIN(&MemBuf,
											 &str);
	LLVMDisposeMemoryBuffer(MemBuf);

	R = LLVMGetGlobalPassRegistry();

	PM = LLVMCreatePassManager();

	PM = LLVMCreateFunctionPassManagerForModule(M);

	PM = LLVMCreateFunctionPassManager(MP);

	b = LLVMRunPassManager(PM, M);

	b = LLVMInitializeFunctionPassManager(PM);

	b = LLVMRunFunctionPassManager(PM, Val);

	b = LLVMFinalizeFunctionPassManager(PM);

	LLVMDisposePassManager(PM);
}


