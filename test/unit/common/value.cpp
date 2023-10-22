#include <gtest/gtest.h>

#include "common/value.hpp"

TEST(Value, ReadAfterOverwrite) {
    pvm::Value v;

    v.overwrite<int>(1);

    EXPECT_EQ(v.get<int>(), 1);
}

TEST(Value, ReadWrongType) {
    pvm::Value v;

    v.overwrite<int>(1);

    EXPECT_THROW(auto val = v.get<float>(), pvm::ValueMismatchError);
}

TEST(Value, WriteWrongType) {
    pvm::Value v;

    v.overwrite<int>(1);

    EXPECT_THROW(v.write<float>(1), pvm::ValueMismatchError);
}

