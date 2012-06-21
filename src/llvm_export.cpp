/*
	Dummy function used to ensure that the library exports llvm-c symbols.
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

void core() {
	unsigned u = 0;
	unsigned long long ull = 0;
	LLVMTypeRef Ty = 0;
	LLVMTypeKind typekind = LLVMVoidTypeKind;
	LLVMBool b = 0;
	char * str = 0;
	LLVMValueRef Val = 0;
	LLVMContextRef C = 0;
	LLVMModuleRef M = 0;
	LLVMModuleProviderRef MP = 0;
	LLVMUseRef U = 0;
	LLVMBuilderRef B = 0, Builder = 0;
	LLVMBasicBlockRef BB = 0;
	LLVMPassRegistryRef R = 0;
	LLVMAttribute PA = LLVMZExtAttribute;
	LLVMIntPredicate Predicate = LLVMIntEQ;
	LLVMRealPredicate RealPredicate = LLVMRealPredicateFalse;
	LLVMLinkage Linkage = LLVMExternalLinkage;
	LLVMVisibility Viz = LLVMDefaultVisibility;
	LLVMPassManagerRef PM = 0;
	LLVMPassManagerBuilderRef PMB = 0;
	LLVMMemoryBufferRef MemBuf = 0;
	LLVMOpcode Op;
	LLVMExecutionEngineRef EE = 0;
	LLVMGenericValueRef GV = 0;
	void * ptr = 0;
	LLVMTargetDataRef TDR = 0;
	LLVMVerifierFailureAction Action = LLVMPrintMessageAction;
	
	
	LLVMInitializeCore(R);
	LLVMInitializeTransformUtils(R);
	LLVMInitializeScalarOpts(R);
	LLVMInitializeInstCombine(R);
	LLVMInitializeIPO(R);
	//LLVMInitializeInstrumentation(R);
	LLVMInitializeAnalysis(R);
	LLVMInitializeIPA(R);
	LLVMInitializeCodeGen(R);
	LLVMInitializeTarget(R);
	
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
	LLVMSetSection(Val, str7);
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
	
	// analysis
	b = LLVMVerifyModule(M, Action, &str);
	b = LLVMVerifyFunction(Val, Action);
	LLVMViewFunctionCFG(Val);
	LLVMViewFunctionCFGOnly(Val);
	
	// bitreader
	b = LLVMParseBitcode(MemBuf, &M, &str);
	b = LLVMParseBitcodeInContext(C, MemBuf, &M, &str);
	b = LLVMGetBitcodeModuleInContext(C, MemBuf, &M, &str);
	b = LLVMGetBitcodeModule(MemBuf, &M, &str);
	b = LLVMGetBitcodeModuleProviderInContext(C, MemBuf, &MP, &str);
	b = LLVMGetBitcodeModuleProvider(MemBuf, &MP, &str);
	
	// bitwriter
	u = LLVMWriteBitcodeToFile(M, str1);
	u = LLVMWriteBitcodeToFD(M, u, u, u);
	u = LLVMWriteBitcodeToFileHandle(M, u);
	
	// ee
	LLVMLinkInJIT();
	//LLVMLinkInInterpreter();

	LLVMCreateGenericValueOfInt(Ty, ull, b);
	GV = LLVMCreateGenericValueOfPointer(ptr);
	GV = LLVMCreateGenericValueOfFloat(Ty, u);
	u = LLVMGenericValueIntWidth(GV);
	ull = LLVMGenericValueToInt(GV, b);
	ptr = LLVMGenericValueToPointer(GV);
	u = LLVMGenericValueToFloat(Ty, GV);
	LLVMDisposeGenericValue(GV);

	b = LLVMCreateExecutionEngineForModule(&EE, M, &str);
	b = LLVMCreateInterpreterForModule(&EE, M, &str);
	b = LLVMCreateJITCompilerForModule(&EE, M, u, &str);

	LLVMDisposeExecutionEngine(EE);
	LLVMRunStaticConstructors(EE);
	LLVMRunStaticDestructors(EE);
	
	u = LLVMRunFunctionAsMain(EE, Val, u, &str2, &str3);

	GV = LLVMRunFunction(EE, Val, u, &GV);

	LLVMFreeMachineCodeForFunction(EE, Val);
	LLVMAddModule(EE, M);
	b = LLVMRemoveModule(EE, M, &M, &str);
	b = LLVMFindFunction(EE, str4, &Val);
	ptr = LLVMRecompileAndRelinkFunction(EE, Val);

	TDR = LLVMGetExecutionEngineTargetData(EE);

	LLVMAddGlobalMapping(EE, Val, ptr);

	ptr = LLVMGetPointerToGlobal(EE, Val);
	
	// target
	LLVMInitializeX86TargetInfo();
    LLVMInitializeX86Target();
    LLVMInitializeX86TargetMC();


	TDR = LLVMCreateTargetData(str8);
	LLVMAddTargetData(TDR, PM);
	str1 = LLVMCopyStringRepOfTargetData(TDR);
	LLVMByteOrder(TDR);
	u = LLVMPointerSize(TDR);
	Ty = LLVMIntPtrType(TDR);
	ull = LLVMSizeOfTypeInBits(TDR, Ty);
	ull = LLVMStoreSizeOfType(TDR, Ty);
	ull = LLVMABISizeOfType(TDR, Ty);
	u = LLVMABIAlignmentOfType(TDR, Ty);
	u = LLVMCallFrameAlignmentOfType(TDR, Ty);
	u =  LLVMPreferredAlignmentOfType(TDR, Ty);
	u =  LLVMPreferredAlignmentOfGlobal(TDR, Val);	
	u = LLVMElementAtOffset(TDR, Ty, ull);
	ull = LLVMOffsetOfElement(TDR, Ty, u);

	//? LLVMInvalidateStructLayout(TDR, Ty);
	LLVMDisposeTargetData(TDR);
	
	// transforms
	/** See llvm::PassManagerBuilder. */
	PMB = LLVMPassManagerBuilderCreate();
	LLVMPassManagerBuilderDispose(PMB);

	/** See llvm::PassManagerBuilder::OptLevel. */
	LLVMPassManagerBuilderSetOptLevel(PMB, u);

	/** See llvm::PassManagerBuilder::SizeLevel. */
	LLVMPassManagerBuilderSetSizeLevel(PMB, u);

	/** See llvm::PassManagerBuilder::DisableUnitAtATime. */
	LLVMPassManagerBuilderSetDisableUnitAtATime(PMB, b);

	/** See llvm::PassManagerBuilder::DisableUnrollLoops. */
	LLVMPassManagerBuilderSetDisableUnrollLoops(PMB, b);

	/** See llvm::PassManagerBuilder::DisableSimplifyLibCalls */
	LLVMPassManagerBuilderSetDisableSimplifyLibCalls(PMB, b);

	/** See llvm::PassManagerBuilder::Inliner. */
	LLVMPassManagerBuilderUseInlinerWithThreshold(PMB, u);

	/** See llvm::PassManagerBuilder::populateFunctionPassManager. */
	LLVMPassManagerBuilderPopulateFunctionPassManager(PMB, PM);

	/** See llvm::PassManagerBuilder::populateModulePassManager. */
	LLVMPassManagerBuilderPopulateModulePassManager(PMB, PM);

	/** See llvm::PassManagerBuilder::populateLTOPassManager. */
	LLVMPassManagerBuilderPopulateLTOPassManager(PMB, PM, b, b);
		/** See llvm::createArgumentPromotionPass function. */
	LLVMAddArgumentPromotionPass(PM);

	/** See llvm::createConstantMergePass function. */
	LLVMAddConstantMergePass(PM);

	/** See llvm::createDeadArgEliminationPass function. */
	LLVMAddDeadArgEliminationPass(PM);

	/** See llvm::createFunctionAttrsPass function. */
	LLVMAddFunctionAttrsPass(PM);

	/** See llvm::createFunctionInliningPass function. */
	LLVMAddFunctionInliningPass(PM);

	/** See llvm::createAlwaysInlinerPass function. */
	LLVMAddAlwaysInlinerPass(PM);

	/** See llvm::createGlobalDCEPass function. */
	LLVMAddGlobalDCEPass(PM);

	/** See llvm::createGlobalOptimizerPass function. */
	LLVMAddGlobalOptimizerPass(PM);

	/** See llvm::createIPConstantPropagationPass function. */
	LLVMAddIPConstantPropagationPass(PM);

	/** See llvm::createPruneEHPass function. */
	LLVMAddPruneEHPass(PM);

	/** See llvm::createIPSCCPPass function. */
	LLVMAddIPSCCPPass(PM);

	/** See llvm::createInternalizePass function. */
	LLVMAddInternalizePass(PM, u);

	/** See llvm::createStripDeadPrototypesPass function. */
	LLVMAddStripDeadPrototypesPass(PM);

	/** See llvm::createStripSymbolsPass function. */
	LLVMAddStripSymbolsPass(PM);

	/** See llvm::createAggressiveDCEPass function. */
	LLVMAddAggressiveDCEPass(PM);

	/** See llvm::createCFGSimplificationPass function. */
	LLVMAddCFGSimplificationPass(PM);

	/** See llvm::createDeadStoreEliminationPass function. */
	LLVMAddDeadStoreEliminationPass(PM);

	/** See llvm::createGVNPass function. */
	LLVMAddGVNPass(PM);

	/** See llvm::createIndVarSimplifyPass function. */
	LLVMAddIndVarSimplifyPass(PM);

	/** See llvm::createInstructionCombiningPass function. */
	LLVMAddInstructionCombiningPass(PM);

	/** See llvm::createJumpThreadingPass function. */
	LLVMAddJumpThreadingPass(PM);

	/** See llvm::createLICMPass function. */
	LLVMAddLICMPass(PM);

	/** See llvm::createLoopDeletionPass function. */
	LLVMAddLoopDeletionPass(PM);

	/** See llvm::createLoopIdiomPass function */
	LLVMAddLoopIdiomPass(PM);

	/** See llvm::createLoopRotatePass function. */
	LLVMAddLoopRotatePass(PM);

	/** See llvm::createLoopUnrollPass function. */
	LLVMAddLoopUnrollPass(PM);

	/** See llvm::createLoopUnswitchPass function. */
	LLVMAddLoopUnswitchPass(PM);

	/** See llvm::createMemCpyOptPass function. */
	LLVMAddMemCpyOptPass(PM);

	/** See llvm::createPromoteMemoryToRegisterPass function. */
	LLVMAddPromoteMemoryToRegisterPass(PM);

	/** See llvm::createReassociatePass function. */
	LLVMAddReassociatePass(PM);

	/** See llvm::createSCCPPass function. */
	LLVMAddSCCPPass(PM);

	/** See llvm::createScalarReplAggregatesPass function. */
	LLVMAddScalarReplAggregatesPass(PM);

	/** See llvm::createScalarReplAggregatesPass function. */
	LLVMAddScalarReplAggregatesPassSSA(PM);

	/** See llvm::createScalarReplAggregatesPass function. */
	LLVMAddScalarReplAggregatesPassWithThreshold(PM, u);

	/** See llvm::createSimplifyLibCallsPass function. */
	LLVMAddSimplifyLibCallsPass(PM);

	/** See llvm::createTailCallEliminationPass function. */
	LLVMAddTailCallEliminationPass(PM);

	/** See llvm::createConstantPropagationPass function. */
	LLVMAddConstantPropagationPass(PM);

	/** See llvm::demotePromoteMemoryToRegisterPass function. */
	LLVMAddDemoteMemoryToRegisterPass(PM);

	/** See llvm::createVerifierPass function. */
	LLVMAddVerifierPass(PM);

	/** See llvm::createCorrelatedValuePropagationPass function */
	LLVMAddCorrelatedValuePropagationPass(PM);

	/** See llvm::createEarlyCSEPass function */
	LLVMAddEarlyCSEPass(PM);

	/** See llvm::createLowerExpectIntrinsicPass function */
	LLVMAddLowerExpectIntrinsicPass(PM);

	/** See llvm::createTypeBasedAliasAnalysisPass function */
	LLVMAddTypeBasedAliasAnalysisPass(PM);

	/** See llvm::createBasicAliasAnalysisPass function */
	LLVMAddBasicAliasAnalysisPass(PM);
	
	//LLVMAddDeadTypeEliminationPass(PM);
	
	
}


