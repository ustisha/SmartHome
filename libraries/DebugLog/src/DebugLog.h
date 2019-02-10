#ifdef SERIAL_DEBUG
#define IF_SERIAL_DEBUG(x) ({x;})
#else
#define IF_SERIAL_DEBUG(x)
#if defined(RF24_TINY)
#define printf_P(...)
#endif
#endif
