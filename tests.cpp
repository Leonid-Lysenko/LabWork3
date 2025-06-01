#include <gtest/gtest.h>
#include "skip_list.h"
#include <string>
#include <vector>
#include <algorithm>

TEST(SkipListTest, DefaultConstructor) {
    SkipList<int, std::string> sl;
    EXPECT_TRUE(sl.empty());
    EXPECT_EQ(sl.size(), 0);
}

TEST(SkipListTest, InitializerListConstructor) {
    SkipList<int, std::string> sl = {
        {1, "first"},
        {2, "second"},
        {3, "third"}
    };
    EXPECT_EQ(sl.size(), 3);
    EXPECT_EQ(sl[1], "first");
    EXPECT_EQ(sl[2], "second");
    EXPECT_EQ(sl[3], "third");
}

TEST(SkipListTest, InsertAndAccess) {
    SkipList<int, int> sl;
    sl.insert(1, 100);
    sl.insert(2, 200);
    
    EXPECT_EQ(sl.size(), 2);
    EXPECT_EQ(sl[1], 100);
    EXPECT_EQ(sl[2], 200);
    
    sl.insert(1, 150);
    EXPECT_EQ(sl.size(), 2);
    EXPECT_EQ(sl[1], 150);
}

TEST(SkipListTest, OperatorBrackets) {
    SkipList<int, bool> sl;
    sl[1] = true;
    sl[2] = false;
    
    EXPECT_EQ(sl[1], true);
    EXPECT_EQ(sl[2], false);
    
    const auto& csl = sl;
    EXPECT_EQ(csl[1], true);
}

TEST(SkipListTest, AtMethod) {
    SkipList<int, double> sl;
    sl.insert(1, 3.14);
    
    EXPECT_DOUBLE_EQ(sl.at(1), 3.14);
    EXPECT_THROW(sl.at(99), std::out_of_range);
    
    const auto& csl = sl;
    EXPECT_DOUBLE_EQ(csl.at(1), 3.14);
    EXPECT_THROW(csl.at(99), std::out_of_range);
}

TEST(SkipListTest, Erase) {
    SkipList<int, char> sl = {
        {1, 'A'},
        {2, 'B'},
        {3, 'C'}
    };
    
    sl.erase(2);
    EXPECT_EQ(sl.size(), 2);
    EXPECT_EQ(sl[1], 'A');
    EXPECT_EQ(sl[3], 'C');
    EXPECT_EQ(sl.find(2), sl.end());
    
    sl.erase(99);
    EXPECT_EQ(sl.size(), 2);
}

TEST(SkipListTest, Find) {
    SkipList<int, int> sl = {
        {1, 10},
        {2, 20}
    };
    
    auto it = sl.find(1);
    EXPECT_NE(it, sl.end());
    EXPECT_EQ(*it, 10);
    
    it = sl.find(99);
    EXPECT_EQ(it, sl.end());
    
    const auto& csl = sl;
    auto cit = csl.find(2);
    EXPECT_NE(cit, csl.end());
    EXPECT_EQ(*cit, 20);
}

TEST(SkipListTest, Iteration) {
    SkipList<int, int> sl = {
        {1, 10},
        {2, 20},
        {3, 30}
    };
    
    std::vector<int> values;
    for (auto& val : sl) {
        values.push_back(val);
    }
    
    EXPECT_EQ(values.size(), 3);
    EXPECT_EQ(values[0], 10);
    EXPECT_EQ(values[1], 20);
    EXPECT_EQ(values[2], 30);
}

TEST(SkipListTest, ConstIteration) {
    const SkipList<int, int> sl = {
        {1, 100},
        {2, 200}
    };
    
    std::vector<int> values;
    for (const auto& val : sl) {
        values.push_back(val);
    }
    
    EXPECT_EQ(values.size(), 2);
    EXPECT_EQ(values[0], 100);
    EXPECT_EQ(values[1], 200);
}

