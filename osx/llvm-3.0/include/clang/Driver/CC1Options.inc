//===- TableGen'erated file -------------------------------------*- C++ -*-===//
//
// Option Parsing Definitions
//
// Automatically generated file, do not edit!
//
//===----------------------------------------------------------------------===//

#ifndef OPTION
#error "Define OPTION prior to including this file!"
#endif

/////////
// Groups

OPTION("<action group>", Action_Group, Group, INVALID, INVALID, 0, 0, 0, 0)

//////////
// Options

OPTION("<input>", INPUT, Input, INVALID, INVALID, DriverOption, 0, 0, 0)
OPTION("<unknown>", UNKNOWN, Unknown, INVALID, INVALID, 0, 0, 0, 0)
OPTION("--help", _help, Flag, INVALID, help, 0, 0, 0, 0)
OPTION("--version", _version, Flag, INVALID, version, 0, 0, 0, 0)
OPTION("-CC", CC, Flag, INVALID, INVALID, 0, 0,
       "Enable comment output in -E mode, even from macro expansions", 0)
OPTION("-C", C, Flag, INVALID, INVALID, 0, 0,
       "Enable comment output in -E mode", 0)
OPTION("-D", D, JoinedOrSeparate, INVALID, INVALID, 0, 0,
       "Predefine the specified macro", "<macro>")
OPTION("-Eonly", Eonly, Flag, Action_Group, INVALID, 0, 0,
       "Just run preprocessor, no output (for timings)", 0)
OPTION("-E", E, Flag, Action_Group, INVALID, 0, 0,
       "Run preprocessor, emit preprocessed file", 0)
OPTION("-F", F, JoinedOrSeparate, INVALID, INVALID, 0, 0,
       "Add directory to framework include search path", "<directory>")
OPTION("-H", H, Flag, INVALID, INVALID, 0, 0,
       "Show header includes and nesting depth", 0)
OPTION("-I", I, JoinedOrSeparate, INVALID, INVALID, 0, 0,
       "Add directory to include search path", "<directory>")
OPTION("-MG", MG, Flag, INVALID, INVALID, 0, 0,
       "Add missing headers to dependency list", 0)
OPTION("-MP", MP, Flag, INVALID, INVALID, 0, 0,
       "Create phony target for each dependency (other than main file)", 0)
OPTION("-MQ", MQ, Separate, INVALID, INVALID, 0, 0,
       "Specify target to quote for dependency", 0)
OPTION("-MT", MT, Separate, INVALID, INVALID, 0, 0,
       "Specify target for dependency", 0)
OPTION("-Os", Os, Flag, INVALID, INVALID, 0, 0,
       "Optimize for size", 0)
OPTION("-Oz", Oz, Flag, INVALID, INVALID, 0, 0,
       "Optimize for size, regardless of performance", 0)
OPTION("-O", O, Joined, INVALID, INVALID, 0, 0,
       "Optimization level", 0)
OPTION("-P", P, Flag, INVALID, INVALID, 0, 0,
       "Disable linemarker output in -E mode", 0)
OPTION("-S", S, Flag, Action_Group, INVALID, 0, 0,
       "Emit native assembly code", 0)
OPTION("-U", U, JoinedOrSeparate, INVALID, INVALID, 0, 0,
       "Undefine the specified macro", "<macro>")
OPTION("-Wlarge-by-value-copy=", Wlarge_by_value_copy_EQ, Joined, INVALID, Wlarge_by_value_copy, 0, 0, 0, 0)
OPTION("-Wlarge-by-value-copy", Wlarge_by_value_copy, Separate, INVALID, INVALID, 0, 0,
       "Warn if a function definition returns or accepts an object larger in bytes that a given value", 0)
OPTION("-Wno-rewrite-macros", Wno_rewrite_macros, Flag, INVALID, INVALID, 0, 0,
       "Silence ObjC rewriting warnings", 0)
OPTION("-W", W, Joined, INVALID, INVALID, 0, 0, 0, 0)
OPTION("-add-plugin", add_plugin, Separate, INVALID, INVALID, 0, 0,
       "Use the named plugin action in addition to the default action", "<name>")
OPTION("-analyze-function=", analyze_function_EQ, Joined, INVALID, analyze_function, 0, 0, 0, 0)
OPTION("-analyze-function", analyze_function, Separate, INVALID, INVALID, 0, 0,
       "Run analysis on specific function", 0)
OPTION("-analyzer-checker-help", analyzer_checker_help, Flag, INVALID, INVALID, 0, 0,
       "Display the list of analyzer checkers that are available", 0)
OPTION("-analyzer-checker=", analyzer_checker_EQ, Joined, INVALID, analyzer_checker, 0, 0, 0, 0)
OPTION("-analyzer-checker", analyzer_checker, Separate, INVALID, INVALID, 0, 0,
       "Choose analyzer checkers to enable", 0)
OPTION("-analyzer-constraints=", analyzer_constraints_EQ, Joined, INVALID, analyzer_constraints, 0, 0, 0, 0)
OPTION("-analyzer-constraints", analyzer_constraints, Separate, INVALID, INVALID, 0, 0,
       "Source Code Analysis - Symbolic Constraint Engines", 0)
OPTION("-analyzer-disable-checker=", analyzer_disable_checker_EQ, Joined, INVALID, analyzer_disable_checker, 0, 0, 0, 0)
OPTION("-analyzer-disable-checker", analyzer_disable_checker, Separate, INVALID, INVALID, 0, 0,
       "Choose analyzer checkers to disable", 0)
OPTION("-analyzer-display-progress", analyzer_display_progress, Flag, INVALID, INVALID, 0, 0,
       "Emit verbose output about the analyzer's progress", 0)
OPTION("-analyzer-eagerly-assume", analyzer_eagerly_assume, Flag, INVALID, INVALID, 0, 0,
       "Eagerly assume the truth/falseness of some symbolic constraints", 0)
OPTION("-analyzer-inline-call", analyzer_inline_call, Flag, INVALID, INVALID, 0, 0,
       "Experimental transfer function inlining callees when its definition is available.", 0)
OPTION("-analyzer-max-loop", analyzer_max_loop, Separate, INVALID, INVALID, 0, 0,
       "The maximum number of times the analyzer will go through a loop", 0)
OPTION("-analyzer-max-nodes", analyzer_max_nodes, Separate, INVALID, INVALID, 0, 0,
       "The maximum number of nodes the analyzer can generate (150000 default, 0 = no limit)", 0)
