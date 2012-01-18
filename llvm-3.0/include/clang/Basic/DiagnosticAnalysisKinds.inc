#ifdef ANALYSISSTART
__ANALYSISSTART = DIAG_START_ANALYSIS,
#undef ANALYSISSTART
#endif

DIAG(warn_uninit_val, CLASS_WARNING, diag::MAP_WARNING, "use of uninitialized variable", "", true, false, false, false, 0, "", "")
