-- Lake file for luaclang
if PLAT == "Darwin" then
	LLVM_DIR = 'osx/llvm-3.0'
	LLVM_LIBS = "LLVMJIT LLVMTarget LLVMScalarOpts LLVMSupport LLVMLinker LLVMCore LLVMMC LLVMAnalysis LLVMipa LLVMipo LLVMTransformUtils LLVMInstCombine LLVMExecutionEngine LLVMCodeGen LLVMX86CodeGen LLVMX86Utils LLVMX86AsmPrinter LLVMSelectionDAG LLVMX86Desc LLVMX86Info LLVMX86Disassembler LLVMX86AsmParser LLVMMCParser LLVMAsmPrinter"

	CLANG_DIR = 'osx/llvm-3.0'
	CLANG_LIBS = "clangBasic clangFrontend clangAST clangSerialization clangSema clangLex clangAnalysis clangCodeGen clangDriver clangParse"

elseif PLAT == "Linux" then
	LLVM_DIR = 'linux/llvm-3.0'
	LLVM_LIBS = "LLVMXCoreCodeGen LLVMTableGen LLVMSystemZCodeGen LLVMSparcCodeGen LLVMPTXCodeGen LLVMPowerPCCodeGen LLVMMSP430CodeGen LLVMMipsCodeGen LLVMMCJIT LLVMRuntimeDyld LLVMObject LLVMMCDisassembler LLVMXCoreDesc LLVMXCoreInfo LLVMSystemZDesc LLVMSystemZInfo LLVMSparcDesc LLVMSparcInfo LLVMPowerPCDesc LLVMPowerPCInfo LLVMPowerPCAsmPrinter LLVMPTXDesc LLVMPTXInfo LLVMPTXAsmPrinter LLVMMipsDesc LLVMMipsInfo LLVMMipsAsmPrinter LLVMMSP430Desc LLVMMSP430Info LLVMMSP430AsmPrinter LLVMMBlazeDisassembler LLVMMBlazeAsmParser LLVMMBlazeCodeGen LLVMMBlazeDesc LLVMMBlazeAsmPrinter LLVMMBlazeInfo LLVMLinker LLVMipo LLVMInterpreter LLVMInstrumentation LLVMJIT LLVMExecutionEngine LLVMDebugInfo LLVMCppBackend LLVMCppBackendInfo LLVMCellSPUCodeGen LLVMCellSPUDesc LLVMCellSPUInfo LLVMCBackend LLVMCBackendInfo LLVMBlackfinCodeGen LLVMBlackfinDesc LLVMBlackfinInfo LLVMBitWriter LLVMX86Disassembler LLVMX86AsmParser LLVMX86CodeGen LLVMX86Desc LLVMX86AsmPrinter LLVMX86Utils LLVMX86Info LLVMAsmParser LLVMARMDisassembler LLVMARMAsmParser LLVMARMCodeGen LLVMARMDesc LLVMARMAsmPrinter LLVMARMInfo LLVMArchive LLVMBitReader LLVMAlphaCodeGen LLVMSelectionDAG LLVMAsmPrinter LLVMMCParser LLVMCodeGen LLVMScalarOpts LLVMInstCombine LLVMTransformUtils LLVMipa LLVMAnalysis LLVMTarget LLVMCore LLVMAlphaDesc LLVMAlphaInfo LLVMMC LLVMSupport"

	CLANG_DIR = 'linux/llvm-3.0'
	CLANG_LIBS = "clangFrontend clangLex clangParse clangDriver clangCodeGen clangSema clangSerialization clangAnalysis clangAST clangBasic "

	PTHREAD_DIR = '/usr/'
	PTHREAD_LIBS = 'pthread' --> please set!

else
	print (PLAT, "not supported")
end

cpp.shared{ 
	'clang', 
	-- source files:
	src = {
		'src/Compiler.cpp',
		'src/luaopen_clang.cpp', 
	},
	-- LLVM needs these defines / flags:
	defines = {
		"NDEBUG",
		"_GNU_SOURCE",
		"__STDC_LIMIT_MACROS", 
		"__STDC_CONSTANT_MACROS",
		"__STDC_FORMAT_MACROS",
	},
	flags = "-fno-rtti -fomit-frame-pointer -fno-exceptions -fPIC -O3",
	--flags = "-fno-rtti -fomit-frame-pointer -fno-exceptions -fPIC -O3 -Woverloaded-virtual -Wcast-qual",
	-- library dependencies:
	needs = { 	
		'clang',
		'llvm',
		'pthread',	-- Linux only?
		'lua', 
	},
	-- put results in debug / release folders 
	--odir = true,	
}
