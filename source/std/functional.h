#ifdef _WIN32
#include <functional>
#endif

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR || TARGET_OS_IPAD || TARGET_OS_IPAD_SIMULATOR
#include <tr1/functional>
#endif