OPTION("-analyzer-no-eagerly-trim-egraph", analyzer_no_eagerly_trim_egraph, Flag, INVALID, INVALID, 0, 0,
       "Don't eagerly remove uninteresting ExplodedNodes from the ExplodedGraph", 0)
OPTION("-analyzer-opt-analyze-headers", analyzer_opt_analyze_headers, Flag, INVALID, INVALID, 0, 0,
       "Force the static analyzer to analyze functions defined in header files", 0)
OPTION("-analyzer-opt-analyze-nested-blocks", analyzer_opt_analyze_nested_blocks, Flag, INVALID, INVALID, 0, 0,
       "Analyze the definitions of blocks in addition to functions", 0)
OPTION("-analyzer-output=", analyzer_output_EQ, Joined, INVALID, analyzer_output, 0, 0, 0, 0)
OPTION("-analyzer-output", analyzer_output, Separate, INVALID, INVALID, 0, 0,
       "Source Code Analysis - Output Options", 0)
OPTION("-analyzer-purge=", analyzer_purge_EQ, Joined, INVALID, analyzer_purge, 0, 0, 0, 0)
OPTION("-analyzer-purge", analyzer_purge, Separate, INVALID, INVALID, 0, 0,
       "Source Code Analysis - Dead Symbol Removal Frequency", 0)
OPTION("-analyzer-store=", analyzer_store_EQ, Joined, INVALID, analyzer_store, 0, 0, 0, 0)
OPTION("-analyzer-store", analyzer_store, Separate, INVALID, INVALID, 0, 0,
       "Source Code Analysis - Abstract Memory Store Models", 0)
OPTION("-analyzer-viz-egraph-graphviz", analyzer_viz_egraph_graphviz, Flag, INVALID, INVALID, 0, 0,
       "Display exploded graph using GraphViz", 0)
OPTION("-analyzer-viz-egraph-ubigraph", analyzer_viz_egraph_ubigraph, Flag, INVALID, INVALID, 0, 0,
       "Display exploded graph using Ubigraph", 0)
OPTION("-analyze", analyze, Flag, Action_Group, INVALID, 0, 0,
       "Run static analysis engine", 0)
OPTION("-arcmt-check", arcmt_check, Flag, INVALID, INVALID, 0, 0,
       "Check for ARC migration issues that need manual handling", 0)
OPTION("-arcmt-migrate-directory", arcmt_migrate_directory, Separate, INVALID, INVALID, 0, 0,
       "Directory for temporary files produced during ARC migration", 0)
OPTION("-arcmt-migrate-emit-errors", arcmt_migrate_emit_arc_errors, Flag, INVALID, INVALID, 0, 0,
       "Emit ARC errors even if the migrator can fix them", 0)
OPTION("-arcmt-migrate-report-output", arcmt_migrate_report_output, Separate, INVALID, INVALID, 0, 0,
       "Output path for the plist report", 0)
OPTION("-arcmt-migrate", arcmt_migrate, Flag, INVALID, INVALID, 0, 0,
       "Apply modifications and produces temporary files that conform to ARC", 0)
OPTION("-arcmt-modify", arcmt_modify, Flag, INVALID, INVALID, 0, 0,
       "Apply modifications to files to conform to ARC", 0)
OPTION("-ast-dump-xml", ast_dump_xml, Flag, Action_Group, INVALID, 0, 0,
       "Build ASTs and then debug dump them in a verbose XML format", 0)
OPTION("-ast-dump", ast_dump, Flag, Action_Group, INVALID, 0, 0,
       "Build ASTs and then debug dump them", 0)
OPTION("-ast-merge", ast_merge, Separate, INVALID, INVALID, 0, 0,
       "Merge the given AST file into the translation unit being compiled.", "<ast file>")
OPTION("-ast-print", ast_print, Flag, Action_Group, INVALID, 0, 0,
       "Build ASTs and then pretty-print them", 0)
OPTION("-ast-view", ast_view, Flag, Action_Group, INVALID, 0, 0,
       "Build ASTs and view them with GraphViz", 0)
OPTION("-backend-option", backend_option, Separate, INVALID, INVALID, 0, 0,
       "Additional arguments to forward to LLVM backend (during code gen)", 0)
OPTION("-c-isystem", c_isystem, JoinedOrSeparate, INVALID, INVALID, 0, 0,
       "Add directory to the C SYSTEM include search path", "<directory>")
OPTION("-cc1", cc1, Flag, INVALID, INVALID, 0, 0, 0, 0)
OPTION("-cfg-add-implicit-dtors", analysis_CFGAddImplicitDtors, Flag, INVALID, INVALID, 0, 0,
       "Add C++ implicit destructors to CFGs for all analyses", 0)
OPTION("-cfg-add-initializers", analysis_CFGAddInitializers, Flag, INVALID, INVALID, 0, 0,
       "Add C++ initializers to CFGs for all analyses", 0)
OPTION("-chain-include", chain_include, Separate, INVALID, INVALID, 0, 0,
       "Include and chain a header file after turning it into PCH", "<file>")
OPTION("-cl-fast-relaxed-math", cl_fast_relaxed_math, Flag, INVALID, INVALID, 0, 0,
       "OpenCL only. Sets -cl-finite-math-only and -cl-unsafe-math-optimizations, and defines __FAST_RELAXED_MATH__", 0)
OPTION("-cl-finite-math-only", cl_finite_math_only, Flag, INVALID, INVALID, 0, 0,
       "OpenCL only. Allow floating-point optimizations that assume arguments and results are not NaNs or +-Inf.", 0)
OPTION("-cl-mad-enable", cl_mad_enable, Flag, INVALID, INVALID, 0, 0,
       "OpenCL only. Enable less precise MAD instructions to be generated.", 0)
OPTION("-cl-opt-disable", cl_opt_disable, Flag, INVALID, INVALID, 0, 0,
       "OpenCL only. This option disables all optimizations. The default is optimizations are enabled.", 0)
OPTION("-cl-single-precision-constant", cl_single_precision_constant, Flag, INVALID, INVALID, 0, 0,
       "OpenCL only. Treat double precision floating-point constant as single precision constant.", 0)
OPTION("-cl-std=", cl_std_EQ, Joined, INVALID, INVALID, 0, 0,
       "OpenCL language standard to compile for", 0)
OPTION("-cl-unsafe-math-optimizations", cl_unsafe_math_optimizations, Flag, INVALID, INVALID, 0, 0,
       "OpenCL only. Allow unsafe floating-point optimizations.  Also implies -cl-no-signed-zeros and -cl-mad-enable", 0)