TEST(SkipListTest, ReverseIteration) {
    SkipList<int, int> sl = {
        {1, 10},
        {2, 20},
        {3, 30}
    };
    
    auto it = sl.end();
    --it;
    EXPECT_EQ(*it, 30);
    --it;
    EXPECT_EQ(*it, 20);
    --it;
    EXPECT_EQ(*it, 10);
    EXPECT_EQ(it, sl.begin());
}

TEST(SkipListTest, CopyConstructor) {
    SkipList<int, int> sl1 = {
        {1, 1},
        {2, 2}
    };
    
    SkipList<int, int> sl2(sl1);
    
    EXPECT_EQ(sl1.size(), sl2.size());
    EXPECT_EQ(sl1[1], sl2[1]);
    EXPECT_EQ(sl1[2], sl2[2]);
}

TEST(SkipListTest, AssignmentOperator) {
    SkipList<int, int> sl1 = {
        {1, 1},
        {2, 2}
    };
    
    SkipList<int, int> sl2;
    sl2 = sl1;
    
    EXPECT_EQ(sl1.size(), sl2.size());
    EXPECT_EQ(sl1[1], sl2[1]);
    EXPECT_EQ(sl1[2], sl2[2]);
}

TEST(SkipListTest, Swap) {
    SkipList<int, int> sl1 = {{1, 1}};
    SkipList<int, int> sl2 = {{2, 2}};
    
    sl1.swap(sl2);
    
    EXPECT_EQ(sl1.size(), 1);
    EXPECT_EQ(sl2.size(), 1);
    EXPECT_EQ(sl1[2], 2);
    EXPECT_EQ(sl2[1], 1);
}

TEST(SkipListTest, Clear) {
    SkipList<int, int> sl = {
        {1, 1},
        {2, 2}
    };
    
    sl.purge();
    EXPECT_TRUE(sl.empty());
    EXPECT_EQ(sl.size(), 0);
}

TEST(SkipListTest, ComparisonOperators) {
    SkipList<int, int> sl1 = {{1, 1}, {2, 2}};
    SkipList<int, int> sl2 = {{1, 1}, {2, 2}};
    SkipList<int, int> sl3 = {{1, 1}, {3, 3}};
    
    EXPECT_TRUE(sl1 == sl2);
    EXPECT_FALSE(sl1 != sl2);
    EXPECT_TRUE(sl1 != sl3);
    EXPECT_FALSE(sl1 == sl3);
}

TEST(SkipListTest, IteratorValidity) {
    SkipList<int, int> sl = {{1, 100}};
    auto it = sl.begin();
    
    sl.insert(2, 200);
    ++it;
    EXPECT_EQ(*it, 200);
    
    sl.erase(1);
    EXPECT_EQ(*it, 200);
}

TEST(SkipListTest, CustomAllocator) {
    using CustomAlloc = std::allocator<std::pair<const int, double>>;
    SkipList<int, double, CustomAlloc> sl;
    
    sl.insert(1, 3.14159);
    EXPECT_EQ(sl.size(), 1);
    EXPECT_DOUBLE_EQ(sl[1], 3.14159);
}

TEST(SkipListTest, StressTest) {
    SkipList<int, int> sl;
    const int N = 1000;
    
    for (int i = 0; i < N; ++i) {
        sl.insert(i, i*i);
    }
    
    EXPECT_EQ(sl.size(), N);
    
    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(sl[i], i*i);
    }
    
    for (int i = 0; i < N; i += 2) {
        sl.erase(i);
    }
    
    EXPECT_EQ(sl.size(), N/2);
    
    for (int i = 1; i < N; i += 2) {
        EXPECT_EQ(sl[i], i*i);
    }
}

