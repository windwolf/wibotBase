//
// Created by zhouj on 2022/12/10.
//

#include "DataSource.hpp"

namespace wibot
{
    uint32_t MemoryDataSource::get_data() {
        return *_data;
    }
    uint32_t DummyDataSource::get_data() {
        return 0;
    }
} // wibot
