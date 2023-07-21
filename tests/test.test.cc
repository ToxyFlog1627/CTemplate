#include "test.hh"

TEST_SUITE("S", {
    TEST_CASE("C", {
        ASSERT(0);
        printf("nope :/\n");
        EXPECT(1);
    });
});