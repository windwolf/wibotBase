#ifndef ___SAFE_PRINTF_H__
#define ___SAFE_PRINTF_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define safe_printf(fmt, ...)       \
    do                              \
    {                               \
        _begin_safe_printf();       \
        printf(fmt, ##__VA_ARGS__); \
        _end_safe_printf();         \
    } while (0)

    void safe_printf_init();

    void _begin_safe_printf();

    void _end_safe_printf();

#ifdef __cplusplus
}
#endif

#endif // ___SAFE_PRINTF_H__