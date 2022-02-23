#include "../inc/console_test.h"
#include "minunit.h"
#include <stdlib.h>
#include "../inc/console.h"
#include <string.h>

#ifdef UNITTEST

static bool test_cfg_raw_getter(const void *context, const int32_t index, void **out);
static bool test_cfg_value_getter(const void *context, const int32_t index, char *out);
static bool test_cfg_value_setter(const void *context, const int32_t index, char *in);
static TreeAccessor ta;
static TreeAccessorItemEntry root = {.name = "/"};
static TreeAccessorItemEntry aa = {.name = "aa"};
static TreeAccessorItemEntry bbs = {.name = "bbs", .rawGetter = test_cfg_raw_getter};
static TreeAccessorItemEntry bf1 = {
    .name = "bf1",
    .getter = test_cfg_value_getter,
    .setter = test_cfg_value_setter};
static TreeAccessorItemEntry aa_a1 = {.name = "a1"};
static TreeAccessorItemEntry aa_a2 = {.name = "a2"};
static TreeAccessorItemEntry aa_a1_f1 = {.name = "f1", .getter = test_cfg_value_getter};
static TreeAccessorItemEntry aa_a1_f2 = {.name = "f2", .getter = test_cfg_value_getter};

static uint8_t buffer[1024];
static RingBuffer rb;
static Console cons;

static void setup();

static void get_test1();
static void get_test2();
static void get_test3();

static void set_test1();
static void set_test2();
static void set_test3();

static void cd_test1();
static void cd_test2();
static void cd_test3();

static void li_test1();
static void li_test2();
static void li_test3();

typedef struct
{
    int f1;
} test_data;

static test_data test_cfg_data[2];

static bool test_cfg_raw_getter(const void *context, const int32_t index, void **out)
{
    *out = &test_cfg_data[index];
    return true;
}

static bool test_cfg_value_getter(const void *context, const int32_t index, char *out)
{
    sprintf(out, "%d", ((test_data *)context)->f1);
    return true;
}

static bool test_cfg_value_setter(const void *context, const int32_t index, char *in)
{
    int value;
    int rst;
    rst = sscanf(in, "%d", &value);
    if (rst != 1)
    {
        return false;
    }
    ((test_data *)context)->f1 = value;
    return true;
}

void console_test()
{
    setup();

    cd_test1();
    set_test1();
    get_test1();
    li_test1();

    cd_test2();
    set_test2();
    get_test2();

    cd_test3();
    set_test3();
    get_test3();
};

static void setup()
{
    bool rst;
    rst = tree_accessor_item_register(&ta, "", &root);
    rst = tree_accessor_item_register(&ta, "/", &aa);
    rst = tree_accessor_item_register(&ta, "/", &bbs);
    rst = tree_accessor_item_register(&ta, "/bbs", &bf1);
    rst = tree_accessor_item_register(&ta, "/aa", &aa_a1);
    rst = tree_accessor_item_register(&ta, "/aa", &aa_a2);
    rst = tree_accessor_item_register(&ta, "/aa/a1", &aa_a1_f1);
    rst = tree_accessor_item_register(&ta, "/aa/a1", &aa_a1_f2);
};

