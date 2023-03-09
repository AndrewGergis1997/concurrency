#ifndef TUNI_HH
#define TUNI_HH

// GCC/g++ with C++ -std=20 gives warnings
// in linux-desktop.tuni.fi from Qt6 header files.
// Since we cannot do anything about it, the
// warnings are suppressed with these macros.

#ifdef __GNUC__
#define TUNI_WARN_OFF() \
    _Pragma("GCC diagnostic push") \
    _Pragma("GCC diagnostic ignored \"-Wtautological-compare\"")
#define TUNI_WARN_ON() \
    _Pragma("GCC diagnostic pop")
#else
#define TUNI_WARN_OFF()
#define TUNI_WARN_ON()
#endif


#endif // TUNI_HH
