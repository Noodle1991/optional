#include <gtest/gtest.h>
#include <string>
#include "optional.h"

struct test_struct {
    test_struct() noexcept {
        ++def_constr;
    }

    test_struct(const test_struct &v) noexcept {
        v.update();
        ++copy_constr;
    }

    test_struct(test_struct &&v) noexcept {
        std::move(*this).update();
        ++move_constr;
    }

    test_struct &operator=(const test_struct &other) noexcept {
        if (this != &other) {
            other.update();
            ++copy_assign;
        }
        return *this;
    }

    test_struct &operator=(test_struct &&other) noexcept {
        std::move(*this).update();
        ++move_assign;
        return *this;
    }

    ~test_struct() {
        ++destruct;
    }

    void update() const &{
        ++c_lvalue_call_count;
    }

    void update() &&{
        ++rvalue_call_count;
    }

    static size_t real_time_counter() {
        return def_constr + copy_constr + move_constr - destruct;
    }

    static void reset() {
        size_t def_constr = 0;
        size_t copy_constr = 0;
        size_t move_constr = 0;
        size_t copy_assign = 0;
        size_t move_assign = 0;
        size_t destruct = 0;
        size_t lvalue_call_count = 0;
        size_t rvalue_call_count = 0;
        size_t c_lvalue_call_count = 0;
    }

    inline static size_t def_constr = 0;
    inline static size_t copy_constr = 0;
    inline static size_t move_constr = 0;
    inline static size_t copy_assign = 0;
    inline static size_t move_assign = 0;
    inline static size_t destruct = 0;
    inline static size_t lvalue_call_count = 0;
    inline static size_t rvalue_call_count = 0;
    inline static size_t c_lvalue_call_count = 0;
};

TEST(optional_test, empty_optional) {
    bmstu::optional<test_struct> opt;
    ASSERT_FALSE(opt.has_value());
    ASSERT_EQ(test_struct::real_time_counter(), 0);
}

TEST(optional_test, value_int) {
    int a = 12345678;
    bmstu::optional<int> aopt;
    ASSERT_FALSE(aopt.has_value());
    aopt = a;
    ASSERT_EQ(aopt.value(), 12345678);
    ASSERT_TRUE(aopt.has_value());
}

TEST(optional_test, value_string) {
    bmstu::optional<std::string> aopt;
    std::string a = "qwerty";
    ASSERT_FALSE(aopt.has_value());
    aopt = a;
    ASSERT_TRUE(aopt.has_value());
    ASSERT_STREQ(aopt.value().c_str(), "qwerty");
}

TEST(optional_test, default_constructor) {
    {
        bmstu::optional<test_struct> opt;
        ASSERT_FALSE(opt.has_value());
        ASSERT_EQ(test_struct::def_constr, 0);
        ASSERT_EQ(test_struct::copy_constr, 0);
        ASSERT_EQ(test_struct::move_constr, 0);
        ASSERT_EQ(test_struct::copy_assign, 0);
        ASSERT_EQ(test_struct::move_assign, 0);
        ASSERT_EQ(test_struct::destruct, 0);
        ASSERT_EQ(test_struct::lvalue_call_count, 0);
        ASSERT_EQ(test_struct::c_lvalue_call_count, 0);
        ASSERT_EQ(test_struct::rvalue_call_count, 0);
        ASSERT_EQ(test_struct::real_time_counter(), 0);
    }
    ASSERT_EQ(test_struct::real_time_counter(), 0);
}

TEST(optional_test, move_constructor) {
    {
        test_struct a;
        bmstu::optional<test_struct> opt(std::move(a));
        ASSERT_TRUE(opt.has_value());
        ASSERT_EQ(test_struct::def_constr, 1);
        ASSERT_EQ(test_struct::copy_constr, 0);
        ASSERT_EQ(test_struct::move_constr, 1);
        ASSERT_EQ(test_struct::copy_assign, 0);
        ASSERT_EQ(test_struct::move_assign, 0);
        ASSERT_EQ(test_struct::destruct, 0);
        ASSERT_EQ(test_struct::lvalue_call_count, 0);
        ASSERT_EQ(test_struct::c_lvalue_call_count, 0);
        ASSERT_EQ(test_struct::rvalue_call_count, 1);
        ASSERT_EQ(test_struct::real_time_counter(), 2);
    }
    ASSERT_EQ(test_struct::real_time_counter(), 0);
}

TEST(optional_test, reset) {
    {
        test_struct a;
        bmstu::optional<test_struct> opt(a);
        ASSERT_TRUE(opt.has_value());
        opt.reset();
        ASSERT_FALSE(opt.has_value());
        ASSERT_EQ(test_struct::real_time_counter(), 1);
        ASSERT_EQ(test_struct::destruct, 1);
        ASSERT_EQ(test_struct::real_time_counter(), 1);
    }
    ASSERT_EQ(test_struct::real_time_counter(), 0);
}

TEST(optional_test, value) {
    {
        test_struct a;
        bmstu::optional<test_struct> opt(a);
        ASSERT_TRUE(opt.has_value());
        ASSERT_EQ(test_struct::def_constr, 1);
        ASSERT_EQ(test_struct::copy_constr, 1);
        ASSERT_EQ(test_struct::move_constr, 0);
        ASSERT_EQ(test_struct::copy_assign, 0);
        ASSERT_EQ(test_struct::move_assign, 0);
        ASSERT_EQ(test_struct::destruct, 0);
        ASSERT_EQ(test_struct::lvalue_call_count, 0);
        ASSERT_EQ(test_struct::c_lvalue_call_count, 1);
        ASSERT_EQ(test_struct::rvalue_call_count, 0);
        ASSERT_EQ(test_struct::real_time_counter(), 2);
    }
    ASSERT_EQ(test_struct::real_time_counter(), 0);
}

TEST(optional_test, copy_constructor) {
    test_struct::reset();
    {
        test_struct a;
        bmstu::optional<test_struct> opt(a);
        ASSERT_TRUE(opt.has_value());
        ASSERT_EQ(test_struct::def_constr, 1);
        ASSERT_EQ(test_struct::copy_constr, 1);
        ASSERT_EQ(test_struct::move_constr, 0);
        ASSERT_EQ(test_struct::copy_assign, 0);
        ASSERT_EQ(test_struct::move_assign, 0);
        ASSERT_EQ(test_struct::destruct, 0);
        ASSERT_EQ(test_struct::lvalue_call_count, 0);
        ASSERT_EQ(test_struct::c_lvalue_call_count, 1);
        ASSERT_EQ(test_struct::rvalue_call_count, 0);
        ASSERT_EQ(test_struct::real_time_counter(), 2);
    }
    ASSERT_EQ(test_struct::real_time_counter(), 0);
}