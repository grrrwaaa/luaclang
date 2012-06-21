local clang = require "clang"
local ffi = require "ffi"
local lib = ffi.C
local header = [[
typedef int LLVMBool;

typedef struct LLVMOpaqueContext *LLVMContextRef;
typedef struct LLVMOpaqueModule *LLVMModuleRef;
typedef struct LLVMOpaqueType *LLVMTypeRef;
typedef struct LLVMOpaqueTypeHandle *LLVMTypeHandleRef;
typedef struct LLVMOpaqueValue *LLVMValueRef;
typedef struct LLVMOpaqueBasicBlock *LLVMBasicBlockRef;
typedef struct LLVMOpaqueBuilder *LLVMBuilderRef;
typedef struct LLVMOpaqueModuleProvider *LLVMModuleProviderRef;
typedef struct LLVMOpaqueMemoryBuffer *LLVMMemoryBufferRef;
typedef struct LLVMOpaquePassManager *LLVMPassManagerRef;
typedef struct LLVMOpaquePassRegistry *LLVMPassRegistryRef;
typedef struct LLVMOpaquePassManagerBuilder *LLVMPassManagerBuilderRef;
typedef struct LLVMOpaqueUse *LLVMUseRef;
typedef struct LLVMOpaqueTargetData *LLVMTargetDataRef;
typedef struct LLVMStructLayout *LLVMStructLayoutRef;
typedef struct LLVMOpaqueGenericValue *LLVMGenericValueRef;
typedef struct LLVMOpaqueExecutionEngine *LLVMExecutionEngineRef;

enum LLVMByteOrdering { LLVMBigEndian, LLVMLittleEndian };

typedef enum {
    LLVMZExtAttribute = 1<<0,
    LLVMSExtAttribute = 1<<1,
    LLVMNoReturnAttribute = 1<<2,
    LLVMInRegAttribute = 1<<3,
    LLVMStructRetAttribute = 1<<4,
    LLVMNoUnwindAttribute = 1<<5,
    LLVMNoAliasAttribute = 1<<6,
    LLVMByValAttribute = 1<<7,
    LLVMNestAttribute = 1<<8,
    LLVMReadNoneAttribute = 1<<9,
    LLVMReadOnlyAttribute = 1<<10,
    LLVMNoInlineAttribute = 1<<11,
    LLVMAlwaysInlineAttribute = 1<<12,
    LLVMOptimizeForSizeAttribute = 1<<13,
    LLVMStackProtectAttribute = 1<<14,
    LLVMStackProtectReqAttribute = 1<<15,
    LLVMAlignment = 31<<16,
    LLVMNoCaptureAttribute = 1<<21,
    LLVMNoRedZoneAttribute = 1<<22,
    LLVMNoImplicitFloatAttribute = 1<<23,
    LLVMNakedAttribute = 1<<24,
    LLVMInlineHintAttribute = 1<<25,
    LLVMStackAlignment = 7<<26
} LLVMAttribute;

typedef enum {
  LLVMRet = 1,
  LLVMBr = 2,
  LLVMSwitch = 3,
  LLVMIndirectBr = 4,
  LLVMInvoke = 5,
  LLVMUnwind = 6,
  LLVMUnreachable = 7,  LLVMAdd = 8,
  LLVMFAdd = 9,
  LLVMSub = 10,
  LLVMFSub = 11,
  LLVMMul = 12,
  LLVMFMul = 13,
  LLVMUDiv = 14,
  LLVMSDiv = 15,
  LLVMFDiv = 16,
  LLVMURem = 17,
  LLVMSRem = 18,
  LLVMFRem = 19,  LLVMShl = 20,
  LLVMLShr = 21,
  LLVMAShr = 22,
  LLVMAnd = 23,
  LLVMOr = 24,
  LLVMXor = 25,  LLVMAlloca = 26,
  LLVMLoad = 27,
  LLVMStore = 28,
  LLVMGetElementPtr = 29,  LLVMTrunc = 30,
  LLVMZExt = 31,
  LLVMSExt = 32,
  LLVMFPToUI = 33,
  LLVMFPToSI = 34,
  LLVMUIToFP = 35,
  LLVMSIToFP = 36,
  LLVMFPTrunc = 37,
  LLVMFPExt = 38,
  LLVMPtrToInt = 39,
  LLVMIntToPtr = 40,
  LLVMBitCast = 41,  LLVMICmp = 42,
  LLVMFCmp = 43,
  LLVMPHI = 44,
  LLVMCall = 45,
  LLVMSelect = 46,  LLVMVAArg = 49,
  LLVMExtractElement = 50,
  LLVMInsertElement = 51,
  LLVMShuffleVector = 52,
  LLVMExtractValue = 53,
  LLVMInsertValue = 54
} LLVMOpcode;

typedef enum {
  LLVMVoidTypeKind,
  LLVMFloatTypeKind,
  LLVMDoubleTypeKind,
  LLVMX86_FP80TypeKind,
  LLVMFP128TypeKind,
  LLVMPPC_FP128TypeKind,
  LLVMLabelTypeKind,
  LLVMIntegerTypeKind,
  LLVMFunctionTypeKind,
  LLVMStructTypeKind,
  LLVMArrayTypeKind,
  LLVMPointerTypeKind,
  LLVMOpaqueTypeKind,
  LLVMVectorTypeKind,
  LLVMMetadataTypeKind,
  LLVMX86_MMXTypeKind
} LLVMTypeKind;

typedef enum {
  LLVMExternalLinkage,
  LLVMAvailableExternallyLinkage,
  LLVMLinkOnceAnyLinkage,
  LLVMLinkOnceODRLinkage,
  LLVMWeakAnyLinkage,
  LLVMWeakODRLinkage,
  LLVMAppendingLinkage,
  LLVMInternalLinkage,
  LLVMPrivateLinkage,
  LLVMDLLImportLinkage,
  LLVMDLLExportLinkage,
  LLVMExternalWeakLinkage,
  LLVMGhostLinkage,
  LLVMCommonLinkage,
  LLVMLinkerPrivateLinkage,
  LLVMLinkerPrivateWeakLinkage,
  LLVMLinkerPrivateWeakDefAutoLinkage
} LLVMLinkage;

typedef enum {
  LLVMDefaultVisibility,
  LLVMHiddenVisibility,
  LLVMProtectedVisibility
} LLVMVisibility;

typedef enum {
  LLVMCCallConv = 0,
  LLVMFastCallConv = 8,
  LLVMColdCallConv = 9,
  LLVMX86StdcallCallConv = 64,
  LLVMX86FastcallCallConv = 65
} LLVMCallConv;

typedef enum {
  LLVMIntEQ = 32,
  LLVMIntNE,
  LLVMIntUGT,
  LLVMIntUGE,
  LLVMIntULT,
  LLVMIntULE,
  LLVMIntSGT,
  LLVMIntSGE,
  LLVMIntSLT,
  LLVMIntSLE
} LLVMIntPredicate;

typedef enum {
  LLVMRealPredicateFalse,
  LLVMRealOEQ,
  LLVMRealOGT,
  LLVMRealOGE,
  LLVMRealOLT,
  LLVMRealOLE,
  LLVMRealONE,
  LLVMRealORD,
  LLVMRealUNO,
  LLVMRealUEQ,
  LLVMRealUGT,
  LLVMRealUGE,
  LLVMRealULT,
  LLVMRealULE,
  LLVMRealUNE,
  LLVMRealPredicateTrue
} LLVMRealPredicate;

void LLVMDisposeMessage(char *Message);

LLVMContextRef LLVMContextCreate(void);
LLVMContextRef LLVMGetGlobalContext(void);
void LLVMContextDispose(LLVMContextRef C);

unsigned LLVMGetMDKindIDInContext(LLVMContextRef C, const char* Name,
                                  unsigned SLen);
unsigned LLVMGetMDKindID(const char* Name, unsigned SLen);

LLVMModuleRef LLVMModuleCreateWithName(const char *ModuleID);
LLVMModuleRef LLVMModuleCreateWithNameInContext(const char *ModuleID,
                                                LLVMContextRef C);
void LLVMDisposeModule(LLVMModuleRef M);
const char *LLVMGetDataLayout(LLVMModuleRef M);
void LLVMSetDataLayout(LLVMModuleRef M, const char *Triple);
const char *LLVMGetTarget(LLVMModuleRef M);
void LLVMSetTarget(LLVMModuleRef M, const char *Triple);
LLVMTypeRef LLVMGetTypeByName(LLVMModuleRef M, const char *Name);
void LLVMDumpModule(LLVMModuleRef M);
void LLVMSetModuleInlineAsm(LLVMModuleRef M, const char *Asm);
LLVMContextRef LLVMGetModuleContext(LLVMModuleRef M);
LLVMTypeKind LLVMGetTypeKind(LLVMTypeRef Ty);
LLVMBool LLVMTypeIsSized(LLVMTypeRef Ty);
LLVMContextRef LLVMGetTypeContext(LLVMTypeRef Ty);
LLVMTypeRef LLVMInt1TypeInContext(LLVMContextRef C);
LLVMTypeRef LLVMInt8TypeInContext(LLVMContextRef C);
LLVMTypeRef LLVMInt16TypeInContext(LLVMContextRef C);
LLVMTypeRef LLVMInt32TypeInContext(LLVMContextRef C);
LLVMTypeRef LLVMInt64TypeInContext(LLVMContextRef C);
LLVMTypeRef LLVMIntTypeInContext(LLVMContextRef C, unsigned NumBits);

LLVMTypeRef LLVMInt1Type(void);
LLVMTypeRef LLVMInt8Type(void);
LLVMTypeRef LLVMInt16Type(void);
LLVMTypeRef LLVMInt32Type(void);
LLVMTypeRef LLVMInt64Type(void);
LLVMTypeRef LLVMIntType(unsigned NumBits);
unsigned LLVMGetIntTypeWidth(LLVMTypeRef IntegerTy);
LLVMTypeRef LLVMFloatTypeInContext(LLVMContextRef C);
LLVMTypeRef LLVMDoubleTypeInContext(LLVMContextRef C);
LLVMTypeRef LLVMX86FP80TypeInContext(LLVMContextRef C);
LLVMTypeRef LLVMFP128TypeInContext(LLVMContextRef C);
LLVMTypeRef LLVMPPCFP128TypeInContext(LLVMContextRef C);

LLVMTypeRef LLVMFloatType(void);
LLVMTypeRef LLVMDoubleType(void);
LLVMTypeRef LLVMX86FP80Type(void);
LLVMTypeRef LLVMFP128Type(void);
LLVMTypeRef LLVMPPCFP128Type(void);
LLVMTypeRef LLVMFunctionType(LLVMTypeRef ReturnType,
                             LLVMTypeRef *ParamTypes, unsigned ParamCount,
                             LLVMBool IsVarArg);
LLVMBool LLVMIsFunctionVarArg(LLVMTypeRef FunctionTy);
LLVMTypeRef LLVMGetReturnType(LLVMTypeRef FunctionTy);
unsigned LLVMCountParamTypes(LLVMTypeRef FunctionTy);
void LLVMGetParamTypes(LLVMTypeRef FunctionTy, LLVMTypeRef *Dest);

LLVMTypeRef LLVMStructTypeInContext(LLVMContextRef C, LLVMTypeRef *ElementTypes,
                                    unsigned ElementCount, LLVMBool Packed);
LLVMTypeRef LLVMStructType(LLVMTypeRef *ElementTypes, unsigned ElementCount,
                           LLVMBool Packed);
LLVMTypeRef LLVMStructCreateNamed(LLVMContextRef C, const char *Name);
const char *LLVMGetStructName(LLVMTypeRef Ty);
void LLVMStructSetBody(LLVMTypeRef StructTy, LLVMTypeRef *ElementTypes,
                       unsigned ElementCount, LLVMBool Packed);

unsigned LLVMCountStructElementTypes(LLVMTypeRef StructTy);
void LLVMGetStructElementTypes(LLVMTypeRef StructTy, LLVMTypeRef *Dest);
LLVMBool LLVMIsPackedStruct(LLVMTypeRef StructTy);
LLVMTypeRef LLVMArrayType(LLVMTypeRef ElementType, unsigned ElementCount);
LLVMTypeRef LLVMPointerType(LLVMTypeRef ElementType, unsigned AddressSpace);
LLVMTypeRef LLVMVectorType(LLVMTypeRef ElementType, unsigned ElementCount);

LLVMTypeRef LLVMGetElementType(LLVMTypeRef Ty);
unsigned LLVMGetArrayLength(LLVMTypeRef ArrayTy);
unsigned LLVMGetPointerAddressSpace(LLVMTypeRef PointerTy);
unsigned LLVMGetVectorSize(LLVMTypeRef VectorTy);
LLVMTypeRef LLVMVoidTypeInContext(LLVMContextRef C);
LLVMTypeRef LLVMLabelTypeInContext(LLVMContextRef C);
LLVMTypeRef LLVMX86MMXTypeInContext(LLVMContextRef C);

LLVMTypeRef LLVMVoidType(void);
LLVMTypeRef LLVMLabelType(void);
LLVMTypeRef LLVMOpaqueType(void);
LLVMTypeRef LLVMX86MMXType(void);
LLVMTypeRef LLVMResolveTypeHandle(LLVMTypeHandleRef TypeHandle);
void LLVMDisposeTypeHandle(LLVMTypeHandleRef TypeHandle);

LLVMTypeRef LLVMTypeOf(LLVMValueRef Val);
const char *LLVMGetValueName(LLVMValueRef Val);
void LLVMSetValueName(LLVMValueRef Val, const char *Name);
void LLVMDumpValue(LLVMValueRef Val);
void LLVMReplaceAllUsesWith(LLVMValueRef OldVal, LLVMValueRef NewVal);
int LLVMHasMetadata(LLVMValueRef Val);
LLVMValueRef LLVMGetMetadata(LLVMValueRef Val, unsigned KindID);
void LLVMSetMetadata(LLVMValueRef Val, unsigned KindID, LLVMValueRef Node);

LLVMValueRef LLVMIsAArgument(LLVMValueRef Val);
 
LLVMValueRef LLVMIsABasicBlock(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAInlineAsm(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAUser(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAConstant(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAConstantAggregateZero(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAConstantArray(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAConstantExpr(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAConstantFP(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAConstantInt(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAConstantPointerNull(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAConstantStruct(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAConstantVector(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAGlobalValue(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAFunction(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAGlobalAlias(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAGlobalVariable(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAUndefValue(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAInstruction(LLVMValueRef Val);
 
LLVMValueRef LLVMIsABinaryOperator(LLVMValueRef Val);
 
LLVMValueRef LLVMIsACallInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAIntrinsicInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsADbgInfoIntrinsic(LLVMValueRef Val);
 
LLVMValueRef LLVMIsADbgDeclareInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAEHSelectorInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAMemIntrinsic(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAMemCpyInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAMemMoveInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAMemSetInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsACmpInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAFCmpInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAICmpInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAExtractElementInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAGetElementPtrInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAInsertElementInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAInsertValueInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAPHINode(LLVMValueRef Val);
 
LLVMValueRef LLVMIsASelectInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAShuffleVectorInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAStoreInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsATerminatorInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsABranchInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAInvokeInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAReturnInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsASwitchInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAUnreachableInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAUnaryInstruction(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAAllocaInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsACastInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsABitCastInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAFPExtInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAFPToSIInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAFPToUIInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAFPTruncInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAIntToPtrInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAPtrToIntInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsASExtInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsASIToFPInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsATruncInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAUIToFPInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAZExtInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAExtractValueInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsALoadInst(LLVMValueRef Val);
 
LLVMValueRef LLVMIsAVAArgInst(LLVMValueRef Val);
LLVMUseRef LLVMGetFirstUse(LLVMValueRef Val);
LLVMUseRef LLVMGetNextUse(LLVMUseRef U);
LLVMValueRef LLVMGetUser(LLVMUseRef U);
LLVMValueRef LLVMGetUsedValue(LLVMUseRef U);
LLVMValueRef LLVMGetOperand(LLVMValueRef Val, unsigned Index);
void LLVMSetOperand(LLVMValueRef User, unsigned Index, LLVMValueRef Val);
int LLVMGetNumOperands(LLVMValueRef Val);
LLVMValueRef LLVMConstNull(LLVMTypeRef Ty);
LLVMValueRef LLVMConstAllOnes(LLVMTypeRef Ty);
LLVMValueRef LLVMGetUndef(LLVMTypeRef Ty);
LLVMBool LLVMIsConstant(LLVMValueRef Val);
LLVMBool LLVMIsNull(LLVMValueRef Val);
LLVMBool LLVMIsUndef(LLVMValueRef Val);
LLVMValueRef LLVMConstPointerNull(LLVMTypeRef Ty);
LLVMValueRef LLVMMDStringInContext(LLVMContextRef C, const char *Str,
                                   unsigned SLen);
LLVMValueRef LLVMMDString(const char *Str, unsigned SLen);
LLVMValueRef LLVMMDNodeInContext(LLVMContextRef C, LLVMValueRef *Vals,
                                 unsigned Count);
LLVMValueRef LLVMMDNode(LLVMValueRef *Vals, unsigned Count);
LLVMValueRef LLVMConstInt(LLVMTypeRef IntTy, unsigned long long N,
                          LLVMBool SignExtend);
LLVMValueRef LLVMConstIntOfArbitraryPrecision(LLVMTypeRef IntTy,
                                              unsigned NumWords,
                                              const uint64_t Words[]);
LLVMValueRef LLVMConstIntOfString(LLVMTypeRef IntTy, const char *Text,
                                  uint8_t Radix);
LLVMValueRef LLVMConstIntOfStringAndSize(LLVMTypeRef IntTy, const char *Text,
                                         unsigned SLen, uint8_t Radix);
LLVMValueRef LLVMConstReal(LLVMTypeRef RealTy, double N);
LLVMValueRef LLVMConstRealOfString(LLVMTypeRef RealTy, const char *Text);
LLVMValueRef LLVMConstRealOfStringAndSize(LLVMTypeRef RealTy, const char *Text,
                                          unsigned SLen);
unsigned long long LLVMConstIntGetZExtValue(LLVMValueRef ConstantVal);
long long LLVMConstIntGetSExtValue(LLVMValueRef ConstantVal);
LLVMValueRef LLVMConstStringInContext(LLVMContextRef C, const char *Str,
                                      unsigned Length, LLVMBool DontNullTerminate);
LLVMValueRef LLVMConstStructInContext(LLVMContextRef C,
                                      LLVMValueRef *ConstantVals,
                                      unsigned Count, LLVMBool Packed);

LLVMValueRef LLVMConstString(const char *Str, unsigned Length,
                             LLVMBool DontNullTerminate);
LLVMValueRef LLVMConstArray(LLVMTypeRef ElementTy,
                            LLVMValueRef *ConstantVals, unsigned Length);
LLVMValueRef LLVMConstStruct(LLVMValueRef *ConstantVals, unsigned Count,
                             LLVMBool Packed);
LLVMValueRef LLVMConstVector(LLVMValueRef *ScalarConstantVals, unsigned Size);
LLVMOpcode LLVMGetConstOpcode(LLVMValueRef ConstantVal);
LLVMValueRef LLVMAlignOf(LLVMTypeRef Ty);
LLVMValueRef LLVMSizeOf(LLVMTypeRef Ty);
LLVMValueRef LLVMConstNeg(LLVMValueRef ConstantVal);
LLVMValueRef LLVMConstNSWNeg(LLVMValueRef ConstantVal);
LLVMValueRef LLVMConstNUWNeg(LLVMValueRef ConstantVal);
LLVMValueRef LLVMConstFNeg(LLVMValueRef ConstantVal);
LLVMValueRef LLVMConstNot(LLVMValueRef ConstantVal);
LLVMValueRef LLVMConstAdd(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstNSWAdd(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstNUWAdd(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstFAdd(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstSub(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstNSWSub(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstNUWSub(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstFSub(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstMul(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstNSWMul(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstNUWMul(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstFMul(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstUDiv(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstSDiv(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstExactSDiv(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstFDiv(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstURem(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstSRem(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstFRem(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstAnd(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstOr(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstXor(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstICmp(LLVMIntPredicate Predicate,
                           LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstFCmp(LLVMRealPredicate Predicate,
                           LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstShl(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstLShr(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstAShr(LLVMValueRef LHSConstant, LLVMValueRef RHSConstant);
LLVMValueRef LLVMConstGEP(LLVMValueRef ConstantVal,
                          LLVMValueRef *ConstantIndices, unsigned NumIndices);
LLVMValueRef LLVMConstInBoundsGEP(LLVMValueRef ConstantVal,
                                  LLVMValueRef *ConstantIndices,
                                  unsigned NumIndices);
LLVMValueRef LLVMConstTrunc(LLVMValueRef ConstantVal, LLVMTypeRef ToType);
LLVMValueRef LLVMConstSExt(LLVMValueRef ConstantVal, LLVMTypeRef ToType);
LLVMValueRef LLVMConstZExt(LLVMValueRef ConstantVal, LLVMTypeRef ToType);
LLVMValueRef LLVMConstFPTrunc(LLVMValueRef ConstantVal, LLVMTypeRef ToType);
LLVMValueRef LLVMConstFPExt(LLVMValueRef ConstantVal, LLVMTypeRef ToType);
LLVMValueRef LLVMConstUIToFP(LLVMValueRef ConstantVal, LLVMTypeRef ToType);
LLVMValueRef LLVMConstSIToFP(LLVMValueRef ConstantVal, LLVMTypeRef ToType);
LLVMValueRef LLVMConstFPToUI(LLVMValueRef ConstantVal, LLVMTypeRef ToType);
LLVMValueRef LLVMConstFPToSI(LLVMValueRef ConstantVal, LLVMTypeRef ToType);
LLVMValueRef LLVMConstPtrToInt(LLVMValueRef ConstantVal, LLVMTypeRef ToType);
LLVMValueRef LLVMConstIntToPtr(LLVMValueRef ConstantVal, LLVMTypeRef ToType);
LLVMValueRef LLVMConstBitCast(LLVMValueRef ConstantVal, LLVMTypeRef ToType);
LLVMValueRef LLVMConstZExtOrBitCast(LLVMValueRef ConstantVal,
                                    LLVMTypeRef ToType);
LLVMValueRef LLVMConstSExtOrBitCast(LLVMValueRef ConstantVal,
                                    LLVMTypeRef ToType);
LLVMValueRef LLVMConstTruncOrBitCast(LLVMValueRef ConstantVal,
                                     LLVMTypeRef ToType);
LLVMValueRef LLVMConstPointerCast(LLVMValueRef ConstantVal,
                                  LLVMTypeRef ToType);
LLVMValueRef LLVMConstIntCast(LLVMValueRef ConstantVal, LLVMTypeRef ToType,
                              LLVMBool isSigned);
LLVMValueRef LLVMConstFPCast(LLVMValueRef ConstantVal, LLVMTypeRef ToType);
LLVMValueRef LLVMConstSelect(LLVMValueRef ConstantCondition,
                             LLVMValueRef ConstantIfTrue,
                             LLVMValueRef ConstantIfFalse);
LLVMValueRef LLVMConstExtractElement(LLVMValueRef VectorConstant,
                                     LLVMValueRef IndexConstant);
LLVMValueRef LLVMConstInsertElement(LLVMValueRef VectorConstant,
                                    LLVMValueRef ElementValueConstant,
                                    LLVMValueRef IndexConstant);
LLVMValueRef LLVMConstShuffleVector(LLVMValueRef VectorAConstant,
                                    LLVMValueRef VectorBConstant,
                                    LLVMValueRef MaskConstant);
LLVMValueRef LLVMConstExtractValue(LLVMValueRef AggConstant, unsigned *IdxList,
                                   unsigned NumIdx);
LLVMValueRef LLVMConstInsertValue(LLVMValueRef AggConstant,
                                  LLVMValueRef ElementValueConstant,
                                  unsigned *IdxList, unsigned NumIdx);
LLVMValueRef LLVMConstInlineAsm(LLVMTypeRef Ty,
                                const char *AsmString, const char *Constraints,
                                LLVMBool HasSideEffects, LLVMBool IsAlignStack);
LLVMValueRef LLVMBlockAddress(LLVMValueRef F, LLVMBasicBlockRef BB);
LLVMModuleRef LLVMGetGlobalParent(LLVMValueRef Global);
LLVMBool LLVMIsDeclaration(LLVMValueRef Global);
LLVMLinkage LLVMGetLinkage(LLVMValueRef Global);
void LLVMSetLinkage(LLVMValueRef Global, LLVMLinkage Linkage);
const char *LLVMGetSection(LLVMValueRef Global);
void LLVMSetSection(LLVMValueRef Global, const char *Section);
LLVMVisibility LLVMGetVisibility(LLVMValueRef Global);
void LLVMSetVisibility(LLVMValueRef Global, LLVMVisibility Viz);
unsigned LLVMGetAlignment(LLVMValueRef Global);
void LLVMSetAlignment(LLVMValueRef Global, unsigned Bytes);
LLVMValueRef LLVMAddGlobal(LLVMModuleRef M, LLVMTypeRef Ty, const char *Name);
LLVMValueRef LLVMAddGlobalInAddressSpace(LLVMModuleRef M, LLVMTypeRef Ty,
                                         const char *Name,
                                         unsigned AddressSpace);
LLVMValueRef LLVMGetNamedGlobal(LLVMModuleRef M, const char *Name);
LLVMValueRef LLVMGetFirstGlobal(LLVMModuleRef M);
LLVMValueRef LLVMGetLastGlobal(LLVMModuleRef M);
LLVMValueRef LLVMGetNextGlobal(LLVMValueRef GlobalVar);
LLVMValueRef LLVMGetPreviousGlobal(LLVMValueRef GlobalVar);
void LLVMDeleteGlobal(LLVMValueRef GlobalVar);
LLVMValueRef LLVMGetInitializer(LLVMValueRef GlobalVar);
void LLVMSetInitializer(LLVMValueRef GlobalVar, LLVMValueRef ConstantVal);
LLVMBool LLVMIsThreadLocal(LLVMValueRef GlobalVar);
void LLVMSetThreadLocal(LLVMValueRef GlobalVar, LLVMBool IsThreadLocal);
LLVMBool LLVMIsGlobalConstant(LLVMValueRef GlobalVar);
void LLVMSetGlobalConstant(LLVMValueRef GlobalVar, LLVMBool IsConstant);
LLVMValueRef LLVMAddAlias(LLVMModuleRef M, LLVMTypeRef Ty, LLVMValueRef Aliasee,
                          const char *Name);
LLVMValueRef LLVMAddFunction(LLVMModuleRef M, const char *Name,
                             LLVMTypeRef FunctionTy);
LLVMValueRef LLVMGetNamedFunction(LLVMModuleRef M, const char *Name);
LLVMValueRef LLVMGetFirstFunction(LLVMModuleRef M);
LLVMValueRef LLVMGetLastFunction(LLVMModuleRef M);
LLVMValueRef LLVMGetNextFunction(LLVMValueRef Fn);
LLVMValueRef LLVMGetPreviousFunction(LLVMValueRef Fn);
void LLVMDeleteFunction(LLVMValueRef Fn);
unsigned LLVMGetIntrinsicID(LLVMValueRef Fn);
unsigned LLVMGetFunctionCallConv(LLVMValueRef Fn);
void LLVMSetFunctionCallConv(LLVMValueRef Fn, unsigned CC);
const char *LLVMGetGC(LLVMValueRef Fn);
void LLVMSetGC(LLVMValueRef Fn, const char *Name);
void LLVMAddFunctionAttr(LLVMValueRef Fn, LLVMAttribute PA);
LLVMAttribute LLVMGetFunctionAttr(LLVMValueRef Fn);
void LLVMRemoveFunctionAttr(LLVMValueRef Fn, LLVMAttribute PA);
unsigned LLVMCountParams(LLVMValueRef Fn);
void LLVMGetParams(LLVMValueRef Fn, LLVMValueRef *Params);
LLVMValueRef LLVMGetParam(LLVMValueRef Fn, unsigned Index);
LLVMValueRef LLVMGetParamParent(LLVMValueRef Inst);
LLVMValueRef LLVMGetFirstParam(LLVMValueRef Fn);
LLVMValueRef LLVMGetLastParam(LLVMValueRef Fn);
LLVMValueRef LLVMGetNextParam(LLVMValueRef Arg);
LLVMValueRef LLVMGetPreviousParam(LLVMValueRef Arg);
void LLVMAddAttribute(LLVMValueRef Arg, LLVMAttribute PA);
void LLVMRemoveAttribute(LLVMValueRef Arg, LLVMAttribute PA);
LLVMAttribute LLVMGetAttribute(LLVMValueRef Arg);
void LLVMSetParamAlignment(LLVMValueRef Arg, unsigned align);
LLVMValueRef LLVMBasicBlockAsValue(LLVMBasicBlockRef BB);
LLVMBool LLVMValueIsBasicBlock(LLVMValueRef Val);
LLVMBasicBlockRef LLVMValueAsBasicBlock(LLVMValueRef Val);
LLVMValueRef LLVMGetBasicBlockParent(LLVMBasicBlockRef BB);
unsigned LLVMCountBasicBlocks(LLVMValueRef Fn);
void LLVMGetBasicBlocks(LLVMValueRef Fn, LLVMBasicBlockRef *BasicBlocks);
LLVMBasicBlockRef LLVMGetFirstBasicBlock(LLVMValueRef Fn);
LLVMBasicBlockRef LLVMGetLastBasicBlock(LLVMValueRef Fn);
LLVMBasicBlockRef LLVMGetNextBasicBlock(LLVMBasicBlockRef BB);
LLVMBasicBlockRef LLVMGetPreviousBasicBlock(LLVMBasicBlockRef BB);
LLVMBasicBlockRef LLVMGetEntryBasicBlock(LLVMValueRef Fn);

LLVMBasicBlockRef LLVMAppendBasicBlockInContext(LLVMContextRef C,
                                                LLVMValueRef Fn,
                                                const char *Name);
LLVMBasicBlockRef LLVMInsertBasicBlockInContext(LLVMContextRef C,
                                                LLVMBasicBlockRef BB,
                                                const char *Name);

LLVMBasicBlockRef LLVMAppendBasicBlock(LLVMValueRef Fn, const char *Name);
LLVMBasicBlockRef LLVMInsertBasicBlock(LLVMBasicBlockRef InsertBeforeBB,
                                       const char *Name);
void LLVMDeleteBasicBlock(LLVMBasicBlockRef BB);

void LLVMMoveBasicBlockBefore(LLVMBasicBlockRef BB, LLVMBasicBlockRef MovePos);
void LLVMMoveBasicBlockAfter(LLVMBasicBlockRef BB, LLVMBasicBlockRef MovePos);
LLVMBasicBlockRef LLVMGetInstructionParent(LLVMValueRef Inst);
LLVMValueRef LLVMGetFirstInstruction(LLVMBasicBlockRef BB);
LLVMValueRef LLVMGetLastInstruction(LLVMBasicBlockRef BB);
LLVMValueRef LLVMGetNextInstruction(LLVMValueRef Inst);
LLVMValueRef LLVMGetPreviousInstruction(LLVMValueRef Inst);
void LLVMSetInstructionCallConv(LLVMValueRef Instr, unsigned CC);
unsigned LLVMGetInstructionCallConv(LLVMValueRef Instr);
void LLVMAddInstrAttribute(LLVMValueRef Instr, unsigned index, LLVMAttribute);
void LLVMRemoveInstrAttribute(LLVMValueRef Instr, unsigned index,
                              LLVMAttribute);
void LLVMSetInstrParamAlignment(LLVMValueRef Instr, unsigned index,
                                unsigned align);
LLVMBool LLVMIsTailCall(LLVMValueRef CallInst);
void LLVMSetTailCall(LLVMValueRef CallInst, LLVMBool IsTailCall);
void LLVMAddIncoming(LLVMValueRef PhiNode, LLVMValueRef *IncomingValues,
                     LLVMBasicBlockRef *IncomingBlocks, unsigned Count);
unsigned LLVMCountIncoming(LLVMValueRef PhiNode);
LLVMValueRef LLVMGetIncomingValue(LLVMValueRef PhiNode, unsigned Index);
LLVMBasicBlockRef LLVMGetIncomingBlock(LLVMValueRef PhiNode, unsigned Index);
LLVMBuilderRef LLVMCreateBuilderInContext(LLVMContextRef C);
LLVMBuilderRef LLVMCreateBuilder(void);
void LLVMPositionBuilder(LLVMBuilderRef Builder, LLVMBasicBlockRef Block,
                         LLVMValueRef Instr);
void LLVMPositionBuilderBefore(LLVMBuilderRef Builder, LLVMValueRef Instr);
void LLVMPositionBuilderAtEnd(LLVMBuilderRef Builder, LLVMBasicBlockRef Block);
LLVMBasicBlockRef LLVMGetInsertBlock(LLVMBuilderRef Builder);
void LLVMClearInsertionPosition(LLVMBuilderRef Builder);
void LLVMInsertIntoBuilder(LLVMBuilderRef Builder, LLVMValueRef Instr);
void LLVMInsertIntoBuilderWithName(LLVMBuilderRef Builder, LLVMValueRef Instr,
                                   const char *Name);
void LLVMDisposeBuilder(LLVMBuilderRef Builder);
void LLVMSetCurrentDebugLocation(LLVMBuilderRef Builder, LLVMValueRef L);
LLVMValueRef LLVMGetCurrentDebugLocation(LLVMBuilderRef Builder);
void LLVMSetInstDebugLocation(LLVMBuilderRef Builder, LLVMValueRef Inst);
LLVMValueRef LLVMBuildRetVoid(LLVMBuilderRef);
LLVMValueRef LLVMBuildRet(LLVMBuilderRef, LLVMValueRef V);
LLVMValueRef LLVMBuildAggregateRet(LLVMBuilderRef, LLVMValueRef *RetVals,
                                   unsigned N);
LLVMValueRef LLVMBuildBr(LLVMBuilderRef, LLVMBasicBlockRef Dest);
LLVMValueRef LLVMBuildCondBr(LLVMBuilderRef, LLVMValueRef If,
                             LLVMBasicBlockRef Then, LLVMBasicBlockRef Else);
LLVMValueRef LLVMBuildSwitch(LLVMBuilderRef, LLVMValueRef V,
                             LLVMBasicBlockRef Else, unsigned NumCases);
LLVMValueRef LLVMBuildIndirectBr(LLVMBuilderRef B, LLVMValueRef Addr,
                                 unsigned NumDests);
LLVMValueRef LLVMBuildInvoke(LLVMBuilderRef, LLVMValueRef Fn,
                             LLVMValueRef *Args, unsigned NumArgs,
                             LLVMBasicBlockRef Then, LLVMBasicBlockRef Catch,
                             const char *Name);
LLVMValueRef LLVMBuildUnreachable(LLVMBuilderRef);
void LLVMAddCase(LLVMValueRef Switch, LLVMValueRef OnVal,
                 LLVMBasicBlockRef Dest);
void LLVMAddDestination(LLVMValueRef IndirectBr, LLVMBasicBlockRef Dest);
LLVMValueRef LLVMBuildAdd(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                          const char *Name);
LLVMValueRef LLVMBuildNSWAdd(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                             const char *Name);
LLVMValueRef LLVMBuildNUWAdd(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                             const char *Name);
LLVMValueRef LLVMBuildFAdd(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                           const char *Name);
LLVMValueRef LLVMBuildSub(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                          const char *Name);
LLVMValueRef LLVMBuildNSWSub(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                             const char *Name);
LLVMValueRef LLVMBuildNUWSub(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                             const char *Name);
LLVMValueRef LLVMBuildFSub(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                           const char *Name);
LLVMValueRef LLVMBuildMul(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                          const char *Name);
LLVMValueRef LLVMBuildNSWMul(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                             const char *Name);
LLVMValueRef LLVMBuildNUWMul(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                             const char *Name);
LLVMValueRef LLVMBuildFMul(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                           const char *Name);
LLVMValueRef LLVMBuildUDiv(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                           const char *Name);
LLVMValueRef LLVMBuildSDiv(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                           const char *Name);
LLVMValueRef LLVMBuildExactSDiv(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                                const char *Name);
LLVMValueRef LLVMBuildFDiv(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                           const char *Name);
LLVMValueRef LLVMBuildURem(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                           const char *Name);
LLVMValueRef LLVMBuildSRem(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                           const char *Name);
LLVMValueRef LLVMBuildFRem(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                           const char *Name);
LLVMValueRef LLVMBuildShl(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                           const char *Name);
LLVMValueRef LLVMBuildLShr(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                           const char *Name);
LLVMValueRef LLVMBuildAShr(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                           const char *Name);
LLVMValueRef LLVMBuildAnd(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                          const char *Name);
LLVMValueRef LLVMBuildOr(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                          const char *Name);
LLVMValueRef LLVMBuildXor(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                          const char *Name);
LLVMValueRef LLVMBuildBinOp(LLVMBuilderRef B, LLVMOpcode Op,
                            LLVMValueRef LHS, LLVMValueRef RHS,
                            const char *Name);
LLVMValueRef LLVMBuildNeg(LLVMBuilderRef, LLVMValueRef V, const char *Name);
LLVMValueRef LLVMBuildNSWNeg(LLVMBuilderRef B, LLVMValueRef V,
                             const char *Name);
LLVMValueRef LLVMBuildNUWNeg(LLVMBuilderRef B, LLVMValueRef V,
                             const char *Name);
LLVMValueRef LLVMBuildFNeg(LLVMBuilderRef, LLVMValueRef V, const char *Name);
LLVMValueRef LLVMBuildNot(LLVMBuilderRef, LLVMValueRef V, const char *Name);
LLVMValueRef LLVMBuildMalloc(LLVMBuilderRef, LLVMTypeRef Ty, const char *Name);
LLVMValueRef LLVMBuildArrayMalloc(LLVMBuilderRef, LLVMTypeRef Ty,
                                  LLVMValueRef Val, const char *Name);
LLVMValueRef LLVMBuildAlloca(LLVMBuilderRef, LLVMTypeRef Ty, const char *Name);
LLVMValueRef LLVMBuildArrayAlloca(LLVMBuilderRef, LLVMTypeRef Ty,
                                  LLVMValueRef Val, const char *Name);
LLVMValueRef LLVMBuildFree(LLVMBuilderRef, LLVMValueRef PointerVal);
LLVMValueRef LLVMBuildLoad(LLVMBuilderRef, LLVMValueRef PointerVal,
                           const char *Name);
LLVMValueRef LLVMBuildStore(LLVMBuilderRef, LLVMValueRef Val, LLVMValueRef Ptr);
LLVMValueRef LLVMBuildGEP(LLVMBuilderRef B, LLVMValueRef Pointer,
                          LLVMValueRef *Indices, unsigned NumIndices,
                          const char *Name);
LLVMValueRef LLVMBuildInBoundsGEP(LLVMBuilderRef B, LLVMValueRef Pointer,
                                  LLVMValueRef *Indices, unsigned NumIndices,
                                  const char *Name);
LLVMValueRef LLVMBuildStructGEP(LLVMBuilderRef B, LLVMValueRef Pointer,
                                unsigned Idx, const char *Name);
LLVMValueRef LLVMBuildGlobalString(LLVMBuilderRef B, const char *Str,
                                   const char *Name);
LLVMValueRef LLVMBuildGlobalStringPtr(LLVMBuilderRef B, const char *Str,
                                      const char *Name);
LLVMValueRef LLVMBuildTrunc(LLVMBuilderRef, LLVMValueRef Val,
                            LLVMTypeRef DestTy, const char *Name);
LLVMValueRef LLVMBuildZExt(LLVMBuilderRef, LLVMValueRef Val,
                           LLVMTypeRef DestTy, const char *Name);
LLVMValueRef LLVMBuildSExt(LLVMBuilderRef, LLVMValueRef Val,
                           LLVMTypeRef DestTy, const char *Name);
LLVMValueRef LLVMBuildFPToUI(LLVMBuilderRef, LLVMValueRef Val,
                             LLVMTypeRef DestTy, const char *Name);
LLVMValueRef LLVMBuildFPToSI(LLVMBuilderRef, LLVMValueRef Val,
                             LLVMTypeRef DestTy, const char *Name);
LLVMValueRef LLVMBuildUIToFP(LLVMBuilderRef, LLVMValueRef Val,
                             LLVMTypeRef DestTy, const char *Name);
LLVMValueRef LLVMBuildSIToFP(LLVMBuilderRef, LLVMValueRef Val,
                             LLVMTypeRef DestTy, const char *Name);
LLVMValueRef LLVMBuildFPTrunc(LLVMBuilderRef, LLVMValueRef Val,
                              LLVMTypeRef DestTy, const char *Name);
LLVMValueRef LLVMBuildFPExt(LLVMBuilderRef, LLVMValueRef Val,
                            LLVMTypeRef DestTy, const char *Name);
LLVMValueRef LLVMBuildPtrToInt(LLVMBuilderRef, LLVMValueRef Val,
                               LLVMTypeRef DestTy, const char *Name);
LLVMValueRef LLVMBuildIntToPtr(LLVMBuilderRef, LLVMValueRef Val,
                               LLVMTypeRef DestTy, const char *Name);
LLVMValueRef LLVMBuildBitCast(LLVMBuilderRef, LLVMValueRef Val,
                              LLVMTypeRef DestTy, const char *Name);
LLVMValueRef LLVMBuildZExtOrBitCast(LLVMBuilderRef, LLVMValueRef Val,
                                    LLVMTypeRef DestTy, const char *Name);
LLVMValueRef LLVMBuildSExtOrBitCast(LLVMBuilderRef, LLVMValueRef Val,
                                    LLVMTypeRef DestTy, const char *Name);
LLVMValueRef LLVMBuildTruncOrBitCast(LLVMBuilderRef, LLVMValueRef Val,
                                     LLVMTypeRef DestTy, const char *Name);
LLVMValueRef LLVMBuildCast(LLVMBuilderRef B, LLVMOpcode Op, LLVMValueRef Val,
                           LLVMTypeRef DestTy, const char *Name);
LLVMValueRef LLVMBuildPointerCast(LLVMBuilderRef, LLVMValueRef Val,
                                  LLVMTypeRef DestTy, const char *Name);
LLVMValueRef LLVMBuildIntCast(LLVMBuilderRef, LLVMValueRef Val,
                              LLVMTypeRef DestTy, const char *Name);
LLVMValueRef LLVMBuildFPCast(LLVMBuilderRef, LLVMValueRef Val,
                             LLVMTypeRef DestTy, const char *Name);
LLVMValueRef LLVMBuildICmp(LLVMBuilderRef, LLVMIntPredicate Op,
                           LLVMValueRef LHS, LLVMValueRef RHS,
                           const char *Name);
LLVMValueRef LLVMBuildFCmp(LLVMBuilderRef, LLVMRealPredicate Op,
                           LLVMValueRef LHS, LLVMValueRef RHS,
                           const char *Name);
LLVMValueRef LLVMBuildPhi(LLVMBuilderRef, LLVMTypeRef Ty, const char *Name);
LLVMValueRef LLVMBuildCall(LLVMBuilderRef, LLVMValueRef Fn,
                           LLVMValueRef *Args, unsigned NumArgs,
                           const char *Name);
LLVMValueRef LLVMBuildSelect(LLVMBuilderRef, LLVMValueRef If,
                             LLVMValueRef Then, LLVMValueRef Else,
                             const char *Name);
LLVMValueRef LLVMBuildVAArg(LLVMBuilderRef, LLVMValueRef List, LLVMTypeRef Ty,
                            const char *Name);
LLVMValueRef LLVMBuildExtractElement(LLVMBuilderRef, LLVMValueRef VecVal,
                                     LLVMValueRef Index, const char *Name);
LLVMValueRef LLVMBuildInsertElement(LLVMBuilderRef, LLVMValueRef VecVal,
                                    LLVMValueRef EltVal, LLVMValueRef Index,
                                    const char *Name);
LLVMValueRef LLVMBuildShuffleVector(LLVMBuilderRef, LLVMValueRef V1,
                                    LLVMValueRef V2, LLVMValueRef Mask,
                                    const char *Name);
LLVMValueRef LLVMBuildExtractValue(LLVMBuilderRef, LLVMValueRef AggVal,
                                   unsigned Index, const char *Name);
LLVMValueRef LLVMBuildInsertValue(LLVMBuilderRef, LLVMValueRef AggVal,
                                  LLVMValueRef EltVal, unsigned Index,
                                  const char *Name);

LLVMValueRef LLVMBuildIsNull(LLVMBuilderRef, LLVMValueRef Val,
                             const char *Name);
LLVMValueRef LLVMBuildIsNotNull(LLVMBuilderRef, LLVMValueRef Val,
                                const char *Name);
LLVMValueRef LLVMBuildPtrDiff(LLVMBuilderRef, LLVMValueRef LHS,
                              LLVMValueRef RHS, const char *Name);
LLVMModuleProviderRef
LLVMCreateModuleProviderForExistingModule(LLVMModuleRef M);
void LLVMDisposeModuleProvider(LLVMModuleProviderRef M);
LLVMBool LLVMCreateMemoryBufferWithContentsOfFile(const char *Path,
                                                  LLVMMemoryBufferRef *OutMemBuf,
                                                  char **OutMessage);
LLVMBool LLVMCreateMemoryBufferWithSTDIN(LLVMMemoryBufferRef *OutMemBuf,
                                         char **OutMessage);
void LLVMDisposeMemoryBuffer(LLVMMemoryBufferRef MemBuf);

LLVMPassRegistryRef LLVMGetGlobalPassRegistry(void);
LLVMPassManagerRef LLVMCreatePassManager(void);

LLVMPassManagerRef LLVMCreateFunctionPassManagerForModule(LLVMModuleRef M);
LLVMPassManagerRef LLVMCreateFunctionPassManager(LLVMModuleProviderRef MP);
LLVMBool LLVMRunPassManager(LLVMPassManagerRef PM, LLVMModuleRef M);
LLVMBool LLVMInitializeFunctionPassManager(LLVMPassManagerRef FPM);

LLVMBool LLVMRunFunctionPassManager(LLVMPassManagerRef FPM, LLVMValueRef F);
LLVMBool LLVMFinalizeFunctionPassManager(LLVMPassManagerRef FPM);
void LLVMDisposePassManager(LLVMPassManagerRef PM);
typedef enum {
  LLVMAbortProcessAction,
  LLVMPrintMessageAction,
  LLVMReturnStatusAction
} LLVMVerifierFailureAction;

LLVMBool LLVMVerifyModule(LLVMModuleRef M, LLVMVerifierFailureAction Action,
                          char **OutMessage);
LLVMBool LLVMVerifyFunction(LLVMValueRef Fn, LLVMVerifierFailureAction Action);
void LLVMViewFunctionCFG(LLVMValueRef Fn);
void LLVMViewFunctionCFGOnly(LLVMValueRef Fn);
LLVMBool LLVMParseBitcode(LLVMMemoryBufferRef MemBuf,
                          LLVMModuleRef *OutModule, char **OutMessage);

LLVMBool LLVMParseBitcodeInContext(LLVMContextRef ContextRef,
                                   LLVMMemoryBufferRef MemBuf,
                                   LLVMModuleRef *OutModule, char **OutMessage);
LLVMBool LLVMGetBitcodeModuleInContext(LLVMContextRef ContextRef,
                                       LLVMMemoryBufferRef MemBuf,
                                       LLVMModuleRef *OutM,
                                       char **OutMessage);

LLVMBool LLVMGetBitcodeModule(LLVMMemoryBufferRef MemBuf, LLVMModuleRef *OutM,
                              char **OutMessage);
LLVMBool LLVMGetBitcodeModuleProviderInContext(LLVMContextRef ContextRef,
                                               LLVMMemoryBufferRef MemBuf,
                                               LLVMModuleProviderRef *OutMP,
                                               char **OutMessage);
LLVMBool LLVMGetBitcodeModuleProvider(LLVMMemoryBufferRef MemBuf,
                                      LLVMModuleProviderRef *OutMP,
                                      char **OutMessage);
int LLVMWriteBitcodeToFile(LLVMModuleRef M, const char *Path);
int LLVMWriteBitcodeToFD(LLVMModuleRef M, int FD, int ShouldClose,
                         int Unbuffered);
int LLVMWriteBitcodeToFileHandle(LLVMModuleRef M, int Handle);


void LLVMInitializeX86TargetInfo(void);
void LLVMInitializeX86Target(void);
void LLVMInitializeX86TargetMC(void);

LLVMTargetDataRef LLVMCreateTargetData(const char *StringRep);
void LLVMAddTargetData(LLVMTargetDataRef, LLVMPassManagerRef);
char *LLVMCopyStringRepOfTargetData(LLVMTargetDataRef);
enum LLVMByteOrdering LLVMByteOrder(LLVMTargetDataRef);
unsigned LLVMPointerSize(LLVMTargetDataRef);
LLVMTypeRef LLVMIntPtrType(LLVMTargetDataRef);
unsigned long long LLVMSizeOfTypeInBits(LLVMTargetDataRef, LLVMTypeRef);
unsigned long long LLVMStoreSizeOfType(LLVMTargetDataRef, LLVMTypeRef);
unsigned long long LLVMABISizeOfType(LLVMTargetDataRef, LLVMTypeRef);
unsigned LLVMABIAlignmentOfType(LLVMTargetDataRef, LLVMTypeRef);
unsigned LLVMCallFrameAlignmentOfType(LLVMTargetDataRef, LLVMTypeRef);
unsigned LLVMPreferredAlignmentOfType(LLVMTargetDataRef, LLVMTypeRef);
unsigned LLVMPreferredAlignmentOfGlobal(LLVMTargetDataRef,
                                        LLVMValueRef GlobalVar);
unsigned LLVMElementAtOffset(LLVMTargetDataRef, LLVMTypeRef StructTy,
                             unsigned long long Offset);
unsigned long long LLVMOffsetOfElement(LLVMTargetDataRef, LLVMTypeRef StructTy,
                                       unsigned Element);

void LLVMDisposeTargetData(LLVMTargetDataRef);

void LLVMLinkInJIT(void);
void LLVMLinkInInterpreter(void);

LLVMGenericValueRef LLVMCreateGenericValueOfInt(LLVMTypeRef Ty,
                                                unsigned long long N,
                                                LLVMBool IsSigned);

LLVMGenericValueRef LLVMCreateGenericValueOfPointer(void *P);

LLVMGenericValueRef LLVMCreateGenericValueOfFloat(LLVMTypeRef Ty, double N);

unsigned LLVMGenericValueIntWidth(LLVMGenericValueRef GenValRef);

unsigned long long LLVMGenericValueToInt(LLVMGenericValueRef GenVal,
                                         LLVMBool IsSigned);

void *LLVMGenericValueToPointer(LLVMGenericValueRef GenVal);

double LLVMGenericValueToFloat(LLVMTypeRef TyRef, LLVMGenericValueRef GenVal);

void LLVMDisposeGenericValue(LLVMGenericValueRef GenVal);
LLVMBool LLVMCreateExecutionEngineForModule(LLVMExecutionEngineRef *OutEE,
                                            LLVMModuleRef M,
                                            char **OutError);

LLVMBool LLVMCreateInterpreterForModule(LLVMExecutionEngineRef *OutInterp,
                                        LLVMModuleRef M,
                                        char **OutError);

LLVMBool LLVMCreateJITCompilerForModule(LLVMExecutionEngineRef *OutJIT,
                                        LLVMModuleRef M,
                                        unsigned OptLevel,
                                        char **OutError);
                                        
LLVMBool LLVMCreateExecutionEngine(LLVMExecutionEngineRef *OutEE,
                                   LLVMModuleProviderRef MP,
                                   char **OutError);
LLVMBool LLVMCreateInterpreter(LLVMExecutionEngineRef *OutInterp,
                               LLVMModuleProviderRef MP,
                               char **OutError);
LLVMBool LLVMCreateJITCompiler(LLVMExecutionEngineRef *OutJIT,
                               LLVMModuleProviderRef MP,
                               unsigned OptLevel,
                               char **OutError);

void LLVMDisposeExecutionEngine(LLVMExecutionEngineRef EE);

void LLVMRunStaticConstructors(LLVMExecutionEngineRef EE);

void LLVMRunStaticDestructors(LLVMExecutionEngineRef EE);

int LLVMRunFunctionAsMain(LLVMExecutionEngineRef EE, LLVMValueRef F,
                          unsigned ArgC, const char * const *ArgV,
                          const char * const *EnvP);

LLVMGenericValueRef LLVMRunFunction(LLVMExecutionEngineRef EE, LLVMValueRef F,
                                    unsigned NumArgs,
                                    LLVMGenericValueRef *Args);

void LLVMFreeMachineCodeForFunction(LLVMExecutionEngineRef EE, LLVMValueRef F);

void LLVMAddModule(LLVMExecutionEngineRef EE, LLVMModuleRef M);
void LLVMAddModuleProvider(LLVMExecutionEngineRef EE, LLVMModuleProviderRef MP);

LLVMBool LLVMRemoveModule(LLVMExecutionEngineRef EE, LLVMModuleRef M,
                          LLVMModuleRef *OutMod, char **OutError);
LLVMBool LLVMRemoveModuleProvider(LLVMExecutionEngineRef EE,
                                  LLVMModuleProviderRef MP,
                                  LLVMModuleRef *OutMod, char **OutError);

LLVMBool LLVMFindFunction(LLVMExecutionEngineRef EE, const char *Name,
                          LLVMValueRef *OutFn);

void *LLVMRecompileAndRelinkFunction(LLVMExecutionEngineRef EE, LLVMValueRef Fn);

LLVMTargetDataRef LLVMGetExecutionEngineTargetData(LLVMExecutionEngineRef EE);

void LLVMAddGlobalMapping(LLVMExecutionEngineRef EE, LLVMValueRef Global,
                          void* Addr);

void *LLVMGetPointerToGlobal(LLVMExecutionEngineRef EE, LLVMValueRef Global);
void LLVMInitializeCore(LLVMPassRegistryRef R);
void LLVMInitializeTransformUtils(LLVMPassRegistryRef R);
void LLVMInitializeScalarOpts(LLVMPassRegistryRef R);
void LLVMInitializeInstCombine(LLVMPassRegistryRef R);
void LLVMInitializeIPO(LLVMPassRegistryRef R);

void LLVMInitializeAnalysis(LLVMPassRegistryRef R);
void LLVMInitializeIPA(LLVMPassRegistryRef R);
void LLVMInitializeCodeGen(LLVMPassRegistryRef R);
void LLVMInitializeTarget(LLVMPassRegistryRef R);
void LLVMAddArgumentPromotionPass(LLVMPassManagerRef PM);
void LLVMAddConstantMergePass(LLVMPassManagerRef PM);
void LLVMAddDeadArgEliminationPass(LLVMPassManagerRef PM);

void LLVMAddFunctionAttrsPass(LLVMPassManagerRef PM);
void LLVMAddFunctionInliningPass(LLVMPassManagerRef PM);
void LLVMAddGlobalDCEPass(LLVMPassManagerRef PM);
void LLVMAddGlobalOptimizerPass(LLVMPassManagerRef PM);
void LLVMAddIPConstantPropagationPass(LLVMPassManagerRef PM);

void LLVMAddPruneEHPass(LLVMPassManagerRef PM);
void LLVMAddIPSCCPPass(LLVMPassManagerRef PM);
void LLVMAddInternalizePass(LLVMPassManagerRef, unsigned AllButMain);

void LLVMAddStripDeadPrototypesPass(LLVMPassManagerRef PM);
void LLVMAddStripSymbolsPass(LLVMPassManagerRef PM);

void LLVMAddAggressiveDCEPass(LLVMPassManagerRef PM);
void LLVMAddCFGSimplificationPass(LLVMPassManagerRef PM);
void LLVMAddDeadStoreEliminationPass(LLVMPassManagerRef PM);
void LLVMAddGVNPass(LLVMPassManagerRef PM);
void LLVMAddIndVarSimplifyPass(LLVMPassManagerRef PM);
void LLVMAddInstructionCombiningPass(LLVMPassManagerRef PM);
void LLVMAddJumpThreadingPass(LLVMPassManagerRef PM);
void LLVMAddLICMPass(LLVMPassManagerRef PM);
void LLVMAddLoopDeletionPass(LLVMPassManagerRef PM);
void LLVMAddLoopRotatePass(LLVMPassManagerRef PM);
void LLVMAddLoopUnrollPass(LLVMPassManagerRef PM);
void LLVMAddLoopUnswitchPass(LLVMPassManagerRef PM);
void LLVMAddMemCpyOptPass(LLVMPassManagerRef PM);
void LLVMAddPromoteMemoryToRegisterPass(LLVMPassManagerRef PM);
void LLVMAddReassociatePass(LLVMPassManagerRef PM);
void LLVMAddSCCPPass(LLVMPassManagerRef PM);
void LLVMAddScalarReplAggregatesPass(LLVMPassManagerRef PM);
void LLVMAddScalarReplAggregatesPassWithThreshold(LLVMPassManagerRef PM,
                                                  int Threshold);
void LLVMAddSimplifyLibCallsPass(LLVMPassManagerRef PM);
void LLVMAddTailCallEliminationPass(LLVMPassManagerRef PM);
void LLVMAddConstantPropagationPass(LLVMPassManagerRef PM);
void LLVMAddDemoteMemoryToRegisterPass(LLVMPassManagerRef PM);
void LLVMAddVerifierPass(LLVMPassManagerRef PM);

LLVMPassManagerBuilderRef LLVMPassManagerBuilderCreate(void);
void LLVMPassManagerBuilderDispose(LLVMPassManagerBuilderRef PMB);

void
LLVMPassManagerBuilderSetOptLevel(LLVMPassManagerBuilderRef PMB,
                                  unsigned OptLevel);
void
LLVMPassManagerBuilderSetSizeLevel(LLVMPassManagerBuilderRef PMB,
                                   unsigned SizeLevel);
void
LLVMPassManagerBuilderSetDisableUnitAtATime(LLVMPassManagerBuilderRef PMB,
                                            LLVMBool Value);
void
LLVMPassManagerBuilderSetDisableUnrollLoops(LLVMPassManagerBuilderRef PMB,
                                            LLVMBool Value);
void
LLVMPassManagerBuilderSetDisableSimplifyLibCalls(LLVMPassManagerBuilderRef PMB,
                                                 LLVMBool Value);
void
LLVMPassManagerBuilderUseInlinerWithThreshold(LLVMPassManagerBuilderRef PMB,
                                              unsigned Threshold);
void
LLVMPassManagerBuilderPopulateFunctionPassManager(LLVMPassManagerBuilderRef PMB,
                                                  LLVMPassManagerRef PM);
void
LLVMPassManagerBuilderPopulateModulePassManager(LLVMPassManagerBuilderRef PMB,
                                                LLVMPassManagerRef PM);
void LLVMPassManagerBuilderPopulateLTOPassManager(LLVMPassManagerBuilderRef PMB,
                                                  LLVMPassManagerRef PM,
                                                  bool Internalize,
                                                  bool RunInliner);

]]
ffi.cdef(header)

if false then
	-- generate metatype bindings by parsing the header for known types as first arguments:
	local objs = { 
		"Context", 
		"Module", 
		"Type", "Value", "BasicBlock", "Builder", 
		"ModuleProvider", "MemoryBuffer", "PassManager", "PassRegistry", "Use",
		"TargetData", "GenericValue", "ExecutionEngine",
	}
	for i, obj in ipairs(objs) do
		print(string.format("\nlocal %s = {", obj))
		local patt = "LLVM(%w+)%(LLVM" .. obj .. "Ref"
		for m in header:gmatch(patt) do
			print(string.format("\t%s = lib.LLVM%s,", m, m))
		end
		print("}")
		print(string.format("%s.__index = %s", obj, obj))
	end
	
	print("-- no modifications to metatables after this point:")
	for i, obj in ipairs(objs) do
		print(string.format('ffi.metatype("struct LLVMOpaque%s", %s)\n', obj, obj))
	end
	os.exit()
end

local llvm = {}

---- METATYPES ----

local Context = {
	ContextDispose = lib.LLVMContextDispose,
	GetMDKindIDInContext = lib.LLVMGetMDKindIDInContext,
	Int1TypeInContext = lib.LLVMInt1TypeInContext,
	Int8TypeInContext = lib.LLVMInt8TypeInContext,
	Int16TypeInContext = lib.LLVMInt16TypeInContext,
	Int32TypeInContext = lib.LLVMInt32TypeInContext,
	Int64TypeInContext = lib.LLVMInt64TypeInContext,
	IntTypeInContext = lib.LLVMIntTypeInContext,
	FloatTypeInContext = lib.LLVMFloatTypeInContext,
	DoubleTypeInContext = lib.LLVMDoubleTypeInContext,
	X86FP80TypeInContext = lib.LLVMX86FP80TypeInContext,
	FP128TypeInContext = lib.LLVMFP128TypeInContext,
	PPCFP128TypeInContext = lib.LLVMPPCFP128TypeInContext,
	StructTypeInContext = lib.LLVMStructTypeInContext,
	StructCreateNamed = lib.LLVMStructCreateNamed,	
	VoidTypeInContext = lib.LLVMVoidTypeInContext,
	LabelTypeInContext = lib.LLVMLabelTypeInContext,
	X86MMXTypeInContext = lib.LLVMX86MMXTypeInContext,
	MDStringInContext = lib.LLVMMDStringInContext,
	MDNodeInContext = lib.LLVMMDNodeInContext,
	ConstStringInContext = lib.LLVMConstStringInContext,
	ConstStructInContext = lib.LLVMConstStructInContext,
	AppendBasicBlockInContext = lib.LLVMAppendBasicBlockInContext,
	InsertBasicBlockInContext = lib.LLVMInsertBasicBlockInContext,
	CreateBuilderInContext = lib.LLVMCreateBuilderInContext,
	ParseBitcodeInContext = lib.LLVMParseBitcodeInContext,
	GetBitcodeModuleInContext = lib.LLVMGetBitcodeModuleInContext,
	GetBitcodeModuleProviderInContext = lib.LLVMGetBitcodeModuleProviderInContext,
}
Context.__index = Context


local Module = {
	DisposeModule = lib.LLVMDisposeModule,
	GetDataLayout = lib.LLVMGetDataLayout,
	SetDataLayout = lib.LLVMSetDataLayout,
	GetTarget = lib.LLVMGetTarget,
	SetTarget = lib.LLVMSetTarget,
	GetTypeByName = lib.LLVMGetTypeByName,
	DumpModule = lib.LLVMDumpModule,
	SetModuleInlineAsm = lib.LLVMSetModuleInlineAsm,
	GetModuleContext = lib.LLVMGetModuleContext,
	AddGlobal = lib.LLVMAddGlobal,
	AddGlobalInAddressSpace = lib.LLVMAddGlobalInAddressSpace,
	GetNamedGlobal = lib.LLVMGetNamedGlobal,
	GetFirstGlobal = lib.LLVMGetFirstGlobal,
	GetLastGlobal = lib.LLVMGetLastGlobal,
	AddAlias = lib.LLVMAddAlias,
	AddFunction = lib.LLVMAddFunction,
	GetNamedFunction = lib.LLVMGetNamedFunction,
	GetFirstFunction = lib.LLVMGetFirstFunction,
	GetLastFunction = lib.LLVMGetLastFunction,
	CreateModuleProviderForExistingModule = lib.LLVMCreateModuleProviderForExistingModule,
	CreateFunctionPassManagerForModule = lib.LLVMCreateFunctionPassManagerForModule,
	VerifyModule = lib.LLVMVerifyModule,
	WriteBitcodeToFile = lib.LLVMWriteBitcodeToFile,
	WriteBitcodeToFD = lib.LLVMWriteBitcodeToFD,
	WriteBitcodeToFileHandle = lib.LLVMWriteBitcodeToFileHandle,
}
Module.__index = Module

local Type = {
	GetTypeKind = lib.LLVMGetTypeKind,
	TypeIsSized = lib.LLVMTypeIsSized,
	GetTypeContext = lib.LLVMGetTypeContext,
	GetIntTypeWidth = lib.LLVMGetIntTypeWidth,
	FunctionType = lib.LLVMFunctionType,
	IsFunctionVarArg = lib.LLVMIsFunctionVarArg,
	GetReturnType = lib.LLVMGetReturnType,
	CountParamTypes = lib.LLVMCountParamTypes,
	GetParamTypes = lib.LLVMGetParamTypes,
	StructType = lib.LLVMStructType,
	GetStructName = lib.LLVMGetStructName,
	StructSetBody = lib.LLVMStructSetBody,
	CountStructElementTypes = lib.LLVMCountStructElementTypes,
	GetStructElementTypes = lib.LLVMGetStructElementTypes,
	IsPackedStruct = lib.LLVMIsPackedStruct,
	ArrayType = lib.LLVMArrayType,
	PointerType = lib.LLVMPointerType,
	VectorType = lib.LLVMVectorType,
	GetElementType = lib.LLVMGetElementType,
	GetArrayLength = lib.LLVMGetArrayLength,
	GetPointerAddressSpace = lib.LLVMGetPointerAddressSpace,
	GetVectorSize = lib.LLVMGetVectorSize,
	ConstNull = lib.LLVMConstNull,
	ConstAllOnes = lib.LLVMConstAllOnes,
	GetUndef = lib.LLVMGetUndef,
	ConstPointerNull = lib.LLVMConstPointerNull,
	ConstInt = lib.LLVMConstInt,
	ConstIntOfArbitraryPrecision = lib.LLVMConstIntOfArbitraryPrecision,
	ConstIntOfString = lib.LLVMConstIntOfString,
	ConstIntOfStringAndSize = lib.LLVMConstIntOfStringAndSize,
	ConstReal = lib.LLVMConstReal,
	ConstRealOfString = lib.LLVMConstRealOfString,
	ConstRealOfStringAndSize = lib.LLVMConstRealOfStringAndSize,
	ConstArray = lib.LLVMConstArray,
	AlignOf = lib.LLVMAlignOf,
	SizeOf = lib.LLVMSizeOf,
	ConstInlineAsm = lib.LLVMConstInlineAsm,
	CreateGenericValueOfInt = lib.LLVMCreateGenericValueOfInt,
	CreateGenericValueOfFloat = lib.LLVMCreateGenericValueOfFloat,
	GenericValueToFloat = lib.LLVMGenericValueToFloat,
}
Type.__index = Type

local Value = {
	TypeOf = lib.LLVMTypeOf,
	GetValueName = lib.LLVMGetValueName,
	SetValueName = lib.LLVMSetValueName,
	DumpValue = lib.LLVMDumpValue,
	ReplaceAllUsesWith = lib.LLVMReplaceAllUsesWith,
	HasMetadata = lib.LLVMHasMetadata,
	GetMetadata = lib.LLVMGetMetadata,
	SetMetadata = lib.LLVMSetMetadata,
	IsAArgument = lib.LLVMIsAArgument,
	IsABasicBlock = lib.LLVMIsABasicBlock,
	IsAInlineAsm = lib.LLVMIsAInlineAsm,
	IsAUser = lib.LLVMIsAUser,
	IsAConstant = lib.LLVMIsAConstant,
	IsAConstantAggregateZero = lib.LLVMIsAConstantAggregateZero,
	IsAConstantArray = lib.LLVMIsAConstantArray,
	IsAConstantExpr = lib.LLVMIsAConstantExpr,
	IsAConstantFP = lib.LLVMIsAConstantFP,
	IsAConstantInt = lib.LLVMIsAConstantInt,
	IsAConstantPointerNull = lib.LLVMIsAConstantPointerNull,
	IsAConstantStruct = lib.LLVMIsAConstantStruct,
	IsAConstantVector = lib.LLVMIsAConstantVector,
	IsAGlobalValue = lib.LLVMIsAGlobalValue,
	IsAFunction = lib.LLVMIsAFunction,
	IsAGlobalAlias = lib.LLVMIsAGlobalAlias,
	IsAGlobalVariable = lib.LLVMIsAGlobalVariable,
	IsAUndefValue = lib.LLVMIsAUndefValue,
	IsAInstruction = lib.LLVMIsAInstruction,
	IsABinaryOperator = lib.LLVMIsABinaryOperator,
	IsACallInst = lib.LLVMIsACallInst,
	IsAIntrinsicInst = lib.LLVMIsAIntrinsicInst,
	IsADbgInfoIntrinsic = lib.LLVMIsADbgInfoIntrinsic,
	IsADbgDeclareInst = lib.LLVMIsADbgDeclareInst,
	IsAEHSelectorInst = lib.LLVMIsAEHSelectorInst,
	IsAMemIntrinsic = lib.LLVMIsAMemIntrinsic,
	IsAMemCpyInst = lib.LLVMIsAMemCpyInst,
	IsAMemMoveInst = lib.LLVMIsAMemMoveInst,
	IsAMemSetInst = lib.LLVMIsAMemSetInst,
	IsACmpInst = lib.LLVMIsACmpInst,
	IsAFCmpInst = lib.LLVMIsAFCmpInst,
	IsAICmpInst = lib.LLVMIsAICmpInst,
	IsAExtractElementInst = lib.LLVMIsAExtractElementInst,
	IsAGetElementPtrInst = lib.LLVMIsAGetElementPtrInst,
	IsAInsertElementInst = lib.LLVMIsAInsertElementInst,
	IsAInsertValueInst = lib.LLVMIsAInsertValueInst,
	IsAPHINode = lib.LLVMIsAPHINode,
	IsASelectInst = lib.LLVMIsASelectInst,
	IsAShuffleVectorInst = lib.LLVMIsAShuffleVectorInst,
	IsAStoreInst = lib.LLVMIsAStoreInst,
	IsATerminatorInst = lib.LLVMIsATerminatorInst,
	IsABranchInst = lib.LLVMIsABranchInst,
	IsAInvokeInst = lib.LLVMIsAInvokeInst,
	IsAReturnInst = lib.LLVMIsAReturnInst,
	IsASwitchInst = lib.LLVMIsASwitchInst,
	IsAUnreachableInst = lib.LLVMIsAUnreachableInst,
	IsAUnaryInstruction = lib.LLVMIsAUnaryInstruction,
	IsAAllocaInst = lib.LLVMIsAAllocaInst,
	IsACastInst = lib.LLVMIsACastInst,
	IsABitCastInst = lib.LLVMIsABitCastInst,
	IsAFPExtInst = lib.LLVMIsAFPExtInst,
	IsAFPToSIInst = lib.LLVMIsAFPToSIInst,
	IsAFPToUIInst = lib.LLVMIsAFPToUIInst,
	IsAFPTruncInst = lib.LLVMIsAFPTruncInst,
	IsAIntToPtrInst = lib.LLVMIsAIntToPtrInst,
	IsAPtrToIntInst = lib.LLVMIsAPtrToIntInst,
	IsASExtInst = lib.LLVMIsASExtInst,
	IsASIToFPInst = lib.LLVMIsASIToFPInst,
	IsATruncInst = lib.LLVMIsATruncInst,
	IsAUIToFPInst = lib.LLVMIsAUIToFPInst,
	IsAZExtInst = lib.LLVMIsAZExtInst,
	IsAExtractValueInst = lib.LLVMIsAExtractValueInst,
	IsALoadInst = lib.LLVMIsALoadInst,
	IsAVAArgInst = lib.LLVMIsAVAArgInst,
	GetFirstUse = lib.LLVMGetFirstUse,
	GetOperand = lib.LLVMGetOperand,
	SetOperand = lib.LLVMSetOperand,
	GetNumOperands = lib.LLVMGetNumOperands,
	IsConstant = lib.LLVMIsConstant,
	IsNull = lib.LLVMIsNull,
	IsUndef = lib.LLVMIsUndef,
	MDNode = lib.LLVMMDNode,
	ConstIntGetZExtValue = lib.LLVMConstIntGetZExtValue,
	ConstIntGetSExtValue = lib.LLVMConstIntGetSExtValue,
	ConstStruct = lib.LLVMConstStruct,
	ConstVector = lib.LLVMConstVector,
	GetConstOpcode = lib.LLVMGetConstOpcode,
	ConstNeg = lib.LLVMConstNeg,
	ConstNSWNeg = lib.LLVMConstNSWNeg,
	ConstNUWNeg = lib.LLVMConstNUWNeg,
	ConstFNeg = lib.LLVMConstFNeg,
	ConstNot = lib.LLVMConstNot,
	ConstAdd = lib.LLVMConstAdd,
	ConstNSWAdd = lib.LLVMConstNSWAdd,
	ConstNUWAdd = lib.LLVMConstNUWAdd,
	ConstFAdd = lib.LLVMConstFAdd,
	ConstSub = lib.LLVMConstSub,
	ConstNSWSub = lib.LLVMConstNSWSub,
	ConstNUWSub = lib.LLVMConstNUWSub,
	ConstFSub = lib.LLVMConstFSub,
	ConstMul = lib.LLVMConstMul,
	ConstNSWMul = lib.LLVMConstNSWMul,
	ConstNUWMul = lib.LLVMConstNUWMul,
	ConstFMul = lib.LLVMConstFMul,
	ConstUDiv = lib.LLVMConstUDiv,
	ConstSDiv = lib.LLVMConstSDiv,
	ConstExactSDiv = lib.LLVMConstExactSDiv,
	ConstFDiv = lib.LLVMConstFDiv,
	ConstURem = lib.LLVMConstURem,
	ConstSRem = lib.LLVMConstSRem,
	ConstFRem = lib.LLVMConstFRem,
	ConstAnd = lib.LLVMConstAnd,
	ConstOr = lib.LLVMConstOr,
	ConstXor = lib.LLVMConstXor,
	ConstShl = lib.LLVMConstShl,
	ConstLShr = lib.LLVMConstLShr,
	ConstAShr = lib.LLVMConstAShr,
	ConstGEP = lib.LLVMConstGEP,
	ConstInBoundsGEP = lib.LLVMConstInBoundsGEP,
	ConstTrunc = lib.LLVMConstTrunc,
	ConstSExt = lib.LLVMConstSExt,
	ConstZExt = lib.LLVMConstZExt,
	ConstFPTrunc = lib.LLVMConstFPTrunc,
	ConstFPExt = lib.LLVMConstFPExt,
	ConstUIToFP = lib.LLVMConstUIToFP,
	ConstSIToFP = lib.LLVMConstSIToFP,
	ConstFPToUI = lib.LLVMConstFPToUI,
	ConstFPToSI = lib.LLVMConstFPToSI,
	ConstPtrToInt = lib.LLVMConstPtrToInt,
	ConstIntToPtr = lib.LLVMConstIntToPtr,
	ConstBitCast = lib.LLVMConstBitCast,
	ConstZExtOrBitCast = lib.LLVMConstZExtOrBitCast,
	ConstSExtOrBitCast = lib.LLVMConstSExtOrBitCast,
	ConstTruncOrBitCast = lib.LLVMConstTruncOrBitCast,
	ConstPointerCast = lib.LLVMConstPointerCast,
	ConstIntCast = lib.LLVMConstIntCast,
	ConstFPCast = lib.LLVMConstFPCast,
	ConstSelect = lib.LLVMConstSelect,
	ConstExtractElement = lib.LLVMConstExtractElement,
	ConstInsertElement = lib.LLVMConstInsertElement,
	ConstShuffleVector = lib.LLVMConstShuffleVector,
	ConstExtractValue = lib.LLVMConstExtractValue,
	ConstInsertValue = lib.LLVMConstInsertValue,
	BlockAddress = lib.LLVMBlockAddress,
	GetGlobalParent = lib.LLVMGetGlobalParent,
	IsDeclaration = lib.LLVMIsDeclaration,
	GetLinkage = lib.LLVMGetLinkage,
	SetLinkage = lib.LLVMSetLinkage,
	GetSection = lib.LLVMGetSection,
	SetSection = lib.LLVMSetSection,
	GetVisibility = lib.LLVMGetVisibility,
	SetVisibility = lib.LLVMSetVisibility,
	GetAlignment = lib.LLVMGetAlignment,
	SetAlignment = lib.LLVMSetAlignment,
	GetNextGlobal = lib.LLVMGetNextGlobal,
	GetPreviousGlobal = lib.LLVMGetPreviousGlobal,
	DeleteGlobal = lib.LLVMDeleteGlobal,
	GetInitializer = lib.LLVMGetInitializer,
	SetInitializer = lib.LLVMSetInitializer,
	IsThreadLocal = lib.LLVMIsThreadLocal,
	SetThreadLocal = lib.LLVMSetThreadLocal,
	IsGlobalConstant = lib.LLVMIsGlobalConstant,
	SetGlobalConstant = lib.LLVMSetGlobalConstant,
	GetNextFunction = lib.LLVMGetNextFunction,
	GetPreviousFunction = lib.LLVMGetPreviousFunction,
	DeleteFunction = lib.LLVMDeleteFunction,
	GetIntrinsicID = lib.LLVMGetIntrinsicID,
	GetFunctionCallConv = lib.LLVMGetFunctionCallConv,
	SetFunctionCallConv = lib.LLVMSetFunctionCallConv,
	GetGC = lib.LLVMGetGC,
	SetGC = lib.LLVMSetGC,
	AddFunctionAttr = lib.LLVMAddFunctionAttr,
	GetFunctionAttr = lib.LLVMGetFunctionAttr,
	RemoveFunctionAttr = lib.LLVMRemoveFunctionAttr,
	CountParams = lib.LLVMCountParams,
	GetParams = lib.LLVMGetParams,
	GetParam = lib.LLVMGetParam,
	GetParamParent = lib.LLVMGetParamParent,
	GetFirstParam = lib.LLVMGetFirstParam,
	GetLastParam = lib.LLVMGetLastParam,
	GetNextParam = lib.LLVMGetNextParam,
	GetPreviousParam = lib.LLVMGetPreviousParam,
	AddAttribute = lib.LLVMAddAttribute,
	RemoveAttribute = lib.LLVMRemoveAttribute,
	GetAttribute = lib.LLVMGetAttribute,
	SetParamAlignment = lib.LLVMSetParamAlignment,
	ValueIsBasicBlock = lib.LLVMValueIsBasicBlock,
	ValueAsBasicBlock = lib.LLVMValueAsBasicBlock,
	CountBasicBlocks = lib.LLVMCountBasicBlocks,
	GetBasicBlocks = lib.LLVMGetBasicBlocks,
	GetFirstBasicBlock = lib.LLVMGetFirstBasicBlock,
	GetLastBasicBlock = lib.LLVMGetLastBasicBlock,
	GetEntryBasicBlock = lib.LLVMGetEntryBasicBlock,
	AppendBasicBlock = lib.LLVMAppendBasicBlock,
	GetInstructionParent = lib.LLVMGetInstructionParent,
	GetNextInstruction = lib.LLVMGetNextInstruction,
	GetPreviousInstruction = lib.LLVMGetPreviousInstruction,
	SetInstructionCallConv = lib.LLVMSetInstructionCallConv,
	GetInstructionCallConv = lib.LLVMGetInstructionCallConv,
	AddInstrAttribute = lib.LLVMAddInstrAttribute,
	RemoveInstrAttribute = lib.LLVMRemoveInstrAttribute,
	SetInstrParamAlignment = lib.LLVMSetInstrParamAlignment,
	IsTailCall = lib.LLVMIsTailCall,
	SetTailCall = lib.LLVMSetTailCall,
	AddIncoming = lib.LLVMAddIncoming,
	CountIncoming = lib.LLVMCountIncoming,
	GetIncomingValue = lib.LLVMGetIncomingValue,
	GetIncomingBlock = lib.LLVMGetIncomingBlock,
	AddCase = lib.LLVMAddCase,
	AddDestination = lib.LLVMAddDestination,
	VerifyFunction = lib.LLVMVerifyFunction,
	ViewFunctionCFG = lib.LLVMViewFunctionCFG,
	ViewFunctionCFGOnly = lib.LLVMViewFunctionCFGOnly,
}
Value.__index = Value


local BasicBlock = {
	BasicBlockAsValue = lib.LLVMBasicBlockAsValue,
	GetBasicBlockParent = lib.LLVMGetBasicBlockParent,
	GetNextBasicBlock = lib.LLVMGetNextBasicBlock,
	GetPreviousBasicBlock = lib.LLVMGetPreviousBasicBlock,
	InsertBasicBlock = lib.LLVMInsertBasicBlock,
	DeleteBasicBlock = lib.LLVMDeleteBasicBlock,
	MoveBasicBlockBefore = lib.LLVMMoveBasicBlockBefore,
	MoveBasicBlockAfter = lib.LLVMMoveBasicBlockAfter,
	GetFirstInstruction = lib.LLVMGetFirstInstruction,
	GetLastInstruction = lib.LLVMGetLastInstruction,
}
BasicBlock.__index = BasicBlock


local Builder = {
	PositionBuilder = lib.LLVMPositionBuilder,
	PositionBuilderBefore = lib.LLVMPositionBuilderBefore,
	PositionBuilderAtEnd = lib.LLVMPositionBuilderAtEnd,
	GetInsertBlock = lib.LLVMGetInsertBlock,
	ClearInsertionPosition = lib.LLVMClearInsertionPosition,
	InsertIntoBuilder = lib.LLVMInsertIntoBuilder,
	InsertIntoBuilderWithName = lib.LLVMInsertIntoBuilderWithName,
	DisposeBuilder = lib.LLVMDisposeBuilder,
	SetCurrentDebugLocation = lib.LLVMSetCurrentDebugLocation,
	GetCurrentDebugLocation = lib.LLVMGetCurrentDebugLocation,
	SetInstDebugLocation = lib.LLVMSetInstDebugLocation,
	BuildRetVoid = lib.LLVMBuildRetVoid,
	BuildRet = lib.LLVMBuildRet,
	BuildAggregateRet = lib.LLVMBuildAggregateRet,
	BuildBr = lib.LLVMBuildBr,
	BuildCondBr = lib.LLVMBuildCondBr,
	BuildSwitch = lib.LLVMBuildSwitch,
	BuildIndirectBr = lib.LLVMBuildIndirectBr,
	BuildInvoke = lib.LLVMBuildInvoke,
	BuildUnreachable = lib.LLVMBuildUnreachable,
	BuildAdd = lib.LLVMBuildAdd,
	BuildNSWAdd = lib.LLVMBuildNSWAdd,
	BuildNUWAdd = lib.LLVMBuildNUWAdd,
	BuildFAdd = lib.LLVMBuildFAdd,
	BuildSub = lib.LLVMBuildSub,
	BuildNSWSub = lib.LLVMBuildNSWSub,
	BuildNUWSub = lib.LLVMBuildNUWSub,
	BuildFSub = lib.LLVMBuildFSub,
	BuildMul = lib.LLVMBuildMul,
	BuildNSWMul = lib.LLVMBuildNSWMul,
	BuildNUWMul = lib.LLVMBuildNUWMul,
	BuildFMul = lib.LLVMBuildFMul,
	BuildUDiv = lib.LLVMBuildUDiv,
	BuildSDiv = lib.LLVMBuildSDiv,
	BuildExactSDiv = lib.LLVMBuildExactSDiv,
	BuildFDiv = lib.LLVMBuildFDiv,
	BuildURem = lib.LLVMBuildURem,
	BuildSRem = lib.LLVMBuildSRem,
	BuildFRem = lib.LLVMBuildFRem,
	BuildShl = lib.LLVMBuildShl,
	BuildLShr = lib.LLVMBuildLShr,
	BuildAShr = lib.LLVMBuildAShr,
	BuildAnd = lib.LLVMBuildAnd,
	BuildOr = lib.LLVMBuildOr,
	BuildXor = lib.LLVMBuildXor,
	BuildBinOp = lib.LLVMBuildBinOp,
	BuildNeg = lib.LLVMBuildNeg,
	BuildNSWNeg = lib.LLVMBuildNSWNeg,
	BuildNUWNeg = lib.LLVMBuildNUWNeg,
	BuildFNeg = lib.LLVMBuildFNeg,
	BuildNot = lib.LLVMBuildNot,
	BuildMalloc = lib.LLVMBuildMalloc,
	BuildArrayMalloc = lib.LLVMBuildArrayMalloc,
	BuildAlloca = lib.LLVMBuildAlloca,
	BuildArrayAlloca = lib.LLVMBuildArrayAlloca,
	BuildFree = lib.LLVMBuildFree,
	BuildLoad = lib.LLVMBuildLoad,
	BuildStore = lib.LLVMBuildStore,
	BuildGEP = lib.LLVMBuildGEP,
	BuildInBoundsGEP = lib.LLVMBuildInBoundsGEP,
	BuildStructGEP = lib.LLVMBuildStructGEP,
	BuildGlobalString = lib.LLVMBuildGlobalString,
	BuildGlobalStringPtr = lib.LLVMBuildGlobalStringPtr,
	BuildTrunc = lib.LLVMBuildTrunc,
	BuildZExt = lib.LLVMBuildZExt,
	BuildSExt = lib.LLVMBuildSExt,
	BuildFPToUI = lib.LLVMBuildFPToUI,
	BuildFPToSI = lib.LLVMBuildFPToSI,
	BuildUIToFP = lib.LLVMBuildUIToFP,
	BuildSIToFP = lib.LLVMBuildSIToFP,
	BuildFPTrunc = lib.LLVMBuildFPTrunc,
	BuildFPExt = lib.LLVMBuildFPExt,
	BuildPtrToInt = lib.LLVMBuildPtrToInt,
	BuildIntToPtr = lib.LLVMBuildIntToPtr,
	BuildBitCast = lib.LLVMBuildBitCast,
	BuildZExtOrBitCast = lib.LLVMBuildZExtOrBitCast,
	BuildSExtOrBitCast = lib.LLVMBuildSExtOrBitCast,
	BuildTruncOrBitCast = lib.LLVMBuildTruncOrBitCast,
	BuildCast = lib.LLVMBuildCast,
	BuildPointerCast = lib.LLVMBuildPointerCast,
	BuildIntCast = lib.LLVMBuildIntCast,
	BuildFPCast = lib.LLVMBuildFPCast,
	BuildICmp = lib.LLVMBuildICmp,
	BuildFCmp = lib.LLVMBuildFCmp,
	BuildPhi = lib.LLVMBuildPhi,
	BuildCall = lib.LLVMBuildCall,
	BuildSelect = lib.LLVMBuildSelect,
	BuildVAArg = lib.LLVMBuildVAArg,
	BuildExtractElement = lib.LLVMBuildExtractElement,
	BuildInsertElement = lib.LLVMBuildInsertElement,
	BuildShuffleVector = lib.LLVMBuildShuffleVector,
	BuildExtractValue = lib.LLVMBuildExtractValue,
	BuildInsertValue = lib.LLVMBuildInsertValue,
	BuildIsNull = lib.LLVMBuildIsNull,
	BuildIsNotNull = lib.LLVMBuildIsNotNull,
	BuildPtrDiff = lib.LLVMBuildPtrDiff,
}
Builder.__index = Builder


local ModuleProvider = {
	DisposeModuleProvider = lib.LLVMDisposeModuleProvider,
	CreateFunctionPassManager = lib.LLVMCreateFunctionPassManager,
}
ModuleProvider.__index = ModuleProvider


local MemoryBuffer = {
	CreateMemoryBufferWithSTDIN = lib.LLVMCreateMemoryBufferWithSTDIN,
	DisposeMemoryBuffer = lib.LLVMDisposeMemoryBuffer,
	ParseBitcode = lib.LLVMParseBitcode,
	GetBitcodeModule = lib.LLVMGetBitcodeModule,
	GetBitcodeModuleProvider = lib.LLVMGetBitcodeModuleProvider,
}
MemoryBuffer.__index = MemoryBuffer


local PassManager = {
	RunPassManager = lib.LLVMRunPassManager,
	InitializeFunctionPassManager = lib.LLVMInitializeFunctionPassManager,
	RunFunctionPassManager = lib.LLVMRunFunctionPassManager,
	FinalizeFunctionPassManager = lib.LLVMFinalizeFunctionPassManager,
	DisposePassManager = lib.LLVMDisposePassManager,
	AddArgumentPromotionPass = lib.LLVMAddArgumentPromotionPass,
	AddConstantMergePass = lib.LLVMAddConstantMergePass,
	AddDeadArgEliminationPass = lib.LLVMAddDeadArgEliminationPass,
	AddFunctionAttrsPass = lib.LLVMAddFunctionAttrsPass,
	AddFunctionInliningPass = lib.LLVMAddFunctionInliningPass,
	AddGlobalDCEPass = lib.LLVMAddGlobalDCEPass,
	AddGlobalOptimizerPass = lib.LLVMAddGlobalOptimizerPass,
	AddIPConstantPropagationPass = lib.LLVMAddIPConstantPropagationPass,
	AddPruneEHPass = lib.LLVMAddPruneEHPass,
	AddIPSCCPPass = lib.LLVMAddIPSCCPPass,
	AddInternalizePass = lib.LLVMAddInternalizePass,
	AddStripDeadPrototypesPass = lib.LLVMAddStripDeadPrototypesPass,
	AddStripSymbolsPass = lib.LLVMAddStripSymbolsPass,
	AddAggressiveDCEPass = lib.LLVMAddAggressiveDCEPass,
	AddCFGSimplificationPass = lib.LLVMAddCFGSimplificationPass,
	AddDeadStoreEliminationPass = lib.LLVMAddDeadStoreEliminationPass,
	AddGVNPass = lib.LLVMAddGVNPass,
	AddIndVarSimplifyPass = lib.LLVMAddIndVarSimplifyPass,
	AddInstructionCombiningPass = lib.LLVMAddInstructionCombiningPass,
	AddJumpThreadingPass = lib.LLVMAddJumpThreadingPass,
	AddLICMPass = lib.LLVMAddLICMPass,
	AddLoopDeletionPass = lib.LLVMAddLoopDeletionPass,
	AddLoopRotatePass = lib.LLVMAddLoopRotatePass,
	AddLoopUnrollPass = lib.LLVMAddLoopUnrollPass,
	AddLoopUnswitchPass = lib.LLVMAddLoopUnswitchPass,
	AddMemCpyOptPass = lib.LLVMAddMemCpyOptPass,
	AddPromoteMemoryToRegisterPass = lib.LLVMAddPromoteMemoryToRegisterPass,
	AddReassociatePass = lib.LLVMAddReassociatePass,
	AddSCCPPass = lib.LLVMAddSCCPPass,
	AddScalarReplAggregatesPass = lib.LLVMAddScalarReplAggregatesPass,
	AddScalarReplAggregatesPassWithThreshold = lib.LLVMAddScalarReplAggregatesPassWithThreshold,
	AddSimplifyLibCallsPass = lib.LLVMAddSimplifyLibCallsPass,
	AddTailCallEliminationPass = lib.LLVMAddTailCallEliminationPass,
	AddConstantPropagationPass = lib.LLVMAddConstantPropagationPass,
	AddDemoteMemoryToRegisterPass = lib.LLVMAddDemoteMemoryToRegisterPass,
	AddVerifierPass = lib.LLVMAddVerifierPass,
}
PassManager.__index = PassManager

local PassManagerBuilder = {
	SetOptLevel = lib.LLVMPassManagerBuilderSetOptLevel,
	SetSizeLevel = lib.LLVMPassManagerBuilderSetSizeLevel,
	SetDisableUnitAtATime = lib.LLVMPassManagerBuilderSetDisableUnitAtATime,
	SetDisableUnrollLoops = lib.LLVMPassManagerBuilderSetDisableUnrollLoops,
	SetDisableSimplifyLibCalls = lib.LLVMPassManagerBuilderSetDisableSimplifyLibCalls,
	UseInlinerWithThreshold = lib.LLVMPassManagerBuilderUseInlinerWithThreshold,
	PopulateFunctionPassManager = lib.LLVMPassManagerBuilderPopulateFunctionPassManager,
	PopulateModulePassManager = lib.LLVMPassManagerBuilderPopulateModulePassManager,
	PopulateLTOPassManager = lib.LLVMPassManagerBuilderPopulateLTOPassManager,
}
PassManagerBuilder.__index = PassManagerBuilder

local PassRegistry = {
	InitializeCore = lib.LLVMInitializeCore,
	InitializeTransformUtils = lib.LLVMInitializeTransformUtils,
	InitializeScalarOpts = lib.LLVMInitializeScalarOpts,
	InitializeInstCombine = lib.LLVMInitializeInstCombine,
	InitializeIPO = lib.LLVMInitializeIPO,
	InitializeAnalysis = lib.LLVMInitializeAnalysis,
	InitializeIPA = lib.LLVMInitializeIPA,
	InitializeCodeGen = lib.LLVMInitializeCodeGen,
	InitializeTarget = lib.LLVMInitializeTarget,
}
PassRegistry.__index = PassRegistry


local Use = {
	GetNextUse = lib.LLVMGetNextUse,
	GetUser = lib.LLVMGetUser,
	GetUsedValue = lib.LLVMGetUsedValue,
}
Use.__index = Use


local TargetData = {
	AddTargetData = lib.LLVMAddTargetData,
	CopyStringRepOfTargetData = lib.LLVMCopyStringRepOfTargetData,
	ByteOrder = lib.LLVMByteOrder,
	PointerSize = lib.LLVMPointerSize,
	IntPtrType = lib.LLVMIntPtrType,
	SizeOfTypeInBits = lib.LLVMSizeOfTypeInBits,
	StoreSizeOfType = lib.LLVMStoreSizeOfType,
	ABISizeOfType = lib.LLVMABISizeOfType,
	ABIAlignmentOfType = lib.LLVMABIAlignmentOfType,
	CallFrameAlignmentOfType = lib.LLVMCallFrameAlignmentOfType,
	PreferredAlignmentOfType = lib.LLVMPreferredAlignmentOfType,
	PreferredAlignmentOfGlobal = lib.LLVMPreferredAlignmentOfGlobal,
	ElementAtOffset = lib.LLVMElementAtOffset,
	OffsetOfElement = lib.LLVMOffsetOfElement,
	DisposeTargetData = lib.LLVMDisposeTargetData,
}
TargetData.__index = TargetData


local GenericValue = {
	GenericValueIntWidth = lib.LLVMGenericValueIntWidth,
	GenericValueToInt = lib.LLVMGenericValueToInt,
	GenericValueToPointer = lib.LLVMGenericValueToPointer,
	DisposeGenericValue = lib.LLVMDisposeGenericValue,
}
GenericValue.__index = GenericValue


local ExecutionEngine = {
	CreateExecutionEngineForModule = lib.LLVMCreateExecutionEngineForModule,
	CreateInterpreterForModule = lib.LLVMCreateInterpreterForModule,
	CreateJITCompilerForModule = lib.LLVMCreateJITCompilerForModule,
	CreateExecutionEngine = lib.LLVMCreateExecutionEngine,
	CreateInterpreter = lib.LLVMCreateInterpreter,
	CreateJITCompiler = lib.LLVMCreateJITCompiler,
	DisposeExecutionEngine = lib.LLVMDisposeExecutionEngine,
	RunStaticConstructors = lib.LLVMRunStaticConstructors,
	RunStaticDestructors = lib.LLVMRunStaticDestructors,
	RunFunctionAsMain = lib.LLVMRunFunctionAsMain,
	RunFunction = lib.LLVMRunFunction,
	FreeMachineCodeForFunction = lib.LLVMFreeMachineCodeForFunction,
	AddModule = lib.LLVMAddModule,
	AddModuleProvider = lib.LLVMAddModuleProvider,
	RemoveModule = lib.LLVMRemoveModule,
	RemoveModuleProvider = lib.LLVMRemoveModuleProvider,
	FindFunction = lib.LLVMFindFunction,
	RecompileAndRelinkFunction = lib.LLVMRecompileAndRelinkFunction,
	GetExecutionEngineTargetData = lib.LLVMGetExecutionEngineTargetData,
	AddGlobalMapping = lib.LLVMAddGlobalMapping,
	GetPointerToGlobal = lib.LLVMGetPointerToGlobal,
}
ExecutionEngine.__index = ExecutionEngine


-- <add methods here>

-- olevel is an integer to specify -O0, -O1, -O2, -O3
-- returns zero if no changes were made
function Module:optimize(olevel)
	local pm = llvm.PassManager()
	local pmb = llvm.PassManagerBuilder()
	pmb:SetOptLevel(olevel or 2)
	pmb:PopulateModulePassManager(pm)
	
	local modified = pm:RunPassManager(self)
	return modified	
end

function Builder:Call(F, outname, ...)
	local args = llvm.ValueArray(...)
	return self:BuildCall(F, args, select("#", ...), outname)
end

-- special case of creating lua_CFunction types and pushing on the current stack
-- (a bit of an ugly hack...)
function ExecutionEngine:GetLuaFunction(F)
	-- allocate a lua_CFunction ** in C:
	local bf = clang.make_function_box()
	-- copy in the ffi'd function:
	ffi.cast("void **", bf)[0] = self:GetPointerToGlobal(F)
	-- use C to push this as a lua_CFunction:
	-- stash the EE as an upvalue to prevent it being collected 
	return clang.unbox_function_box(bf, self)
end


ffi.metatype("struct LLVMOpaqueContext", Context)
ffi.metatype("struct LLVMOpaqueModule", Module)
ffi.metatype("struct LLVMOpaqueType", Type)
ffi.metatype("struct LLVMOpaqueValue", Value)
ffi.metatype("struct LLVMOpaqueBasicBlock", BasicBlock)
ffi.metatype("struct LLVMOpaqueBuilder", Builder)
ffi.metatype("struct LLVMOpaqueModuleProvider", ModuleProvider)
ffi.metatype("struct LLVMOpaqueMemoryBuffer", MemoryBuffer)
ffi.metatype("struct LLVMOpaquePassManager", PassManager)
ffi.metatype("struct LLVMOpaquePassRegistry", PassRegistry)
ffi.metatype("struct LLVMOpaquePassManagerBuilder", PassManagerBuilder)
ffi.metatype("struct LLVMOpaqueUse", Use)
ffi.metatype("struct LLVMOpaqueTargetData", TargetData)
ffi.metatype("struct LLVMOpaqueGenericValue", GenericValue)
ffi.metatype("struct LLVMOpaqueExecutionEngine", ExecutionEngine)


---- END OF METATYPES ---

-- initialize native target:
lib.LLVMInitializeX86TargetInfo()
lib.LLVMInitializeX86Target()
lib.LLVMInitializeX86TargetMC()


-- Friendly API:

function llvm.Context(name, Ctx)
	local o = lib.LLVMContextCreate()
	ffi.gc(o, lib.LLVMContextDispose)
	return o
end

function llvm.Module(name, Ctx)
	local o = Ctx and lib.LLVMModuleCreateWithNameInContext(name, Ctx) or lib.LLVMModuleCreateWithName(name)
	ffi.gc(o, lib.LLVMDisposeModule)
	return o
end

function llvm.FunctionType(ret, args, isvarargs)
	local numargs = 1
	if type(args) == "table" then numargs = #args end
	local argsTypes = ffi.new("LLVMTypeRef[?]", numargs, args)
	return lib.LLVMFunctionType(ret, argsTypes, numargs, isvarargs and true or false)
end

function llvm.ValueArray(...)
	local numargs = select("#", ...)
	return ffi.new("LLVMValueRef[?]", numargs, ...)
end

function llvm.Builder(Ctx)
	local B = Ctx and lib.LLVMCreateBuilderInContext(Ctx) or lib.LLVMCreateBuilder()
	ffi.gc(B, lib.LLVMDisposeBuilder)
	return B
end

function llvm.ExecutionEngine(M, InlineLevel)
	local EEptr = ffi.new("LLVMExecutionEngineRef[1]")
	local err = ffi.new("char *[1024]")		-- not quite right...
	assert(lib.LLVMCreateJITCompilerForModule(EEptr, M, InlineLevel or 1, err) == 0, "failed to create execution engine")
	-- EE takes ownership of Module:
	ffi.gc(M, nil)
	
	local o = EEptr[0]
	ffi.gc(o, DisposeExecutionEngine)
	return o
end

function llvm.PassManager()
	local o = lib.LLVMCreatePassManager()
	ffi.gc(o, lib.LLVMDisposePassManager)
	return o
end

function llvm.PassManagerBuilder()
	local o = lib.LLVMPassManagerBuilderCreate()
	ffi.gc(o, lib.LLVMPassManagerBuilderDispose)
	return o
end

-- All other lib.LLVM* functions become llvm.*:
setmetatable(llvm, {
	__index = function(self, k)
		local prefixed = "LLVM" .. k
		local v = lib[prefixed]
		self[k] = v
		return v
	end,
})

return llvm