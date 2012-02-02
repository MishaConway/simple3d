#ifdef _WIN32
#include <functional>
#define portable_function std::function
#endif

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR || TARGET_OS_IPAD || TARGET_OS_IPAD_SIMULATOR
#include <tr1/functional>
#define portable_function std::tr1::function
#endif