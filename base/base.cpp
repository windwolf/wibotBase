#include "base.hpp"

namespace ww
{
char *strtrim(char *str, char delimit)
{
    while (*str != 0x00)
    {
        if (*str == delimit)
        {
            str++;
            continue;
        }
        else
        {
            return str;
        }
    }

    return str;
};

uint32_t fast_log2(uint32_t _val)
{
    uint32_t ret;
    float fdata = (float)_val;
    // unsigned int uData = (fdata>>23)&0xFF;
    // //ç´ćĽç§ťĺ¨ĺşé
    uint32_t data = *(uint32_t *)&fdata; //(unsigned int&)fdata ä¸(unsigned
                                         // int*)&fdata; ä¸č
    unsigned int udata = (data >> 23) & 0xFF;
    ret = (int)udata - 127; //-ć çŹŚĺˇĺ°ćçŹŚĺ

    return ret;
}

// Result Initializable::init()
// {
//     if (_initialized)
//     {
//         return Result_OK;
//     }
//     auto rst = _init();
//     if (rst == Result_OK)
//     {
//         _initialized = true;
//     }
//     return rst;
// };
// void Initializable::deinit()
// {
//     if (_initialized)
//     {
//         _deinit();
//         _initialized = false;
//     }
// };

}; // namespace ww