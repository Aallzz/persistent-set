#include <iostream>
#include "persistent_set.h"
#include "smart_linked_pointer.h"
#include <algorithm>
#include <random>
#include "gtest/gtest.h"
#include <gmock/gmock.h>

struct my_struct {
    int x;


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
        ans += (char)(rand() % 25 + 'a');
    }
    return ans;
}

TEST(SharedPtr_Construction, default_constructor_int) {
    persistent_set<int> st;
}

TEST(SharedPtr_Construction, copy_constructor_int) {
    persistent_set<int> st;
    std::vector<int> c {6, 1, 5, 2, 3, 8, 10, 9, 11, 18, 303, 123, 404, 1, -123};
    for (int x : c) st.insert(x);
    persistent_set<int> st2(st);
    st2.insert(13);
    EXPECT_TRUE(st.find(13) == st.end());
    EXPECT_TRUE(st2.find(13) != st2.end());
    st.insert(14);
    EXPECT_TRUE(st.find(14) != st.end());
    EXPECT_TRUE(st2.find(14) == st2.end());
    st2.erase(st2.find(13));
    st.erase(st.find(14));
    for (int x : st) {
        EXPECT_TRUE(st2.find(x) != st2.end());
    }
    for (int x : st2) {
        EXPECT_TRUE(st.find(x) != st.end());
    }
    auto st3(st);

    sort(c.begin(), c.end());
    c.resize(unique(c.begin(), c.end()) - c.begin());
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(c), std::end(c), rng);

    for (int x : c) {
        st3.erase(st3.find(x));
        ASSERT_TRUE(st.find(x) != st.end());
        ASSERT_TRUE(st3.find(x) == st3.end());
    }
}

TEST(SharedPtr_Construction, move_constructor_int) {
    persistent_set<int> st;
    persistent_set<int> sst;
    persistent_set<int> ssst;
    std::vector<int> c {6, 1, 5, 2, 3, 8, 10, 9, 11, 18, 303, 123, 404, 1, -123};
    for (int x : c) st.insert(x), sst.insert(x), ssst.insert(x);
    persistent_set<int> st2(std::move(st));
    st2.insert(13);
    EXPECT_TRUE(sst.find(13) == sst.end());
    EXPECT_TRUE(st2.find(13) != st2.end());
    sst.insert(14);
    EXPECT_TRUE(sst.find(14) != sst.end());
    EXPECT_TRUE(st2.find(14) == st2.end());
    st2.erase(st2.find(13));
    sst.erase(sst.find(14));
    for (int x : sst) {
        EXPECT_TRUE(st2.find(x) != st2.end());
    }
    for (int x : st2) {
        EXPECT_TRUE(sst.find(x) != sst.end());
    }

    auto st3(std::move(sst));

    sort(c.begin(), c.end());
    c.resize(unique(c.begin(), c.end()) - c.begin());
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(c), std::end(c), rng);

    for (int x : c) {
        st3.erase(st3.find(x));
        ASSERT_TRUE(ssst.find(x) != ssst.end());
        ASSERT_TRUE(st3.find(x) == st3.end());
    }

}

TEST(SharedPtr_Assignment, copy_assignment) {
    std::vector<int> c {6, 7, 3, 4, 8};
    persistent_set<int> st;
    for (int x : c) st.insert(x);
    persistent_set<int> sst;
    std::vector<int> cc {2, 1, 5, 9};
    for (int x : cc) sst.insert(x);
    sst = st;
    st.insert(1123);
    ASSERT_TRUE(sst.find(1123) == sst.end());
    for (int x : c) {
        ASSERT_TRUE(sst.find(x) != sst.end());
    }
    for (int x : cc) {
        ASSERT_TRUE(sst.find(x) == sst.end());
    }
}

