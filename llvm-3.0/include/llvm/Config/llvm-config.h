/* include/llvm/Config/llvm-config.h.  Generated from llvm-config.h.in by configure.  */
/*===-- llvm/config/llvm-config.h - llvm configure variable -------*- C -*-===*/
/*                                                                            */
/*                     The LLVM Compiler Infrastructure                       */
/*                                                                            */
/* This file is distributed under the University of Illinois Open Source      */
/* License. See LICENSE.TXT for details.                                      */
/*                                                                            */
/*===----------------------------------------------------------------------===*/

/* This file enumerates all of the llvm variables from configure so that
   they can be in exported headers and won't override package specific
   directives.  This is a C file so we can include it in the llvm-c headers.  */

/* To avoid multiple inclusions of these variables when we include the exported
   headers and config.h, conditionally include these.  */
/* TODO: This is a bit of a hack.  */
#ifndef CONFIG_H

/* Installation directory for binary executables */
#define LLVM_BINDIR "/Volumes/Sandbox/llvm/3.0-release/final/Phase3/Release/llvmCore-3.0-final.install/bin"

/* Time at which LLVM was configured */
#define LLVM_CONFIGTIME "Mon Nov 28 18:55:28 PST 2011"

/* Installation directory for data files */
#define LLVM_DATADIR "/Volumes/Sandbox/llvm/3.0-release/final/Phase3/Release/llvmCore-3.0-final.install/share/llvm"

/* Installation directory for documentation */
#define LLVM_DOCSDIR "/Volumes/Sandbox/llvm/3.0-release/final/Phase3/Release/llvmCore-3.0-final.install/share/doc/llvm"

/* Installation directory for config files */
#define LLVM_ETCDIR "/Volumes/Sandbox/llvm/3.0-release/final/Phase3/Release/llvmCore-3.0-final.install/etc/llvm"

/* Has gcc/MSVC atomic intrinsics */
#define LLVM_HAS_ATOMICS 1

/* Host triple we were built on */
#define LLVM_HOSTTRIPLE "x86_64-apple-darwin12.0.0"

/* Installation directory for include files */
#define LLVM_INCLUDEDIR "/Volumes/Sandbox/llvm/3.0-release/final/Phase3/Release/llvmCore-3.0-final.install/include"

/* Installation directory for .info files */
#define LLVM_INFODIR "/Volumes/Sandbox/llvm/3.0-release/final/Phase3/Release/llvmCore-3.0-final.install/info"

/* Installation directory for libraries */
#define LLVM_LIBDIR "/Volumes/Sandbox/llvm/3.0-release/final/Phase3/Release/llvmCore-3.0-final.install/lib"

/* Installation directory for man pages */
#define LLVM_MANDIR "/Volumes/Sandbox/llvm/3.0-release/final/Phase3/Release/llvmCore-3.0-final.install/man"

/* LLVM architecture name for the native architecture, if available */
#define LLVM_NATIVE_ARCH X86

/* LLVM name for the native AsmParser init function, if available */
#define LLVM_NATIVE_ASMPARSER LLVMInitializeX86AsmParser

/* LLVM name for the native AsmPrinter init function, if available */
#define LLVM_NATIVE_ASMPRINTER LLVMInitializeX86AsmPrinter

/* LLVM name for the native Target init function, if available */
#define LLVM_NATIVE_TARGET LLVMInitializeX86Target

/* LLVM name for the native TargetInfo init function, if available */
#define LLVM_NATIVE_TARGETINFO LLVMInitializeX86TargetInfo

/* LLVM name for the native target MC init function, if available */
#define LLVM_NATIVE_TARGETMC LLVMInitializeX86TargetMC

/* Define if this is Unixish platform */
#define LLVM_ON_UNIX 1

/* Define if this is Win32ish platform */
/* #undef LLVM_ON_WIN32 */

/* Define to path to circo program if found or 'echo circo' otherwise */
#define LLVM_PATH_CIRCO "/usr/local/bin/circo"

/* Define to path to dot program if found or 'echo dot' otherwise */
#define LLVM_PATH_DOT "/usr/local/bin/dot"

/* Define to path to dotty program if found or 'echo dotty' otherwise */
#define LLVM_PATH_DOTTY "/usr/local/bin/dotty"

/* Define to path to fdp program if found or 'echo fdp' otherwise */
#define LLVM_PATH_FDP "/usr/local/bin/fdp"

/* Define to path to Graphviz program if found or 'echo Graphviz' otherwise */
#define LLVM_PATH_GRAPHVIZ "/Applications/Graphviz.app/Contents/MacOS/Graphviz"

/* Define to path to gv program if found or 'echo gv' otherwise */
/* #undef LLVM_PATH_GV */

/* Define to path to neato program if found or 'echo neato' otherwise */
#define LLVM_PATH_NEATO "/usr/local/bin/neato"

/* Define to path to twopi program if found or 'echo twopi' otherwise */
#define LLVM_PATH_TWOPI "/usr/local/bin/twopi"

/* Define to path to xdot.py program if found or 'echo xdot.py' otherwise */
/* #undef LLVM_PATH_XDOT_PY */

/* Installation prefix directory */
#define LLVM_PREFIX "/Volumes/Sandbox/llvm/3.0-release/final/Phase3/Release/llvmCore-3.0-final.install"

#endif
