// some regulations:

// [almost] all standart types should be overriden by own type definitions, for instance - UINT32, INT64, POINTER

// no standart types in code as possible
// no platform-specific macros and definitions in code outside section "platform" (/core/platform, this section)
// use own platform definitions for further platform selection and conditional compilation in code

#define X86_PROCESSOR_MODEL
//#define ITANIUM_PROCESSOR_MODEL
//#define X86_64_PROCESSOR_MODEL

#ifdef WIN32
#define WINDOWS_THREADING_MODEL
#else
#define POSIX_THREADING_MODEL
#endif

// no #define macros as possible

// all export types should be pre-defined at the top of there headers
// all local variables should be defined at the functions` begining, before any action

// variable names are simple nouns, one word as possible

// SMTH smth - static variable of type SMTH
// PSMTH [p]smth - pointer to the variable of type SMTH
// SMTH * smth - pointer to dynamic array of items of type SMTH

// any more rules?..