OPTION("-code-completion-at=", code_completion_at_EQ, Joined, INVALID, code_completion_at, 0, 0, 0, 0)
OPTION("-code-completion-at", code_completion_at, Separate, INVALID, INVALID, 0, 0,
       "Dump code-completion information at a location", "<file>:<line>:<column>")
OPTION("-code-completion-macros", code_completion_macros, Flag, INVALID, INVALID, 0, 0,
       "Include macros in code-completion results", 0)
OPTION("-code-completion-patterns", code_completion_patterns, Flag, INVALID, INVALID, 0, 0,
       "Include code patterns in code-completion results", 0)
OPTION("-coverage-file=", coverage_file_EQ, Joined, INVALID, coverage_file, 0, 0, 0, 0)
OPTION("-coverage-file", coverage_file, Separate, INVALID, INVALID, 0, 0,
       "Emit coverage data to this filename. The extension will be replaced.", 0)
OPTION("-cxx-abi", cxx_abi, Separate, INVALID, INVALID, 0, 0,
       "Target a particular C++ ABI type", 0)
OPTION("-cxx-isystem", cxx_isystem, JoinedOrSeparate, INVALID, INVALID, 0, 0,
       "Add directory to the C++ SYSTEM include search path", "<directory>")
OPTION("-dD", dD, Flag, INVALID, INVALID, 0, 0,
       "Print macro definitions in -E mode in addition to normal output", 0)
OPTION("-dM", dM, Flag, INVALID, INVALID, 0, 0,
       "Print macro definitions in -E mode instead of normal output", 0)
OPTION("-dependency-file", dependency_file, Separate, INVALID, INVALID, 0, 0,
       "Filename (or -) to write dependency output to", 0)
OPTION("-detailed-preprocessing-record", detailed_preprocessing_record, Flag, INVALID, INVALID, 0, 0,
       "include a detailed record of preprocessing actions", 0)
OPTION("-diagnostic-log-file", diagnostic_log_file, Separate, INVALID, INVALID, 0, 0,
       "Filename (or -) to log diagnostics to", 0)
OPTION("-disable-free", disable_free, Flag, INVALID, INVALID, 0, 0,
       "Disable freeing of memory on exit", 0)
OPTION("-disable-llvm-optzns", disable_llvm_optzns, Flag, INVALID, INVALID, 0, 0,
       "Don't run LLVM optimization passes", 0)
OPTION("-disable-llvm-verifier", disable_llvm_verifier, Flag, INVALID, INVALID, 0, 0,
       "Don't run the LLVM IR verifier pass", 0)
OPTION("-disable-red-zone", disable_red_zone, Flag, INVALID, INVALID, 0, 0,
       "Do not emit code that uses the red zone.", 0)
OPTION("-dump-build-information", dump_build_information, Separate, INVALID, INVALID, 0, 0,
       "output a dump of some build information to a file", "<filename>")
OPTION("-dump-deserialized-decls", dump_deserialized_pch_decls, Flag, INVALID, INVALID, 0, 0,
       "Dump declarations that are deserialized from PCH, for testing", 0)
OPTION("-dump-raw-tokens", dump_raw_tokens, Flag, Action_Group, INVALID, 0, 0,
       "Lex file in raw mode and dump raw tokens", 0)
OPTION("-dump-tokens", dump_tokens, Flag, Action_Group, INVALID, 0, 0,
       "Run preprocessor, dump internal rep of tokens", 0)
OPTION("-dwarf-debug-flags", dwarf_debug_flags, Separate, INVALID, INVALID, 0, 0,
       "The string to embed in the Dwarf debug flags record.", 0)
OPTION("-emit-codegen-only", emit_codegen_only, Flag, Action_Group, INVALID, 0, 0,
       "Generate machine code, but discard output", 0)
OPTION("-emit-html", emit_html, Flag, Action_Group, INVALID, 0, 0,
       "Output input source as HTML", 0)
OPTION("-emit-llvm-bc", emit_llvm_bc, Flag, Action_Group, INVALID, 0, 0,
       "Build ASTs then convert to LLVM, emit .bc file", 0)
OPTION("-emit-llvm-only", emit_llvm_only, Flag, Action_Group, INVALID, 0, 0,
       "Build ASTs and convert to LLVM, discarding output", 0)
OPTION("-emit-llvm", emit_llvm, Flag, Action_Group, INVALID, 0, 0,
       "Build ASTs then convert to LLVM, emit .ll file", 0)
OPTION("-emit-module", emit_module, Flag, Action_Group, INVALID, 0, 0,
       "Generate pre-compiled module file", 0)
OPTION("-emit-obj", emit_obj, Flag, Action_Group, INVALID, 0, 0,
       "Emit native object files", 0)
OPTION("-emit-pch", emit_pch, Flag, Action_Group, INVALID, 0, 0,
       "Generate pre-compiled header file", 0)
OPTION("-emit-pth", emit_pth, Flag, Action_Group, INVALID, 0, 0,
       "Generate pre-tokenized header file", 0)
OPTION("-error-on-deserialized-decl", error_on_deserialized_pch_decl, Separate, INVALID, INVALID, 0, 0,
       "Emit error if a specific declaration is deserialized from PCH, for testing", 0)
OPTION("-faltivec", faltivec, Flag, INVALID, INVALID, 0, 0,
       "Enable AltiVec vector initializer syntax", 0)
OPTION("-fapple-kext", fapple_kext, Flag, INVALID, INVALID, 0, 0,
       "Use Apple's kernel extensions ABI", 0)
OPTION("-fauto-module-import", fauto_module_import, Flag, INVALID, INVALID, 0, 0,
       "Automatically translate #include/#import into module imports when possible", 0)
OPTION("-fblocks-runtime-optional", fblocks_runtime_optional, Flag, INVALID, INVALID, 0, 0,
       "Weakly link in the blocks runtime", 0)
OPTION("-fblocks", fblocks, Flag, INVALID, INVALID, 0, 0,
       "Enable the 'blocks' language feature", 0)
OPTION("-fborland-extensions", fborland_extensions, Flag, INVALID, INVALID, 0, 0,
       "Accept non-standard constructs supported by the Borland compiler", 0)
OPTION("-fcatch-undefined-behavior", fcatch_undefined_behavior, Flag, INVALID, INVALID, 0, 0,
       "Generate runtime checks for undefined behavior.", 0)
OPTION("-fcolor-diagnostics", fcolor_diagnostics, Flag, INVALID, INVALID, 0, 0,
       "Use colors in diagnostics", 0)
