#include <cstdlib>

#include "kiera_assert.h"

void test_tools()
{
    welkin::KIERA_CHECK(false, "hello...");
    welkin::KIERA_CHECK(false, "you can't see that...");
}

int main(int argc, const char **argv)
{
    test_tools();
    return 0;
}