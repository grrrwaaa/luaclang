#ifdef ASTSTART
__ASTSTART = DIAG_START_AST,
#undef ASTSTART
#endif

DIAG(err_asm_empty_symbolic_operand_name, CLASS_ERROR, diag::MAP_ERROR, "empty symbolic operand name in inline assembly string", "", true, false, false, false, 5, "", "")
DIAG(err_asm_invalid_escape, CLASS_ERROR, diag::MAP_ERROR, "invalid %% escape in inline assembly string", "", true, false, false, false, 5, "", "")
DIAG(err_asm_invalid_operand_number, CLASS_ERROR, diag::MAP_ERROR, "invalid operand number in inline asm string", "", true, false, false, false, 5, "", "")
DIAG(err_asm_unknown_symbolic_operand_name, CLASS_ERROR, diag::MAP_ERROR, "unknown symbolic operand name in inline assembly string", "", true, false, false, false, 5, "", "")
DIAG(err_asm_unterminated_symbolic_operand_name, CLASS_ERROR, diag::MAP_ERROR, "unterminated symbolic operand name in inline assembly string", "", true, false, false, false, 5, "", "")
DIAG(err_odr_different_num_template_parameters, CLASS_ERROR, diag::MAP_ERROR, "template parameter lists have a different number of parameters (%0 vs %1)", "", true, false, false, false, 0, "", "")
DIAG(err_odr_different_template_parameter_kind, CLASS_ERROR, diag::MAP_ERROR, "template parameter has different kinds in different translation units", "", true, false, false, false, 0, "", "")
DIAG(err_odr_field_type_inconsistent, CLASS_ERROR, diag::MAP_ERROR, "field %0 declared with incompatible types in different translation units (%1 vs. %2)", "", true, false, false, false, 0, "", "")
DIAG(err_odr_function_type_inconsistent, CLASS_ERROR, diag::MAP_ERROR, "external function %0 declared with incompatible types in different translation units (%1 vs. %2)", "", true, false, false, false, 0, "", "")
DIAG(err_odr_ivar_type_inconsistent, CLASS_ERROR, diag::MAP_ERROR, "instance variable %0 declared with incompatible types in different translation units (%1 vs. %2)", "", true, false, false, false, 0, "", "")
DIAG(err_odr_non_type_parameter_type_inconsistent, CLASS_ERROR, diag::MAP_ERROR, "non-type template parameter declared with incompatible types in different translation units (%0 vs. %1)", "", true, false, false, false, 0, "", "")
DIAG(err_odr_objc_method_num_params_inconsistent, CLASS_ERROR, diag::MAP_ERROR, "%select{class|instance}0 method %1 has a different number of parameters in different translation units (%2 vs. %3)", "", true, false, false, false, 0, "", "")
DIAG(err_odr_objc_method_param_type_inconsistent, CLASS_ERROR, diag::MAP_ERROR, "%select{class|instance}0 method %1 has a parameter with a different types in different translation units (%2 vs. %3)", "", true, false, false, false, 0, "", "")
DIAG(err_odr_objc_method_result_type_inconsistent, CLASS_ERROR, diag::MAP_ERROR, "%select{class|instance}0 method %1 has incompatible result types in different translation units (%2 vs. %3)", "", true, false, false, false, 0, "", "")
DIAG(err_odr_objc_method_variadic_inconsistent, CLASS_ERROR, diag::MAP_ERROR, "%select{class|instance}0 method %1 is variadic in one translation unit and not variadic in another", "", true, false, false, false, 0, "", "")
DIAG(err_odr_objc_property_impl_kind_inconsistent, CLASS_ERROR, diag::MAP_ERROR, "property %0 is implemented with %select{@synthesize|@dynamic}1 in one translation but %select{@dynamic|@synthesize}1 in another translation unit", "", true, false, false, false, 0, "", "")
DIAG(err_odr_objc_property_type_inconsistent, CLASS_ERROR, diag::MAP_ERROR, "property %0 declared with incompatible types in different translation units (%1 vs. %2)", "", true, false, false, false, 0, "", "")
DIAG(err_odr_objc_superclass_inconsistent, CLASS_ERROR, diag::MAP_ERROR, "class %0 has incompatible superclasses", "", true, false, false, false, 0, "", "")
DIAG(err_odr_objc_synthesize_ivar_inconsistent, CLASS_ERROR, diag::MAP_ERROR, "property %0 is synthesized to different ivars in different translation units (%1 vs. %2)", "", true, false, false, false, 0, "", "")
DIAG(err_odr_parameter_pack_non_pack, CLASS_ERROR, diag::MAP_ERROR, "parameter kind mismatch; parameter is %select{not a|a}0 parameter pack", "", true, false, false, false, 0, "", "")
DIAG(err_odr_variable_multiple_def, CLASS_ERROR, diag::MAP_ERROR, "external variable %0 defined in multiple translation units", "", true, false, false, false, 0, "", "")
DIAG(err_odr_variable_type_inconsistent, CLASS_ERROR, diag::MAP_ERROR, "external variable %0 declared with incompatible types in different translation units (%1 vs. %2)", "", true, false, false, false, 0, "", "")
DIAG(err_unsupported_ast_node, CLASS_ERROR, diag::MAP_ERROR, "cannot import unsupported AST node %0", "", true, false, false, false, 0, "", "")
DIAG(note_expr_divide_by_zero, CLASS_NOTE, diag::MAP_FATAL, "division by zero", "", true, false, false, false, 0, "", "")
DIAG(note_odr_base, CLASS_NOTE, diag::MAP_FATAL, "class has base type %0", "", true, false, false, false, 0, "", "")
DIAG(note_odr_bit_field, CLASS_NOTE, diag::MAP_FATAL, "bit-field %0 with type %1 and length %2 here", "", true, false, false, false, 0, "", "")
DIAG(note_odr_defined_here, CLASS_NOTE, diag::MAP_FATAL, "also defined here", "", true, false, false, false, 0, "", "")
DIAG(note_odr_enumerator, CLASS_NOTE, diag::MAP_FATAL, "enumerator %0 with value %1 here", "", true, false, false, false, 0, "", "")
DIAG(note_odr_field, CLASS_NOTE, diag::MAP_FATAL, "field %0 has type %1 here", "", true, false, false, false, 0, "", "")
DIAG(note_odr_missing_base, CLASS_NOTE, diag::MAP_FATAL, "no corresponding base class here", "", true, false, false, false, 0, "", "")
DIAG(note_odr_missing_enumerator, CLASS_NOTE, diag::MAP_FATAL, "no corresponding enumerator here", "", true, false, false, false, 0, "", "")
DIAG(note_odr_missing_field, CLASS_NOTE, diag::MAP_FATAL, "no corresponding field here", "", true, false, false, false, 0, "", "")
DIAG(note_odr_not_bit_field, CLASS_NOTE, diag::MAP_FATAL, "field %0 is not a bit-field", "", true, false, false, false, 0, "", "")
DIAG(note_odr_number_of_bases, CLASS_NOTE, diag::MAP_FATAL, "class has %0 base %plural{1:class|:classes}0", "", true, false, false, false, 0, "", "")
DIAG(note_odr_objc_method_here, CLASS_NOTE, diag::MAP_FATAL, "%select{class|instance}0 method %1 also declared here", "", true, false, false, false, 0, "", "")
DIAG(note_odr_objc_missing_superclass, CLASS_NOTE, diag::MAP_FATAL, "no corresponding superclass here", "", true, false, false, false, 0, "", "")
DIAG(note_odr_objc_property_impl_kind, CLASS_NOTE, diag::MAP_FATAL, "property %0 is implemented with %select{@synthesize|@dynamic}1 here", "", true, false, false, false, 0, "", "")
DIAG(note_odr_objc_superclass, CLASS_NOTE, diag::MAP_FATAL, "inherits from superclass %0 here", "", true, false, false, false, 0, "", "")
DIAG(note_odr_objc_synthesize_ivar_here, CLASS_NOTE, diag::MAP_FATAL, "property is synthesized to ivar %0 here", "", true, false, false, false, 0, "", "")
DIAG(note_odr_parameter_pack_non_pack, CLASS_NOTE, diag::MAP_FATAL, "%select{parameter|parameter pack}0 declared here", "", true, false, false, false, 0, "", "")
DIAG(note_odr_tag_kind_here, CLASS_NOTE, diag::MAP_FATAL, "%0 is a %select{struct|union|class|enum}1 here", "", true, false, false, false, 0, "", "")
DIAG(note_odr_template_parameter_here, CLASS_NOTE, diag::MAP_FATAL, "template parameter declared here", "", true, false, false, false, 0, "", "")
DIAG(note_odr_template_parameter_list, CLASS_NOTE, diag::MAP_FATAL, "template parameter list also declared here", "", true, false, false, false, 0, "", "")
DIAG(note_odr_value_here, CLASS_NOTE, diag::MAP_FATAL, "declared here with type %0", "", true, false, false, false, 0, "", "")
DIAG(note_odr_virtual_base, CLASS_NOTE, diag::MAP_FATAL, "%select{non-virtual|virtual}0 derivation here", "", true, false, false, false, 0, "", "")
DIAG(warn_odr_tag_type_inconsistent, CLASS_WARNING, diag::MAP_WARNING, "type %0 has incompatible definitions in different translation units", "", true, false, false, false, 0, "", "")