OPTION("-fconst-strings", fconst_strings, Flag, INVALID, INVALID, 0, 0,
       "Use a const qualified type for string literals in C and ObjC", 0)
OPTION("-fconstant-string-class", fconstant_string_class, Separate, INVALID, INVALID, 0, 0,
       "Specify the class to use for constant Objective-C string objects.", "<class name>")
OPTION("-fcuda-is-device", fcuda_is_device, Flag, INVALID, INVALID, 0, 0,
       "Generate code for CUDA device", 0)
OPTION("-fcxx-exceptions", fcxx_exceptions, Flag, INVALID, INVALID, 0, 0,
       "Enable C++ exceptions", 0)
OPTION("-fdata-sections", fdata_sections, Flag, INVALID, INVALID, 0, 0,
       "Place each data in its own section (ELF Only)", 0)
OPTION("-fdebugger-support", fdebugger_support, Flag, INVALID, INVALID, 0, 0,
       "Enable special debugger support behavior", 0)
OPTION("-fdelayed-template-parsing", fdelayed_template_parsing, Flag, INVALID, INVALID, 0, 0,
       "Parse templated function definitions at the end of the translation unit ", 0)
OPTION("-fdeprecated-macro", fdeprecated_macro, Flag, INVALID, INVALID, 0, 0,
       "Defines the __DEPRECATED macro", 0)
OPTION("-fdiagnostics-format", fdiagnostics_format, Separate, INVALID, INVALID, 0, 0,
       "Change diagnostic formatting to match IDE and command line tools", 0)
OPTION("-fdiagnostics-parseable-fixits", fdiagnostics_parseable_fixits, Flag, INVALID, INVALID, 0, 0,
       "Print fix-its in machine parseable form", 0)
OPTION("-fdiagnostics-print-source-range-info", fdiagnostics_print_source_range_info, Flag, INVALID, INVALID, 0, 0,
       "Print source range spans in numeric form", 0)
OPTION("-fdiagnostics-show-category", fdiagnostics_show_category, Separate, INVALID, INVALID, 0, 0,
       "Print diagnostic category", 0)
OPTION("-fdiagnostics-show-name", fdiagnostics_show_name, Flag, INVALID, INVALID, 0, 0,
       "Print diagnostic name", 0)
OPTION("-fdiagnostics-show-note-include-stack", fdiagnostics_show_note_include_stack, Flag, INVALID, INVALID, 0, 0,
       "Display include stacks for diagnostic notes", 0)
OPTION("-fdiagnostics-show-option", fdiagnostics_show_option, Flag, INVALID, INVALID, 0, 0,
       "Print option name with mappable diagnostics", 0)
OPTION("-fdisable-module-hash", fdisable_module_hash, Flag, INVALID, INVALID, 0, 0,
       "Disable the module hash", 0)
OPTION("-fdollars-in-identifiers", fdollars_in_identifiers, Flag, INVALID, INVALID, 0, 0,
       "Allow '$' in identifiers", 0)
OPTION("-fdump-record-layouts", fdump_record_layouts, Flag, INVALID, INVALID, 0, 0,
       "Dump record layout information", 0)
OPTION("-fdump-vtable-layouts", fdump_vtable_layouts, Flag, INVALID, INVALID, 0, 0,
       "Dump the layouts of all vtables that will be emitted in a translation unit", 0)
OPTION("-femit-all-decls", femit_all_decls, Flag, INVALID, INVALID, 0, 0,
       "Emit all declarations, even if unused", 0)
OPTION("-femit-coverage-data", femit_coverage_data, Flag, INVALID, INVALID, 0, 0,
       "Instrument the program to emit gcov coverage data when run.", 0)
OPTION("-femit-coverage-notes", femit_coverage_notes, Flag, INVALID, INVALID, 0, 0,
       "Emit a gcov coverage notes file when compiling.", 0)
OPTION("-ferror-limit", ferror_limit, Separate, INVALID, INVALID, 0, 0,
       "Set the maximum number of errors to emit before stopping (0 = no limit).", "<N>")
OPTION("-fexceptions", fexceptions, Flag, INVALID, INVALID, 0, 0,
       "Enable support for exception handling", 0)
OPTION("-ffake-address-space-map", ffake_address_space_map, Flag, INVALID, INVALID, 0, 0,
       "Use a fake address space map; OpenCL testing purposes only", 0)
OPTION("-fforbid-guard-variables", fforbid_guard_variables, Flag, INVALID, INVALID, 0, 0,
       "Emit an error if a C++ static local initializer would need a guard variable", 0)
OPTION("-ffreestanding", ffreestanding, Flag, INVALID, INVALID, 0, 0,
       "Assert that the compilation takes place in a freestanding environment", 0)
OPTION("-ffunction-sections", ffunction_sections, Flag, INVALID, INVALID, 0, 0,
       "Place each function in its own section (ELF Only)", 0)
OPTION("-fgnu-keywords", fgnu_keywords, Flag, INVALID, INVALID, 0, 0,
       "Allow GNU-extension keywords regardless of language standard", 0)
OPTION("-fgnu-runtime", fgnu_runtime, Flag, INVALID, INVALID, 0, 0,
       "Generate output compatible with the standard GNU Objective-C runtime", 0)
OPTION("-fgnu89-inline", fgnu89_inline, Flag, INVALID, INVALID, 0, 0,
       "Use the gnu89 inline semantics", 0)
OPTION("-fheinous-gnu-extensions", fheinous_gnu_extensions, Flag, INVALID, INVALID, 0, 0, 0, 0)
OPTION("-fhidden-weak-vtables", fhidden_weak_vtables, Flag, INVALID, INVALID, 0, 0,
       "Generate weak vtables and RTTI with hidden visibility", 0)
OPTION("-finstrument-functions", finstrument_functions, Flag, INVALID, INVALID, 0, 0,
       "Generate calls to instrument function entry and exit", 0)
OPTION("-fix-what-you-can", fix_what_you_can, Flag, INVALID, INVALID, 0, 0,
       "Apply fix-it advice even in the presence of unfixable errors", 0)
OPTION("-fixit=", fixit_EQ, Joined, Action_Group, INVALID, 0, 0,
       "Apply fix-it advice creating a file with the given suffix", 0)
OPTION("-fixit", fixit, Flag, Action_Group, INVALID, 0, 0,
       "Apply fix-it advice to the input source", 0)
OPTION("-flimit-debug-info", flimit_debug_info, Flag, INVALID, INVALID, 0, 0,
       "Limit debug information produced to reduce size of debug binary", 0)