TEST(SharedPtr_Assignment, move_assignment) {
    std::vector<int> c {6, 7, 3, 4, 8};
    persistent_set<int> st;
    for (int x : c) st.insert(x);
    persistent_set<int> sst;
    std::vector<int> cc {2, 1, 5, 9};
    for (int x : cc) sst.insert(x);
    sst = std::move(st);
    for (int x : c) {
        ASSERT_TRUE(sst.find(x) != sst.end());
    }
    for (int x : cc) {
        ASSERT_TRUE(sst.find(x) == sst.end());
    }
}

TEST(SharedPtr_CornerCases, empty_set) {
    persistent_set<int> st;
    EXPECT_EQ(st.begin(), st.end());
}

TEST(SharedPtr_CornerCases, copy_constr_empty_set) {
    persistent_set<int> st;
    persistent_set<int> sst(st);
    EXPECT_EQ(st.begin(), st.end());
    EXPECT_EQ(sst.begin(), sst.end());
}

TEST(SharedPtr_CornerCases, empty_set_ins_erase) {
    persistent_set<int> st;
    st.insert(1);
    st.erase(st.begin());
    EXPECT_EQ(st.begin(), st.end());
}

TEST(SharedPtr_CornerCases, empty_set_find) {
    persistent_set<int> st;
    EXPECT_TRUE(st.find(123) == st.end());
}

TEST(SharedPtr_CornerCases, empty_assignment) {
    persistent_set<int> st;
    persistent_set<int> sst;
    st = sst;
    EXPECT_EQ(st.begin(), st.end());
}

