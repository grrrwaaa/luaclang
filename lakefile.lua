-- Lake file for luaclang

LLVM_DIR = 'llvm-3.0'
LLVM_LIBS = "LLVMJIT LLVMTarget LLVMScalarOpts LLVMSupport LLVMLinker LLVMCore LLVMMC LLVMAnalysis LLVMipa LLVMipo LLVMTransformUtils LLVMInstCombine LLVMExecutionEngine LLVMCodeGen LLVMX86CodeGen LLVMX86Utils LLVMX86AsmPrinter LLVMSelectionDAG LLVMX86Desc LLVMX86Info LLVMX86Disassembler LLVMX86AsmParser LLVMMCParser LLVMAsmPrinter"

CLANG_DIR = 'llvm-3.0'
CLANG_LIBS = "clangBasic clangFrontend clangAST clangSerialization clangSema clangLex clangAnalysis clangCodeGen clangDriver clangParse"

cpp.shared{ 
	'clang', 
	-- source files:
	src = {
		'src/Compiler.cpp',
		'src/luaopen_clang.cpp', 
	},
	-- LLVM needs these defines / flags:
	defines = {
		"__STDC_LIMIT_MACROS", 
		"__STDC_CONSTANT_MACROS",
	},
	flags = "-fno-rtti",
	-- library dependencies:
	needs = { 
		'lua', 
		'llvm',
		'clang',
	},
	-- put results in debug / release folders 
	--odir = true,	
}