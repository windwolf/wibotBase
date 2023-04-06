//
// Created by zhouj on 2023/3/31.
//

#include "CircularBuffer.test.hpp"

#include "minunit.h"

static void test1() {
    uint8_t buf[8];
    uint8_t ref[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    uint8_t rst[8];
    uint32_t l;
    wibot::CircularBuffer<uint8_t> cb(buf, 8);
    cb.write(ref, 1);

    l = cb.peek(rst);
    MU_ASSERT_EQUALS(l, 1);
    MU_ASSERT_VEC_EQUALS(rst, ref, 1);

    cb.write(ref + 1, 2);

    l = cb.peek(rst, 0, 3);
    MU_ASSERT_EQUALS(l, 3);
    MU_ASSERT_VEC_EQUALS(rst, ref, 3);

    cb.write(ref + 3, 3);

    l = cb.peek(rst, 0, 6);
    MU_ASSERT_EQUALS(l, 6);
    MU_ASSERT_VEC_EQUALS(rst, ref, 6);

    l = cb.peek(rst, 1, 5);
    MU_ASSERT_EQUALS(l, 5);
    MU_ASSERT_VEC_EQUALS(rst, ref + 1, 5);

    l = cb.peek(rst, 1, 6);
    MU_ASSERT_EQUALS(l, 5);
    MU_ASSERT_VEC_EQUALS(rst, ref + 1, 5);

    cb.write(ref + 6, 4);
    l = cb.peek(rst, 0, 1);
    MU_ASSERT_EQUALS(l, 1);
    MU_ASSERT_VEC_EQUALS(rst, ref + 2, 1);
}

static void test2() {
    uint8_t buf[8];
    uint8_t ref[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    uint8_t rst[8];
    uint32_t l;
    wibot::CircularBuffer<uint8_t> cb(buf, 8);

    for (int j = 1; j < 8; ++j) {
        for (int i = 0; i < 20; ++i) {
            cb.write(ref, j);
            MU_ASSERT_EQUALS(cb.getSize(), j);
            l = cb.read(rst, j);
            MU_ASSERT_EQUALS(l, j);
            MU_ASSERT_EQUALS(cb.getSize(), 0);
            MU_ASSERT_VEC_EQUALS(rst, ref, j);
        }
    }

    for (int i = 0; i < 20; ++i) {
        cb.write(ref, 7);
        l = cb.read(rst, 7);
        MU_ASSERT_EQUALS(l, 7);
        MU_ASSERT_VEC_EQUALS(rst, ref, 7);
    }

    cb.write(ref, 7);
    MU_ASSERT_EQUALS(cb.getSize(), 7);
    l = cb.read(rst, 6);
    MU_ASSERT_EQUALS(l, 6);
    MU_ASSERT_EQUALS(cb.getSize(), 1);
    MU_ASSERT_EQUALS(rst[0], 0);

    cb.write(ref, 7);
    MU_ASSERT_EQUALS(cb.getSize(), 8);
    l = cb.read(rst, 6);
    MU_ASSERT_EQUALS(l, 6);
    MU_ASSERT_EQUALS(cb.getSize(), 2);
    MU_ASSERT_EQUALS(rst[0], 6);

    for (int i = 0; i < 20; ++i) {
        cb.write(ref, 7);
        l = cb.read(rst, 6);
        MU_ASSERT_EQUALS(l, 6);
        MU_ASSERT_EQUALS(cb.getSize(), 2);
        MU_ASSERT_EQUALS(rst[0], 6);
    }
}

void wibot::base::test::circularBufferTest() {
    LOG_D("---circularTest begin---");
    LOG_D("---test1---");
    test1();
    LOG_D("---test2---");
    test2();
    LOG_D("---circularTest end---");
};