static void cd_test1()
{
    bool rst;
    MU_ASSERT(ta.context.currentContextNodeIndex == 0);
    MU_ASSERT(strcmp(tree_accessor_context_path_get(&ta), "/") == 0);

    rst = tree_accessor_context_change(&ta, "/aa");
    MU_ASSERT(rst);
    MU_ASSERT(ta.context.currentContextNodeIndex == 1);
    MU_ASSERT(strcmp(tree_accessor_context_path_get(&ta), "/aa") == 0);

    rst = tree_accessor_context_change(&ta, "..");
    MU_ASSERT(rst);
    MU_ASSERT(ta.context.currentContextNodeIndex == 0);
    MU_ASSERT(strcmp(tree_accessor_context_path_get(&ta), "/") == 0);

    rst = tree_accessor_context_change(&ta, "/aa/a1");
    MU_ASSERT(rst);
    MU_ASSERT(ta.context.currentContextNodeIndex == 2);
    MU_ASSERT(strcmp(tree_accessor_context_path_get(&ta), "/aa/a1") == 0);

    rst = tree_accessor_context_change(&ta, ".");
    MU_ASSERT(rst);
    MU_ASSERT(ta.context.currentContextNodeIndex == 2);
    MU_ASSERT(strcmp(tree_accessor_context_path_get(&ta), "/aa/a1") == 0);

    rst = tree_accessor_context_change(&ta, "f1");
    MU_ASSERT(rst);
    MU_ASSERT(ta.context.currentContextNodeIndex == 3);
    MU_ASSERT(strcmp(tree_accessor_context_path_get(&ta), "/aa/a1/f1") == 0);

    rst = tree_accessor_context_change(&ta, "../../a2");
    MU_ASSERT(rst);
    MU_ASSERT(ta.context.currentContextNodeIndex == 2);
    MU_ASSERT(strcmp(tree_accessor_context_path_get(&ta), "/aa/a2") == 0);

    rst = tree_accessor_context_change(&ta, "../a1/f2");
    MU_ASSERT(rst);
    MU_ASSERT(ta.context.currentContextNodeIndex == 3);
    MU_ASSERT(strcmp(tree_accessor_context_path_get(&ta), "/aa/a1/f2") == 0);

    rst = tree_accessor_context_change(&ta, "/");
    MU_ASSERT(rst);
    MU_ASSERT(ta.context.currentContextNodeIndex == 0);
    MU_ASSERT(strcmp(tree_accessor_context_path_get(&ta), "/") == 0);
};

static void set_test1()
{
    bool rst;
    char value[64];
    rst = tree_accessor_value_set(&ta, "/aa/a1/f1", value);
    MU_ASSERT(!rst);
    rst = tree_accessor_value_set(&ta, "/aa/a1", value);
    MU_ASSERT(!rst);
    rst = tree_accessor_value_set(&ta, "/aa", value);
    MU_ASSERT(!rst);
    rst = tree_accessor_value_set(&ta, "/", value);
    MU_ASSERT(!rst);
    rst = tree_accessor_value_set(&ta, "/cc", value);
    MU_ASSERT(!rst);
    rst = tree_accessor_value_set(&ta, "/aa/cc", value);
    MU_ASSERT(!rst);
    rst = tree_accessor_value_set(&ta, "/aa/a1/cc", value);
    MU_ASSERT(!rst);
    rst = tree_accessor_value_set(&ta, "/cc/a1/f1/cc", value);
    MU_ASSERT(!rst);
    rst = tree_accessor_value_set(&ta, "/cc/cc", value);
    MU_ASSERT(!rst);
};
static void get_test1()
{
    bool rst;
    char value[64];
    rst = tree_accessor_value_get(&ta, "/aa/a1/f1", value);
    MU_ASSERT(rst);
    rst = tree_accessor_value_get(&ta, "/aa/a1", value);
    MU_ASSERT(!rst);
    rst = tree_accessor_value_get(&ta, "/aa", value);
    MU_ASSERT(!rst);
    rst = tree_accessor_value_get(&ta, "/", value);
    MU_ASSERT(!rst);
    rst = tree_accessor_value_get(&ta, "/cc", value);
    MU_ASSERT(!rst);
    rst = tree_accessor_value_get(&ta, "/aa/cc", value);
    MU_ASSERT(!rst);
    rst = tree_accessor_value_get(&ta, "/aa/a1/cc", value);
    MU_ASSERT(!rst);
    rst = tree_accessor_value_get(&ta, "/cc/a1/f1/cc", value);
    MU_ASSERT(!rst);
    rst = tree_accessor_value_get(&ta, "/cc/cc", value);
    MU_ASSERT(!rst);
};

