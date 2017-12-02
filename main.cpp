#include <iostream>
#include "persistent_set.h"
#include <algorithm>
#include "gtest/gtest.h"
#include <gmock/gmock.h>

struct my_struct {
    int x {};


    bool operator < (my_struct const& other) const {
        return x < other.x;
    }

    bool operator > (my_struct const& other) const {
        return x > other.x;
    }

    bool operator == (my_struct const& other) const {
        return x == other.x;
    }

    bool operator <= (my_struct const& other) const {
        return x <= other.x;
    }

    bool operator >= (my_struct const& other) const {
        return x >= other.x;
    }

    bool operator != (my_struct const& other) const {
        return x != other.x;
    }
};

std::string get_word() {
    int n = rand() % 100 + 1;
    std::string ans;
    for (int i = 0; i < n; ++i) {
        ans += rand() % 25 + 'a';
    }
    return ans;
}

TEST (RandomTests, insert_range_ints) {

    srand(time(nullptr));
    for (int xxx = 0; xxx < 10; ++xxx) {
        std::vector<int> cc, c;
        persistent_set<int> st;
        for (int i = 0, x = rand() % 200; i < 30; ++i, x = rand() % 200) {
            st.insert(x);
            cc.push_back(x);
        }

        std::sort(cc.begin(), cc.end());
        cc.resize(std::unique(cc.begin(), cc.end()) - cc.begin());

        for (int x : st) c.push_back(x);

        EXPECT_THAT(cc, ::testing::ContainerEq(c));
    }
}


TEST (RandomTests, insert_range_strings) {
    srand(time(nullptr));

    for (int xxx = 0; xxx < 10; ++xxx) {
        std::vector<std::string> cc, c;
        persistent_set<std::string> st;
        std::string x = get_word();
        for (int i = 0; i < 30; ++i, x = get_word()) {
            st.insert(x);
            cc.push_back(x);
        }

        std::sort(cc.begin(), cc.end());
        cc.resize(std::unique(cc.begin(), cc.end()) - cc.begin());

        for (std::string x : st) c.push_back(x);

        EXPECT_THAT(cc, ::testing::ContainerEq(c));
    }
}

TEST (RandomTests, insert_range_ints_mystructs) {
    srand(time(nullptr));

    for (int xxx = 0; xxx < 10; ++xxx) {
        std::vector<my_struct> cc, c;
        persistent_set<my_struct> st;
        my_struct x {};
        for (int i = 0; i < 30; ++i, x = my_struct{rand() % 100}) {
            st.insert(x);
            cc.push_back(x);
        }

        std::sort(cc.begin(), cc.end());
        cc.resize(std::unique(cc.begin(), cc.end()) - cc.begin());

        for (auto x : st) c.push_back(x);

        EXPECT_THAT(cc, ::testing::ContainerEq(c));
    }
}

TEST (RandomTests, insert_moveit_ints) {
    persistent_set<int> st;
    srand(time(nullptr));
    std::set<int> rst;
    for (int i = 0, x = rand() % 200 - 20; i < 30; ++i, x = rand() % 200 - 20) {
        st.insert(x);
        rst.insert(x);
    }

    auto rit = rst.begin();
    auto it = st.begin();


    std::vector<int> mv = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0};

    for (int x : mv) {
        if (x > 0) {
            it++;
            rit++;
        } else {
            it--;
            rit--;
        }
        EXPECT_TRUE(*rit == *it);
    }
}


TEST(RandomTests, insert_find_ints) {
    srand(time(nullptr));
    persistent_set<int> st;
    std::set<int> rst;
    for (int i = 0, x = rand() % 2000 - 1000; i < 30000; ++i, x = rand() % 2000 - 1000) {
        st.insert(x);
        rst.insert(x);
    }

    for (int i = 0; i < 100; ++i) {
        int x = rand() % 2000 - 1000;
        std::set<int>::iterator rit = rst.find(x);
        persistent_set<int>::iterator it = st.find(x);
        EXPECT_EQ(rit == rst.end(), it == st.end());
        if (rit != rst.end()) {
            EXPECT_EQ(*it, x);
        }
    }
}


TEST(RandomTests, insert_find_strings) {
    srand(5551);

    persistent_set<std::string> st;
    std::set<std::string> rst;
    std::string x = get_word();
    for (int i = 0; i < 30000; ++i, x = get_word()) {
        st.insert(x);
        rst.insert(x);
    }

    for (int i = 0; i < 100; ++i) {
        std::string x = get_word();
        std::set<std::string>::iterator rit = rst.find(x);
        persistent_set<std::string>::iterator it = st.find(x);
        EXPECT_EQ(rit == rst.end(), it == st.end());
        if (rit != rst.end()) {
            EXPECT_EQ(*it, x);
        }
    }

}

TEST(RandomTests, insert_erase_ints) {
    srand(time(nullptr));
    persistent_set<int> st;
    std::set<int> rst;
    std::vector<int> ccc;
    for (int i = 0, x = rand() % 2000 - 1000; i < 3000; ++i, x = rand() % 2000 - 1000) {
        st.insert(x);
        rst.insert(x);
        ccc.push_back(x);
    }

    for (int i = 0; i < 100; ++i) {
        int x = rand() % 2000 - 1000;
        if (x & 1) x = ccc[rand() % 3000];
        std::set<int>::iterator rit = rst.find(x);
        persistent_set<int>::iterator it = st.find(x);
        EXPECT_EQ(rit == rst.end(), it == st.end());
        if (rit != rst.end()) {
            st.erase(it);
            rst.erase(rit);
        }
    }

    auto it = st.begin();
    auto rit = rst.begin();

    while (it != st.end()) {
        EXPECT_EQ(*it, *rit);
        ++it;
        ++rit;
    }
}


TEST(RandomTests, insert_erase_strings) {
    srand(time(nullptr));
    persistent_set<std::string> st;
    std::set<std::string> rst;
    std::string x = get_word();
    std::vector<std::string> ccc;
    for (int i = 0; i < 3000; ++i, x = get_word()) {
        st.insert(x);
        rst.insert(x);
        ccc.push_back(x);
    }

    for (int i = 0; i < 100; ++i) {
        std::string x = get_word();
        if (i & 1) x = ccc[rand() % 3000];
        std::set<std::string>::iterator rit = rst.find(x);
        persistent_set<std::string>::iterator it = st.find(x);
        EXPECT_EQ(rit == rst.end(), it == st.end());
        if (rit != rst.end()) {
            st.erase(it);
            rst.erase(rit);
        }
    }

    auto it = st.begin();
    auto rit = rst.begin();

    while (it != st.end()) {
        EXPECT_EQ(*it, *rit);
        ++it;
        ++rit;
    }
}

TEST(CornerCases, empty_set) {
    persistent_set<int> st;
    EXPECT_EQ(st.begin(), st.end());
}

TEST(CornerCases, empty_set_ins_erase) {
    persistent_set<int> st;
    st.insert(1);
    st.erase(st.begin());
    EXPECT_EQ(st.begin(), st.end());
}

TEST(CornerCases, empty_set_find) {
    persistent_set<int> st;
    EXPECT_TRUE(st.find(123) == st.end());
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
