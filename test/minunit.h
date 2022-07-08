#ifndef ___MINIUNIT_H__
#define ___MINIUNIT_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdio.h"

/* file: minunit.h */
#define MU_ASSERT(test)               \
    do                                \
    {                                 \
        if (!(test))                  \
            printf("TEST: "           \
                   "(" __FILE__ ":%d" \
                   ":" #test ")\r\n", \
                   __LINE__);         \
    } while (0)

#define MU_RUN(test)                  \
    do                                \
    {                                 \
        if (!test())                  \
            printf("TEST: "           \
                   "(" __FILE__ ":%d" \
                   ":" #test ")\r\n", \
                   __LINE__);         \
    } while (0)

#define MU_ASSERT_EQUALS(a, b)             \
    do                                     \
    {                                      \
        if (a != b)                        \
            printf("TEST: "                \
                   "(" __FILE__ ":%d"      \
                   ":" #a "==" #b ")\r\n", \
                   __LINE__);              \
    } while (0)

#define MU_ASSERT_VEC_EQUALS(a, b, size)               \
    do                                                 \
    {                                                  \
        for (uint32_t i = 0; i < size; i++)            \
        {                                              \
            if (a[i] != b[i])                          \
                printf("TEST: "                        \
                       "(" __FILE__ ":%d"              \
                       ":" #a "[%d]==" #b "[%d])\r\n", \
                       __LINE__, i, i);                \
        }                                              \
    } while (0)

#define MU_VEC_CLEAR(vec, size)             \
    do                                      \
    {                                       \
        for (uint32_t i = 0; i < size; i++) \
        {                                   \
            vec[i] = 0;                     \
        }                                   \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif // ___MINIUNIT_H__