OPTION("-fmacro-backtrace-limit", fmacro_backtrace_limit, Separate, INVALID, INVALID, 0, 0,
       "Set the maximum number of entries to print in a macro expansion backtrace (0 = no limit).", "<N>")
OPTION("-fmath-errno", fmath_errno, Flag, INVALID, INVALID, 0, 0,
       "Require math functions to indicate errors by setting errno", 0)
OPTION("-fmessage-length", fmessage_length, Separate, INVALID, INVALID, 0, 0,
       "Format message diagnostics so that they fit within N columns or fewer, when possible.", "<N>")
OPTION("-fmodule-cache-path", fmodule_cache_path, Separate, INVALID, INVALID, 0, 0,
       "Specify the module cache path", "<directory>")
OPTION("-fms-compatibility", fms_compatibility, Flag, INVALID, INVALID, 0, 0,
       "Enable Microsoft compatibility mode", 0)
OPTION("-fms-extensions", fms_extensions, Flag, INVALID, INVALID, 0, 0,
       "Accept some non-standard constructs supported by the Microsoft compiler", 0)
OPTION("-fmsc-version=", fmsc_version, Joined, INVALID, INVALID, 0, 0,
       "Version of the Microsoft C/C++ compiler to report in _MSC_VER (0 = don't define it (default))", 0)
OPTION("-fno-access-control", fno_access_control, Flag, INVALID, INVALID, 0, 0,
       "Disable C++ access control", 0)
OPTION("-fno-assume-sane-operator-new", fno_assume_sane_operator_new, Flag, INVALID, INVALID, 0, 0,
       "Don't assume that C++'s global operator new can't alias any pointer", 0)
OPTION("-fno-bitfield-type-align", fno_bitfield_type_align, Flag, INVALID, INVALID, 0, 0,
       "Ignore bit-field types when aligning structures", 0)
OPTION("-fno-builtin", fno_builtin, Flag, INVALID, INVALID, 0, 0,
       "Disable implicit builtin knowledge of functions", 0)
OPTION("-fno-caret-diagnostics", fno_caret_diagnostics, Flag, INVALID, INVALID, 0, 0,
       "Do not include source line and caret with diagnostics", 0)
OPTION("-fno-common", fno_common, Flag, INVALID, INVALID, 0, 0,
       "Compile common globals like normal definitions", 0)
OPTION("-fno-const-strings", fno_const_strings, Flag, INVALID, INVALID, 0, 0,
       "Don't use a const qualified type for string literals in C and ObjC", 0)
OPTION("-fno-constant-cfstrings", fno_constant_cfstrings, Flag, INVALID, INVALID, 0, 0,
       "Enable creation of CodeFoundation-type constant strings", 0)
OPTION("-fno-deprecated-macro", fno_deprecated_macro, Flag, INVALID, INVALID, 0, 0,
       "Undefines the __DEPRECATED macro", 0)
OPTION("-fno-diagnostics-fixit-info", fno_diagnostics_fixit_info, Flag, INVALID, INVALID, 0, 0,
       "Do not include fixit information in diagnostics", 0)
OPTION("-fno-diagnostics-show-note-include-stack", fno_diagnostics_show_note_include_stack, Flag, INVALID, INVALID, 0, 0,
       "Display include stacks for diagnostic notes", 0)
OPTION("-fno-dollars-in-identifiers", fno_dollars_in_identifiers, Flag, INVALID, INVALID, 0, 0,
       "Disallow '$' in identifiers", 0)
OPTION("-fno-dwarf2-cfi-asm", fno_dwarf2_cfi_asm, Flag, INVALID, INVALID, 0, 0,
       "Don't use the cfi directives", 0)
OPTION("-fno-elide-constructors", fno_elide_constructors, Flag, INVALID, INVALID, 0, 0,
       "Disable C++ copy constructor elision", 0)
OPTION("-fno-gnu-keywords", fno_gnu_keywords, Flag, INVALID, INVALID, 0, 0,
       "Disallow GNU-extension keywords regardless of language standard", 0)
OPTION("-fno-lax-vector-conversions", fno_lax_vector_conversions, Flag, INVALID, INVALID, 0, 0,
       "Disallow implicit conversions between vectors with a different number of elements or different element types", 0)
OPTION("-fno-merge-all-constants", fno_merge_all_constants, Flag, INVALID, INVALID, 0, 0,
       "Disallow merging of constants.", 0)
OPTION("-fno-objc-infer-related-result-type", fno_objc_infer_related_result_type, Flag, INVALID, INVALID, 0, 0,
       "do not infer Objective-C related result type based on method family", 0)
OPTION("-fno-operator-names", fno_operator_names, Flag, INVALID, INVALID, 0, 0,
       "Do not treat C++ operator name keywords as synonyms for operators", 0)
OPTION("-fno-rtti", fno_rtti, Flag, INVALID, INVALID, 0, 0,
       "Disable generation of rtti information", 0)
OPTION("-fno-show-column", fno_show_column, Flag, INVALID, INVALID, 0, 0,
       "Do not include column number on diagnostics", 0)
OPTION("-fno-show-source-location", fno_show_source_location, Flag, INVALID, INVALID, 0, 0,
       "Do not include source location information with diagnostics", 0)
OPTION("-fno-signed-char", fno_signed_char, Flag, INVALID, INVALID, 0, 0,
       "Char is unsigned", 0)
OPTION("-fno-spell-checking", fno_spell_checking, Flag, INVALID, INVALID, 0, 0,
       "Disable spell-checking", 0)
OPTION("-fno-threadsafe-statics", fno_threadsafe_statics, Flag, INVALID, INVALID, 0, 0,
       "Do not emit code to make initialization of local statics thread safe", 0)
OPTION("-fno-use-cxa-atexit", fno_use_cxa_atexit, Flag, INVALID, INVALID, 0, 0,
       "Don't use __cxa_atexit for calling destructors", 0)
OPTION("-fno-validate-pch", fno_validate_pch, Flag, INVALID, INVALID, 0, 0,
       "Disable validation of precompiled headers", 0)
OPTION("-fobjc-arc-cxxlib=", fobjc_arc_cxxlib_EQ, Joined, INVALID, INVALID, 0, 0,
       "Objective-C++ Automatic Reference Counting standard library kind", 0)
OPTION("-fobjc-arc-exceptions", fobjc_arc_exceptions, Flag, INVALID, INVALID, 0, 0,
       "Use EH-safe code when synthesizing retains and releases in -fobjc-arc", 0)
OPTION("-fobjc-arc", fobjc_arc, Flag, INVALID, INVALID, 0, 0,
       "Synthesize retain and release calls for Objective-C pointers", 0)