TEST(SkipListTest, ReverseIterators) {
    SkipList<int, std::string> sl = {
        {1, "one"},
        {2, "two"},
        {3, "three"}
    };
    
    std::vector<std::string> reversed_values;
    for (auto it = sl.rbegin(); it != sl.rend(); ++it) {
        reversed_values.push_back(*it);
    }
    
    EXPECT_EQ(reversed_values.size(), 3);
    EXPECT_EQ(reversed_values[0], "three");
    EXPECT_EQ(reversed_values[1], "two");
    EXPECT_EQ(reversed_values[2], "one");
}

TEST(SkipListTest, ConstReverseIterators) {
    const SkipList<int, std::string> sl = {
        {1, "first"},
        {2, "second"}
    };
    
    std::vector<std::string> values;
    for (auto it = sl.crbegin(); it != sl.crend(); ++it) {
        values.push_back(*it);
    }
    
    EXPECT_EQ(values.size(), 2);
    EXPECT_EQ(values[0], "second");
    EXPECT_EQ(values[1], "first");
}

TEST(SkipListTest, ReverseIteratorOperations) {
    SkipList<int, int> sl = {{1, 10}, {2, 20}, {3, 30}};
    
    auto rit = sl.rbegin();
    EXPECT_EQ(*rit, 30);
    
    ++rit;
    EXPECT_EQ(*rit, 20);
    
    --rit;
    EXPECT_EQ(*rit, 30);
    
    rit++;
    EXPECT_EQ(*rit, 20);
    
    rit--;
    EXPECT_EQ(*rit, 30);
}

TEST(SkipListTest, ComparisonOperatorsDetailed) {
    SkipList<int, int> sl1 = {{1, 10}, {2, 20}};
    SkipList<int, int> sl2 = {{1, 10}, {2, 20}};
    SkipList<int, int> sl3 = {{1, 10}, {2, 25}};
    SkipList<int, int> sl4 = {{1, 10}, {3, 20}};
    SkipList<int, int> sl5 = {{1, 10}};
    
    EXPECT_TRUE(sl1 == sl2);
    EXPECT_FALSE(sl1 == sl3);
    
    EXPECT_FALSE(sl1 != sl2);
    EXPECT_TRUE(sl1 != sl3);
    
    EXPECT_TRUE(sl5 < sl1);
    EXPECT_FALSE(sl1 < sl5);
    EXPECT_TRUE(sl1 < sl3);
    EXPECT_TRUE(sl1 < sl4);
    
    EXPECT_TRUE(sl1 > sl5);
    EXPECT_FALSE(sl5 > sl1);
    EXPECT_TRUE(sl3 > sl1);
    EXPECT_TRUE(sl4 > sl1);
    
    EXPECT_TRUE(sl5 <= sl1);
    EXPECT_TRUE(sl1 <= sl2);
    EXPECT_FALSE(sl1 <= sl5);
    
    EXPECT_TRUE(sl1 >= sl5);
    EXPECT_TRUE(sl1 >= sl2);
    EXPECT_FALSE(sl5 >= sl1);
}

TEST(SkipListTest, ReverseIteratorEdgeCases) {
    SkipList<int, int> empty;
    EXPECT_EQ(empty.rbegin(), empty.rend());
    
    SkipList<int, int> single = {{1, 100}};
    auto rit = single.rbegin();
    EXPECT_NE(rit, single.rend());
    EXPECT_EQ(*rit, 100);
    ++rit;
    EXPECT_EQ(rit, single.rend());
}

TEST(SkipListTest, ComparisonWithDifferentSizes) {
    SkipList<int, int> sl1 = {{1, 10}};
    SkipList<int, int> sl2 = {{1, 10}, {2, 20}};
    
    EXPECT_TRUE(sl1 < sl2);
    EXPECT_FALSE(sl2 < sl1);
    EXPECT_TRUE(sl1 <= sl2);
    EXPECT_FALSE(sl2 <= sl1);
    EXPECT_TRUE(sl2 > sl1);
    EXPECT_FALSE(sl1 > sl2);
    EXPECT_TRUE(sl2 >= sl1);
    EXPECT_FALSE(sl1 >= sl2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