static void li_test1()
{
    bool rst;
    char **li;
    rst = tree_accessor_context_change(&ta, "/");
    li = tree_accessor_item_list(&ta, ".");
    MU_ASSERT(li != NULL);
    MU_ASSERT(strcmp(li[0], "aa") == 0);
    MU_ASSERT(strcmp(li[1], "bbs") == 0);
    MU_ASSERT(li[2] == NULL);

    li = tree_accessor_item_list(&ta, "aa");
    MU_ASSERT(li != NULL);
    MU_ASSERT(strcmp(li[0], "a1") == 0);
    MU_ASSERT(strcmp(li[1], "a2") == 0);
    MU_ASSERT(li[2] == NULL);

    li = tree_accessor_item_list(&ta, "aa/a1");
    MU_ASSERT(li != NULL);
    MU_ASSERT(strcmp(li[0], "f1") == 0);
    MU_ASSERT(strcmp(li[1], "f2") == 0);
    MU_ASSERT(li[2] == NULL);

    rst = tree_accessor_context_change(&ta, "/aa");
    li = tree_accessor_item_list(&ta, ".");
    MU_ASSERT(li != NULL);
    MU_ASSERT(strcmp(li[0], "a1") == 0);
    MU_ASSERT(strcmp(li[1], "a2") == 0);
    MU_ASSERT(li[2] == NULL);

    li = tree_accessor_item_list(&ta, "/");
    MU_ASSERT(li != NULL);
    MU_ASSERT(strcmp(li[0], "aa") == 0);
    MU_ASSERT(strcmp(li[1], "bbs") == 0);
    MU_ASSERT(li[2] == NULL);

    li = tree_accessor_item_list(&ta, "a1");
    MU_ASSERT(li != NULL);
    MU_ASSERT(strcmp(li[0], "f1") == 0);
    MU_ASSERT(strcmp(li[1], "f2") == 0);
    MU_ASSERT(li[2] == NULL);
};

static void cd_test2()
{
    bool rst;
    rst = tree_accessor_context_change(&ta, "/");
    MU_ASSERT(rst);
    MU_ASSERT(ta.context.currentContextNodeIndex == 0);
    MU_ASSERT(!strcmp(tree_accessor_context_path_get(&ta), "/"));
};
static void set_test2()
{

    bool rst;
    rst = tree_accessor_value_set(&ta, "/bbs[0]/bf1", "1000");
    MU_ASSERT(rst);
    MU_ASSERT(test_cfg_data[0].f1 == 1000);

    rst = tree_accessor_value_set(&ta, "/bbs[1]/bf1", "2000");
    MU_ASSERT(rst);
    MU_ASSERT(test_cfg_data[1].f1 == 2000);
};
static void get_test2()
{
    bool rst;
    char value[64];
    rst = tree_accessor_value_get(&ta, "/bbs[0]/bf1", value);
    MU_ASSERT(rst);
    MU_ASSERT(strcmp(value, "1000") == 0);

    rst = tree_accessor_value_get(&ta, "/bbs[1]/bf1", value);
    MU_ASSERT(rst);
    MU_ASSERT(strcmp(value, "2000") == 0);
};

static void cd_test3()
{
    bool rst;
    rst = tree_accessor_context_change(&ta, "/bbs[0]");
    MU_ASSERT(rst);
    MU_ASSERT(ta.context.currentContextNodeIndex == 1);
    MU_ASSERT(!strcmp(tree_accessor_context_path_get(&ta), "/bbs[0]"));
};

static void set_test3()
{
    bool rst;
    rst = tree_accessor_value_set(&ta, "bf1", "3000");
    MU_ASSERT(rst);
    MU_ASSERT(test_cfg_data[0].f1 == 3000);
};

static void get_test3()
{
    bool rst;
    char value[64];
    rst = tree_accessor_value_get(&ta, "bf1", value);
    MU_ASSERT(rst);
    MU_ASSERT(strcmp(value, "3000") == 0);
};

#endif //UNITTEST