OPTION("-fobjc-default-synthesize-properties", fobjc_default_synthesize_properties, Flag, INVALID, INVALID, 0, 0,
       "enable the default synthesis of Objective-C properties", 0)
OPTION("-fobjc-dispatch-method=", fobjc_dispatch_method_EQ, Joined, INVALID, INVALID, 0, 0,
       "Objective-C dispatch method to use", 0)
OPTION("-fobjc-exceptions", fobjc_exceptions, Flag, INVALID, INVALID, 0, 0,
       "Enable Objective-C exceptions", 0)
OPTION("-fobjc-fragile-abi", fobjc_fragile_abi, Flag, INVALID, INVALID, 0, 0,
       "Use Objective-C's fragile ABI", 0)
OPTION("-fobjc-gc-only", fobjc_gc_only, Flag, INVALID, INVALID, 0, 0,
       "Use GC exclusively for Objective-C related memory management", 0)
OPTION("-fobjc-gc", fobjc_gc, Flag, INVALID, INVALID, 0, 0,
       "Enable Objective-C garbage collection", 0)
OPTION("-fobjc-runtime-has-arc", fobjc_runtime_has_arc, Flag, INVALID, INVALID, 0, 0,
       "The target Objective-C runtime provides ARC entrypoints", 0)
OPTION("-fobjc-runtime-has-terminate", fobjc_runtime_has_terminate, Flag, INVALID, INVALID, 0, 0,
       "The target Objective-C runtime provides an objc_terminate entrypoint", 0)
OPTION("-fobjc-runtime-has-weak", fobjc_runtime_has_weak, Flag, INVALID, INVALID, 0, 0,
       "The target Objective-C runtime supports ARC weak operations", 0)
OPTION("-fpack-struct", fpack_struct, Separate, INVALID, INVALID, 0, 0,
       "Specify the default maximum struct packing alignment", 0)
OPTION("-fpascal-strings", fpascal_strings, Flag, INVALID, INVALID, 0, 0,
       "Recognize and construct Pascal-style string literals", 0)
OPTION("-fshort-enums", fshort_enums, Flag, INVALID, INVALID, 0, 0,
       "Allocate to an enum type only as many bytes as it needs for the declared range of possible values", 0)
OPTION("-fshort-wchar", fshort_wchar, Flag, INVALID, INVALID, 0, 0,
       "Force wchar_t to be a short unsigned int", 0)
OPTION("-fshow-column", fshow_column, Flag, INVALID, INVALID, 0, 0,
       "Include column number on diagnostics", 0)
OPTION("-fshow-overloads=", fshow_overloads_EQ, Joined, INVALID, INVALID, 0, 0,
       "Which overload candidates to show when overload resolution fails: best|all; defaults to all", 0)
OPTION("-fsjlj-exceptions", fsjlj_exceptions, Flag, INVALID, INVALID, 0, 0,
       "Use SjLj style exceptions", 0)
OPTION("-fsyntax-only", fsyntax_only, Flag, Action_Group, INVALID, 0, 0,
       "Run parser and perform semantic analysis", 0)
OPTION("-ftabstop", ftabstop, Separate, INVALID, INVALID, 0, 0,
       "Set the tab stop distance.", "<N>")
OPTION("-ftemplate-backtrace-limit", ftemplate_backtrace_limit, Separate, INVALID, INVALID, 0, 0,
       "Set the maximum number of entries to print in a template instantiation backtrace (0 = no limit).", "<N>")
OPTION("-ftemplate-depth", ftemplate_depth, Separate, INVALID, INVALID, 0, 0,
       "Maximum depth of recursive template instantiation", 0)
OPTION("-ftime-report", ftime_report, Flag, INVALID, INVALID, 0, 0,
       "Print the amount of time each phase of compilation takes", 0)
OPTION("-ftrapv-handler", ftrapv_handler, Separate, INVALID, INVALID, 0, 0,
       "Specify the function to be called on overflow.", "<function name>")
OPTION("-ftrapv", ftrapv, Flag, INVALID, INVALID, 0, 0,
       "Trap on integer overflow", 0)
OPTION("-funknown-anytype", funknown_anytype, Flag, INVALID, INVALID, 0, 0,
       "Enable parser support for the __unknown_anytype type; for testing purposes only", 0)
OPTION("-funroll-loops", funroll_loops, Flag, INVALID, INVALID, 0, 0,
       "Turn on loop unroller", 0)
OPTION("-fuse-register-sized-bitfield-access", fuse_register_sized_bitfield_access, Flag, INVALID, INVALID, 0, 0,
       "Use register sized accesses to bit-fields, when possible.", 0)
OPTION("-fvisibility-inlines-hidden", fvisibility_inlines_hidden, Flag, INVALID, INVALID, 0, 0,
       "Give inline C++ member functions default visibility by default", 0)
OPTION("-fvisibility", fvisibility, Separate, INVALID, INVALID, 0, 0,
       "Default symbol visibility", 0)
OPTION("-fwrapv", fwrapv, Flag, INVALID, INVALID, 0, 0,
       "Treat signed integer overflow as two's complement", 0)
OPTION("-fwritable-strings", fwritable_strings, Flag, INVALID, INVALID, 0, 0,
       "Store string literals as writable data", 0)
OPTION("-g", g, Flag, INVALID, INVALID, 0, 0,
       "Generate source level debug information", 0)
OPTION("-header-include-file", header_include_file, Separate, INVALID, INVALID, 0, 0,
       "Filename (or -) to write header include output to", 0)
OPTION("-help", help, Flag, INVALID, INVALID, 0, 0,
       "Print this help text", 0)
OPTION("-idirafter", idirafter, JoinedOrSeparate, INVALID, INVALID, 0, 0,
       "Add directory to AFTER include search path", "<directory>")
OPTION("-imacros", imacros, JoinedOrSeparate, INVALID, INVALID, 0, 0,
       "Include macros from file before parsing", "<file>")
OPTION("-include-pch", include_pch, Separate, INVALID, INVALID, 0, 0,
       "Include precompiled header file", "<file>")
OPTION("-include-pth", include_pth, Separate, INVALID, INVALID, 0, 0,
       "Include file before parsing", "<file>")
OPTION("-include", include, JoinedOrSeparate, INVALID, INVALID, 0, 0,
       "Include file before parsing", "<file>")
OPTION("-index-header-map", index_header_map, Flag, INVALID, INVALID, 0, 0,
       "Make the next included directory (-I or -F) an indexer header map", 0)