TEST(SharedPtr_RandomTests, insert_range_ints) {
    srand((unsigned)time(nullptr));
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


TEST(SharedPtr_RandomTests, insert_range_strings) {
    srand((unsigned)time(nullptr));

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

TEST(SharedPtr_RandomTests, insert_range_ints_mystructs) {
    srand((unsigned)time(nullptr));

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

TEST(SharedPtr_RandomTests, insert_moveit_ints) {
    persistent_set<int> st;
    srand((unsigned)time(nullptr));
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


TEST(SharedPtr_RandomTests, insert_find_ints) {
    srand((unsigned)time(nullptr));
    persistent_set<int> st;
    std::set<int> rst;
    for (int i = 0, x = rand() % 2000 - 1000; i < 3000; ++i, x = rand() % 2000 - 1000) {
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


TEST(SharedPtr_RandomTests, insert_find_strings) {
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

TEST(SharedPtr_RandomTests, insert_erase_ints) {
    srand((unsigned)time(nullptr));
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


TEST(SharedPtr_RandomTests, insert_erase_strings) {
    srand((unsigned)time(nullptr));
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

TEST(SharedPtr_RandomTests, copy_constructor_int) {
    srand(4418);
    persistent_set<int> st;
    std::vector<int> c;
    for (int i = 0; i < 1000; ++i) c.push_back(rand() % 1000 - 500);

    for (int x : c) st.insert(x);

    persistent_set<int> st2(st);
    std::vector<int> add;
    for (int i = 0; i < 50; ++i) {
        add.push_back(rand() % 10000 + 1000);
        st2.insert(add.back());
        EXPECT_TRUE(st.find(add.back()) == st.end());
        EXPECT_TRUE(st2.find(add.back()) != st2.end());
    }
    for (int x : add) {
        st2.erase(st2.find(x));
    }
    for (int x : st) {
        EXPECT_TRUE(st2.find(x) != st2.end());
    }
    for (int x : st2) {
        EXPECT_TRUE(st.find(x) != st.end());
    }

    auto st3(st);

    sort(c.begin(), c.end());
    c.resize(unique(c.begin(), c.end()) - c.begin());
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(c), std::end(c), rng);

    for (int x : c) {
        st3.erase(st3.find(x));
        ASSERT_TRUE(st.find(x) != st.end());
        ASSERT_TRUE(st3.find(x) == st3.end());
    }
}

TEST(SmartLinkedPtr_RandomTests, easy_check) {
        persistent_set<int, smart_linked_pointer> st;
        st.insert(19);
        st.insert(186);
        auto it = st.begin();
        EXPECT_TRUE(*it == 19);
        ++it;
        EXPECT_TRUE(*it == 186);
        ++it;
}

TEST(SmartLinkedPtr_RandomTests, insert_range_ints) {
    srand((unsigned)time(nullptr));
    for (int xxx = 0; xxx < 10; ++xxx) {
        std::vector<int> cc, c;
        persistent_set<int, smart_linked_pointer> st;
        for (int i = 0, x = rand() % 200; i < 3; ++i, x = rand() % 200) {
            st.insert(x);
            cc.push_back(x);
        }

        std::sort(cc.begin(), cc.end());
        cc.resize(std::unique(cc.begin(), cc.end()) - cc.begin());

        for (int x : st)
            c.push_back(x);


        EXPECT_THAT(cc, ::testing::ContainerEq(c));
    }
}

TEST(SmartLinkedPtr_RandomTests, insert_range_strings) {
    srand((unsigned)time(nullptr));

    for (int xxx = 0; xxx < 10; ++xxx) {
        std::vector<std::string> cc, c;
        persistent_set<std::string, smart_linked_pointer> st;
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

TEST(SmartLinkedPtr_RandomTests, insert_range_ints_mystructs) {
    srand((unsigned)time(nullptr));

    for (int xxx = 0; xxx < 10; ++xxx) {
        std::vector<my_struct> cc, c;
        persistent_set<my_struct, smart_linked_pointer> st;
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

TEST(SmartLinkedPtr_RandomTests, insert_moveit_ints) {
    persistent_set<int, smart_linked_pointer> st;
    srand((unsigned)time(nullptr));
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

TEST(SmartLinkedPtr_RandomTests, insert_find_ints) {
    srand((unsigned)time(nullptr));
    persistent_set<int, smart_linked_pointer> st;
    std::set<int> rst;
    for (int i = 0, x = rand() % 2000 - 1000; i < 3000; ++i, x = rand() % 2000 - 1000) {
        st.insert(x);
        rst.insert(x);
    }

    for (int i = 0; i < 100; ++i) {
        int x = rand() % 2000 - 1000;
        std::set<int>::iterator rit = rst.find(x);
        persistent_set<int, smart_linked_pointer>::iterator it = st.find(x);
        EXPECT_EQ(rit == rst.end(), it == st.end());
        if (rit != rst.end()) {
            EXPECT_EQ(*it, x);
        }
    }
}

TEST(SmartLinkedPtr_RandomTests, insert_find_erase_ints) {
    srand((unsigned)time(nullptr));
    persistent_set<int, smart_linked_pointer> st;
    persistent_set<int, smart_linked_pointer> sst;
    std::set<int> cst, csst;
    std::set<int> rst;
    for (int i = 0; i < 10000; ++i) {
        int step = rand() % 8;
        int x = rand() % 1000;
        switch (step) {
        case 0:
            cst.insert(x);
            st.insert(x);
            break;
        case 1:
            csst.insert(x);
            sst.insert(x);
            break;
        case 2:
            if (st.find(x) != st.end()) {
                st.erase(st.find(x));
                cst.erase(x);
            }
            break;
        case 3:
            if (sst.find(x) != sst.end()) {
                sst.erase(sst.find(x));
                csst.erase(x);
            }
            break;
        case 4:
            if (st.begin() != st.end()) {
                st.erase(st.begin());
                cst.erase(cst.begin());
            }
            break;
        case 5:
            if (sst.begin() != sst.end()) {
                sst.erase(sst.begin());
                csst.erase(csst.begin());
            }
            break;
        case 6:
            st = sst;
            cst = csst;
            break;
        case 7:
            sst = st;
            csst = cst;
            break;
        default:
            break;
        }
        std::vector<int> vst, vsst;
        for (int x : st) vst.push_back(x);
        for (int x : sst) vsst.push_back(x);
        std::vector<int> vcst, vcsst;
        for (int x : cst) vcst.push_back(x);
        for (int x : csst) vcsst.push_back(x);
        EXPECT_EQ(vst, vcst);
        EXPECT_EQ(vsst, vcsst);
    }
}



TEST(SmartLinkedPtr_RandomTests, insert_find_strings) {
    srand(5551);

    persistent_set<std::string, smart_linked_pointer> st;
    std::set<std::string> rst;
    std::string x = get_word();
    for (int i = 0; i < 30000; ++i, x = get_word()) {
        st.insert(x);
        rst.insert(x);
    }

    for (int i = 0; i < 100; ++i) {
        std::string x = get_word();
        std::set<std::string>::iterator rit = rst.find(x);
        persistent_set<std::string, smart_linked_pointer>::iterator it = st.find(x);
        EXPECT_EQ(rit == rst.end(), it == st.end());
        if (rit != rst.end()) {
            EXPECT_EQ(*it, x);
        }
    }

}

TEST(SmartLinkedPtr_RandomTests, copy_constructor_int) {
    srand(4418);
    persistent_set<int, smart_linked_pointer> st;
    std::vector<int> c;
    for (int i = 0; i < 3; ++i) c.push_back(rand() % 1000 - 500);

    for (int x : c) st.insert(x);

    persistent_set<int, smart_linked_pointer> st2(st);

    for (int x : st) {
        EXPECT_TRUE(st2.find(x) != st2.end());
    }

    std::vector<int> add;
    for (int i = 0; i < 2; ++i) {
        add.push_back(rand() % 10000 + 1000);
        st2.insert(add.back());
        EXPECT_TRUE(st.find(add.back()) == st.end());
        EXPECT_TRUE(st2.find(add.back()) != st2.end());
    }
    for (int x : add) {
        ASSERT_TRUE(st2.find(x) != st2.end());
        st2.erase(st2.find(x));
    }

    for (int x : st) {
        EXPECT_TRUE(st2.find(x) != st2.end());
    }
    for (int x : st2) {
        EXPECT_TRUE(st.find(x) != st.end());
    }

    auto st3(st);

    sort(c.begin(), c.end());
    c.resize(unique(c.begin(), c.end()) - c.begin());
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(c), std::end(c), rng);

    for (int x : c) {
        st3.erase(st3.find(x));
        ASSERT_TRUE(st.find(x) != st.end());
        ASSERT_TRUE(st3.find(x) == st3.end());
    }
}


TEST(SmartLinkedPtr_Construction, default_constructor_int) {
    persistent_set<int, smart_linked_pointer> st;
}

TEST(SmartLinkedPtr_Construction, copy_constructor_int) {
    persistent_set<int, smart_linked_pointer> st;
    std::vector<int> c {6, 1, 5, 2, 3, 8, 10, 9, 11, 18, 303, 123, 404, 1, -123};
    for (int x : c) st.insert(x);
    persistent_set<int, smart_linked_pointer> st2(st);
    st2.insert(13);
    EXPECT_TRUE(st.find(13) == st.end());
    EXPECT_TRUE(st2.find(13) != st2.end());
    st.insert(14);
    EXPECT_TRUE(st.find(14) != st.end());
    EXPECT_TRUE(st2.find(14) == st2.end());
    st2.erase(st2.find(13));
    st.erase(st.find(14));
    for (int x : st) {
        EXPECT_TRUE(st2.find(x) != st2.end());
    }
    for (int x : st2) {
        EXPECT_TRUE(st.find(x) != st.end());
    }
    auto st3(st);

    sort(c.begin(), c.end());
    c.resize(unique(c.begin(), c.end()) - c.begin());
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(c), std::end(c), rng);

    for (int x : c) {
        st3.erase(st3.find(x));
        ASSERT_TRUE(st.find(x) != st.end());
        ASSERT_TRUE(st3.find(x) == st3.end());
    }
}

TEST(SmartLinkedPtr_Construction, move_constructor_int) {
    persistent_set<int, smart_linked_pointer> st;
    persistent_set<int, smart_linked_pointer> sst;
    persistent_set<int, smart_linked_pointer> ssst;
    std::vector<int> c {6, 1, 5, 2, 3, 8, 10, 9, 11, 18, 303, 123, 404, 1, -123};
    for (int x : c) st.insert(x), sst.insert(x), ssst.insert(x);
    persistent_set<int, smart_linked_pointer> st2(std::move(st));
    st2.insert(13);
    EXPECT_TRUE(sst.find(13) == sst.end());
    EXPECT_TRUE(st2.find(13) != st2.end());
    sst.insert(14);
    EXPECT_TRUE(sst.find(14) != sst.end());
    EXPECT_TRUE(st2.find(14) == st2.end());
    st2.erase(st2.find(13));
    sst.erase(sst.find(14));
    for (int x : sst) {
        EXPECT_TRUE(st2.find(x) != st2.end());
    }
    for (int x : st2) {
        EXPECT_TRUE(sst.find(x) != sst.end());
    }

    auto st3(std::move(sst));

    sort(c.begin(), c.end());
    c.resize(unique(c.begin(), c.end()) - c.begin());
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(c), std::end(c), rng);

    for (int x : c) {
        st3.erase(st3.find(x));
        ASSERT_TRUE(ssst.find(x) != ssst.end());
        ASSERT_TRUE(st3.find(x) == st3.end());
    }

}

TEST(SmartLinkedPtr_Assignment, copy_assignment) {
    std::vector<int> c {6, 7, 3, 4, 8};
    persistent_set<int, smart_linked_pointer> st;
    for (int x : c) st.insert(x);
    persistent_set<int, smart_linked_pointer> sst;
    std::vector<int> cc {2, 1, 5, 9};
    for (int x : cc) sst.insert(x);
    sst = st;
    st.insert(1123);
    ASSERT_TRUE(sst.find(1123) == sst.end());
    for (int x : c) {
        ASSERT_TRUE(sst.find(x) != sst.end());
    }
    for (int x : cc) {
        ASSERT_TRUE(sst.find(x) == sst.end());
    }
}

TEST(SmartLinkedPtr_Assignment, move_assignment) {
    std::vector<int> c {6, 7, 3, 4, 8};
    persistent_set<int, smart_linked_pointer> st;
    for (int x : c) st.insert(x);
    persistent_set<int, smart_linked_pointer> sst;
    std::vector<int> cc {2, 1, 5, 9};
    for (int x : cc) sst.insert(x);
    sst = std::move(st);
    for (int x : c) {
        ASSERT_TRUE(sst.find(x) != sst.end());
    }
    for (int x : cc) {
        ASSERT_TRUE(sst.find(x) == sst.end());
    }
}

TEST(SmartLinkedPtrr_CornerCases, empty_set) {
    persistent_set<int, smart_linked_pointer> st;
    EXPECT_EQ(st.begin(), st.end());
}

TEST(SmartLinkedPtr_CornerCases, copy_constr_empty_set) {
    persistent_set<int, smart_linked_pointer> st;
    persistent_set<int, smart_linked_pointer> sst(st);
    EXPECT_EQ(st.begin(), st.end());
    EXPECT_EQ(sst.begin(), sst.end());
}

TEST(SmartLinkedPtr_CornerCases, empty_set_ins_erase) {
    persistent_set<int, smart_linked_pointer> st;
    st.insert(1);
    st.erase(st.begin());
    EXPECT_EQ(st.begin(), st.end());
}

TEST(SmartLinkedPtr_CornerCases, empty_assignment) {
    persistent_set<int, smart_linked_pointer> st;
    persistent_set<int, smart_linked_pointer> sst;
    st = sst;
    st.insert(1);
    sst.insert(2);
    EXPECT_TRUE(*(st.begin()) == 1);
    EXPECT_TRUE(*(sst.begin()) == 2);
}

TEST(SmartLinkedPtr_CornerCases, self_assignment) {
    persistent_set<int, smart_linked_pointer> st;
    st.insert(12);
    st = st;
    st.insert(1);
    EXPECT_TRUE(*(st.begin()) == 1);
}

template struct smart_linked_pointer<std::string>;
template struct smart_linked_pointer<int>;


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
//    ::testing::GTEST_FLAG(filter) = "";
    return RUN_ALL_TESTS();
}