OPTION("-init-only", init_only, Flag, Action_Group, INVALID, 0, 0,
       "Only execute frontend initialization", 0)
OPTION("-internal-externc-isystem", internal_externc_isystem, JoinedOrSeparate, INVALID, INVALID, 0, 0,
       "Add directory to the internal system include search path with implicit extern \"C\" semantics; these are assumed to not be user-provided and are used to model system and standard headers' paths.", "<directory>")
OPTION("-internal-isystem", internal_isystem, JoinedOrSeparate, INVALID, INVALID, 0, 0,
       "Add directory to the internal system include search path; these are assumed to not be user-provided and are used to model system and standard headers' paths.", "<directory>")
OPTION("-iprefix", iprefix, JoinedOrSeparate, INVALID, INVALID, 0, 0,
       "Set the -iwithprefix/-iwithprefixbefore prefix", "<prefix>")
OPTION("-iquote", iquote, JoinedOrSeparate, INVALID, INVALID, 0, 0,
       "Add directory to QUOTE include search path", "<directory>")
OPTION("-isysroot", isysroot, JoinedOrSeparate, INVALID, INVALID, 0, 0,
       "Set the system root directory (usually /)", "<dir>")
OPTION("-isystem", isystem, JoinedOrSeparate, INVALID, INVALID, 0, 0,
       "Add directory to SYSTEM include search path", "<directory>")
OPTION("-iwithprefixbefore", iwithprefixbefore, JoinedOrSeparate, INVALID, INVALID, 0, 0,
       "Set directory to include search path with prefix", "<dir>")
OPTION("-iwithprefix", iwithprefix, JoinedOrSeparate, INVALID, INVALID, 0, 0,
       "Set directory to SYSTEM include search path with prefix", "<dir>")
OPTION("-iwithsysroot", iwithsysroot, JoinedOrSeparate, INVALID, INVALID, 0, 0,
       "Add directory to SYSTEM include search path, absolute paths are relative to -isysroot", "<directory>")
OPTION("-load", load, Separate, INVALID, INVALID, 0, 0,
       "Load the named plugin (dynamic shared object)", "<dsopath>")
OPTION("-main-file-name", main_file_name, Separate, INVALID, INVALID, 0, 0,
       "Main file name to use for debug info", 0)
OPTION("-masm-verbose", masm_verbose, Flag, INVALID, INVALID, 0, 0,
       "Generate verbose assembly output", 0)
OPTION("-mcode-model", mcode_model, Separate, INVALID, INVALID, 0, 0,
       "The code model to use", 0)
OPTION("-mconstructor-aliases", mconstructor_aliases, Flag, INVALID, INVALID, 0, 0,
       "Emit complete constructors and destructors as aliases when possible", 0)
OPTION("-mdebug-pass", mdebug_pass, Separate, INVALID, INVALID, 0, 0,
       "Enable additional debug output", 0)
OPTION("-mdisable-fp-elim", mdisable_fp_elim, Flag, INVALID, INVALID, 0, 0,
       "Disable frame pointer elimination optimization", 0)
OPTION("-mfloat-abi", mfloat_abi, Separate, INVALID, INVALID, 0, 0,
       "The float ABI to use", 0)
OPTION("-mlimit-float-precision", mlimit_float_precision, Separate, INVALID, INVALID, 0, 0,
       "Limit float precision to the given value", 0)
OPTION("-mllvm", mllvm, Separate, INVALID, INVALID, 0, 0,
       "Additional arguments to forward to LLVM's option processing", 0)
OPTION("-mms-bitfields", mms_bitfields, Flag, INVALID, INVALID, 0, 0,
       "Set the default structure layout to be compatible with the Microsoft compiler standard.", 0)
OPTION("-mno-global-merge", mno_global_merge, Flag, INVALID, INVALID, 0, 0,
       "Disable merging of globals", 0)
OPTION("-mno-zero-initialized-in-bss", mno_zero_initialized_in_bss, Flag, INVALID, INVALID, 0, 0,
       "Do not put zero initialized data in the BSS", 0)
OPTION("-mnoexecstack", mno_exec_stack, Flag, INVALID, INVALID, 0, 0,
       "Mark the file as not needing an executable stack", 0)
OPTION("-momit-leaf-frame-pointer", momit_leaf_frame_pointer, Flag, INVALID, INVALID, 0, 0,
       "Omit frame pointer setup for leaf functions.", 0)
OPTION("-mregparm", mregparm, Separate, INVALID, INVALID, 0, 0,
       "Limit the number of registers available for integer arguments", 0)
OPTION("-mrelax-all", mrelax_all, Flag, INVALID, INVALID, 0, 0,
       "(integrated-as) Relax all machine instructions", 0)
OPTION("-mrelocation-model", mrelocation_model, Separate, INVALID, INVALID, 0, 0,
       "The relocation model to use", 0)
OPTION("-mrtd", mrtd, Flag, INVALID, INVALID, 0, 0,
       "Make StdCall calling convention the default", 0)
OPTION("-msave-temp-labels", msave_temp_labels, Flag, INVALID, INVALID, 0, 0,
       "(integrated-as) Save temporary labels", 0)
OPTION("-msoft-float", msoft_float, Flag, INVALID, INVALID, 0, 0,
       "Use software floating point", 0)
OPTION("-mstackrealign", mstackrealign, Flag, INVALID, INVALID, 0, 0,
       "Force realign the stack at entry to every function.", 0)
OPTION("-munwind-tables", munwind_tables, Flag, INVALID, INVALID, 0, 0,
       "Generate unwinding tables for all functions", 0)
OPTION("-no-code-completion-globals", no_code_completion_globals, Flag, INVALID, INVALID, 0, 0,
       "Do not include global declarations in code-completion results.", 0)
OPTION("-no-implicit-float", no_implicit_float, Flag, INVALID, INVALID, 0, 0,
       "Don't generate implicit floating point instructions (x86-only)", 0)
OPTION("-nobuiltininc", nobuiltininc, Flag, INVALID, INVALID, 0, 0,
       "Disable builtin #include directories", 0)
OPTION("-nostdinc++", nostdincxx, Flag, INVALID, INVALID, 0, 0,
       "Disable standard #include directories for the C++ standard library", 0)
OPTION("-nostdsysteminc", nostdsysteminc, Flag, INVALID, INVALID, 0, 0,
       "Disable standard system #include directories", 0)
OPTION("-objc-isystem", objc_isystem, JoinedOrSeparate, INVALID, INVALID, 0, 0,
       "Add directory to the ObjC SYSTEM include search path", "<directory>")
OPTION("-objcxx-isystem", objcxx_isystem, JoinedOrSeparate, INVALID, INVALID, 0, 0,
       "Add directory to the ObjC++ SYSTEM include search path", "<directory>")
OPTION("-o", o, Separate, INVALID, INVALID, 0, 0,
       "Specify output file", "<path>")
OPTION("-pedantic-errors", pedantic_errors, Flag, INVALID, INVALID, 0, 0, 0, 0)
OPTION("-pedantic", pedantic, Flag, INVALID, INVALID, 0, 0, 0, 0)
OPTION("-pg", pg, Flag, INVALID, INVALID, 0, 0,
       "Enable mcount instrumentation", 0)
OPTION("-pic-level", pic_level, Separate, INVALID, INVALID, 0, 0,
       "Value for __PIC__", 0)
OPTION("-plugin-arg-", plugin_arg, JoinedAndSeparate, INVALID, INVALID, 0, 0,
       "Pass <arg> to plugin <name>", "<name> <arg>")
OPTION("-plugin", plugin, Separate, INVALID, INVALID, 0, 0,
       "Use the named plugin action instead of the default action (use \"help\" to list available options)", "<name>")
OPTION("-preamble-bytes=", preamble_bytes_EQ, Joined, INVALID, INVALID, 0, 0,
       "Assume that the precompiled header is a precompiled preamble covering the first N bytes of the main file", 0)
OPTION("-print-decl-contexts", print_decl_contexts, Flag, Action_Group, INVALID, 0, 0,
       "Print DeclContexts and their Decls", 0)
OPTION("-print-ivar-layout", print_ivar_layout, Flag, INVALID, INVALID, 0, 0,
       "Enable Objective-C Ivar layout bitmap print trace", 0)
OPTION("-print-preamble", print_preamble, Flag, Action_Group, INVALID, 0, 0,
       "Print the \"preamble\" of a file, which is a candidate for implicit precompiled headers.", 0)
OPTION("-print-stats", print_stats, Flag, INVALID, INVALID, 0, 0,
       "Print performance metrics and statistics", 0)
OPTION("-pthread", pthread, Flag, INVALID, INVALID, 0, 0,
       "Support POSIX threads in generated code", 0)
OPTION("-relaxed-aliasing", relaxed_aliasing, Flag, INVALID, INVALID, 0, 0,
       "Turn off Type Based Alias Analysis", 0)
OPTION("-relocatable-pch", relocatable_pch, Flag, INVALID, INVALID, 0, 0,
       "Whether to build a relocatable precompiled header", 0)
OPTION("-remap-file", remap_file, Separate, INVALID, INVALID, 0, 0,
       "Replace the contents of the <from> file with the contents of the <to> file", "<from>;<to>")
OPTION("-resource-dir", resource_dir, Separate, INVALID, INVALID, 0, 0,
       "The directory which holds the compiler resource files", 0)
OPTION("-rewrite-macros", rewrite_macros, Flag, Action_Group, INVALID, 0, 0,
       "Expand macros without full preprocessing", 0)
OPTION("-rewrite-objc", rewrite_objc, Flag, Action_Group, INVALID, 0, 0,
       "Rewrite ObjC into C (code rewriter example)", 0)
OPTION("-rewrite-test", rewrite_test, Flag, Action_Group, INVALID, 0, 0,
       "Rewriter playground", 0)
OPTION("-stack-protector", stack_protector, Separate, INVALID, INVALID, 0, 0,
       "Enable stack protectors", 0)
OPTION("-static-define", static_define, Flag, INVALID, INVALID, 0, 0,
       "Should __STATIC__ be defined", 0)
OPTION("-std=", std_EQ, Joined, INVALID, INVALID, 0, 0,
       "Language standard to compile for", 0)
OPTION("-stdlib=", stdlib_EQ, Joined, INVALID, INVALID, 0, 0,
       "C++ standard library to use", 0)
OPTION("-sys-header-deps", sys_header_deps, Flag, INVALID, INVALID, 0, 0,
       "Include system headers in dependency output", 0)
OPTION("-target-abi", target_abi, Separate, INVALID, INVALID, 0, 0,
       "Target a particular ABI type", 0)
OPTION("-target-cpu", target_cpu, Separate, INVALID, INVALID, 0, 0,
       "Target a specific cpu type", 0)
OPTION("-target-feature", target_feature, Separate, INVALID, INVALID, 0, 0,
       "Target specific attributes", 0)
OPTION("-target-linker-version", target_linker_version, Separate, INVALID, INVALID, 0, 0,
       "Target linker version", 0)
OPTION("-token-cache", token_cache, Separate, INVALID, INVALID, 0, 0,
       "Use specified token cache file", "<path>")
OPTION("-traditional-cpp", traditional_cpp, Flag, INVALID, INVALID, 0, 0,
       "Enable some traditional CPP emulation", 0)
OPTION("-trigraphs", trigraphs, Flag, INVALID, INVALID, 0, 0,
       "Process trigraph sequences", 0)
OPTION("-trim-egraph", trim_egraph, Flag, INVALID, INVALID, 0, 0,
       "Only show error-related paths in the analysis graph", 0)
OPTION("-triple=", triple_EQ, Joined, INVALID, triple, 0, 0, 0, 0)
OPTION("-triple", triple, Separate, INVALID, INVALID, 0, 0,
       "Specify target triple (e.g. i686-apple-darwin9)", 0)
OPTION("-undef", undef, Flag, INVALID, INVALID, 0, 0,
       "undef all system defines", "<macro>")
OPTION("-unoptimized-cfg", analysis_UnoptimizedCFG, Flag, INVALID, INVALID, 0, 0,
       "Generate unoptimized CFGs for all analyses", 0)
OPTION("-verify", verify, Flag, INVALID, INVALID, 0, 0,
       "Verify emitted diagnostics and warnings", 0)
OPTION("-version", version, Flag, INVALID, INVALID, 0, 0,
       "Print the compiler version", 0)
OPTION("-v", v, Flag, INVALID, INVALID, 0, 0,
       "Enable verbose output", 0)
OPTION("-working-directory=", working_directory_EQ, Joined, INVALID, working_directory, 0, 0, 0, 0)
OPTION("-working-directory", working_directory, JoinedOrSeparate, INVALID, INVALID, 0, 0,
       "Resolve file paths relative to the specified directory", 0)
OPTION("-w", w, Flag, INVALID, INVALID, 0, 0,
       "Suppress all warnings", 0)
OPTION("-x", x, Separate, INVALID, INVALID, 0, 0,
       "Input language type", 0)
