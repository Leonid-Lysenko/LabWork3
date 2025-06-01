/* Leonid Lysenko st128618@student.spbu.ru
   Lab3
*/

#include <gtest/gtest.h>
#include "skip_list.h"
#include <string>
#include <vector>
#include <algorithm>

/**
 * @brief Tests default construction of SkipList.
 * Verifies that a newly created SkipList is empty and size is zero.
 */
TEST(SkipListTest, DefaultConstructor) {
    SkipList<int, std::string> sl;
    EXPECT_TRUE(sl.empty());
    EXPECT_EQ(sl.size(), 0);
}

/**
 * @brief Tests construction of SkipList from initializer list.
 * Checks that elements are correctly inserted and accessible.
 */
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

/**
 * @brief Tests insertion and access of elements.
 * Verifies that inserting existing keys updates values without changing size.
 */
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

/**
 * @brief Tests operator[] for insertion and access.
 * Checks both mutable and const access.
 */
TEST(SkipListTest, OperatorBrackets) {
    SkipList<int, bool> sl;
    sl[1] = true;
    sl[2] = false;
    
    EXPECT_EQ(sl[1], true);
    EXPECT_EQ(sl[2], false);
    
    const auto& csl = sl;
    EXPECT_EQ(csl[1], true);
}

/**
 * @brief Tests at() method for element access with bounds checking.
 * Verifies correct retrieval and exception throwing for missing keys.
 */
TEST(SkipListTest, AtMethod) {
    SkipList<int, double> sl;
    sl.insert(1, 3.14);
    
    EXPECT_DOUBLE_EQ(sl.at(1), 3.14);
    EXPECT_THROW(sl.at(99), std::out_of_range);
    
    const auto& csl = sl;
    EXPECT_DOUBLE_EQ(csl.at(1), 3.14);
    EXPECT_THROW(csl.at(99), std::out_of_range);
}

/**
 * @brief Tests erasing elements by key.
 * Checks size adjustment and that erasing non-existent keys has no effect.
 */
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

/**
 * @brief Tests find() method for locating elements.
 * Verifies iterator correctness for found and not found keys.
 */
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

/**
 * @brief Tests iteration over SkipList elements.
 * Confirms iteration order and correctness of values.
 */
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

/**
 * @brief Tests const iteration over SkipList elements.
 * Ensures const correctness and iteration order.
 */
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

/**
 * @brief Tests reverse iteration over SkipList.
 * Checks correctness of reverse order traversal.
 */
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

/**
 * @brief Tests copy constructor of SkipList.
 * Verifies that the copy has the same elements and size.
 */
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

/**
 * @brief Tests copy assignment operator.
 * Checks that assigned SkipList matches the source.
 */
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

/**
 * @brief Tests swap operation between two SkipLists.
 * Verifies that contents are exchanged correctly.
 */
TEST(SkipListTest, Swap) {
    SkipList<int, int> sl1 = {{1, 1}};
    SkipList<int, int> sl2 = {{2, 2}};
    
    sl1.swap(sl2);
    
    EXPECT_EQ(sl1.size(), 1);
    EXPECT_EQ(sl2.size(), 1);
    EXPECT_EQ(sl1[2], 2);
    EXPECT_EQ(sl2[1], 1);
}

/**
 * @brief Tests clearing all elements via purge().
 * Ensures SkipList becomes empty with size zero.
 */
TEST(SkipListTest, Clear) {
    SkipList<int, int> sl = {
        {1, 1},
        {2, 2}
    };
    
    sl.purge();
    EXPECT_TRUE(sl.empty());
    EXPECT_EQ(sl.size(), 0);
}

/**
 * @brief Tests comparison operators for equality and inequality.
 * Checks correctness of operator== and operator!=.
 */
TEST(SkipListTest, ComparisonOperators) {
    SkipList<int, int> sl1 = {{1, 1}, {2, 2}};
    SkipList<int, int> sl2 = {{1, 1}, {2, 2}};
    SkipList<int, int> sl3 = {{1, 1}, {3, 3}};
    
    EXPECT_TRUE(sl1 == sl2);
    EXPECT_FALSE(sl1 != sl2);
    EXPECT_TRUE(sl1 != sl3);
    EXPECT_FALSE(sl1 == sl3);
}

/**
 * @brief Tests iterator validity after insertions and erasures.
 * Ensures iterators remain valid and point to correct elements.
 */
TEST(SkipListTest, IteratorValidity) {
    SkipList<int, int> sl = {{1, 100}};
    auto it = sl.begin();
    
    sl.insert(2, 200);
    ++it;
    EXPECT_EQ(*it, 200);
    
    sl.erase(1);
    EXPECT_EQ(*it, 200);
}

/**
 * @brief Tests usage of SkipList with a custom allocator.
 * Verifies insertion and access with custom allocator type.
 */
TEST(SkipListTest, CustomAllocator) {
    using CustomAlloc = std::allocator<std::pair<const int, double>>;
    SkipList<int, double, CustomAlloc> sl;
    
    sl.insert(1, 3.14159);
    EXPECT_EQ(sl.size(), 1);
    EXPECT_DOUBLE_EQ(sl[1], 3.14159);
}

/**
 * @brief Stress test with large number of insertions and deletions.
 * Checks correctness and performance under heavy usage.
 */
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

/**
 * @brief Tests reverse iterators for correctness of traversal.
 * Verifies reverse iteration order and values.
 */
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

/**
 * @brief Tests const reverse iterators for correctness.
 * Ensures reverse traversal works on const SkipList.
 */
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

/**
 * @brief Tests reverse iterator operations including increment and decrement.
 * Checks correctness of operator++, operator--, post-increment and post-decrement.
 */
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

/**
 * @brief Tests detailed comparison operators for various SkipList instances.
 * Verifies relational operators <, >, <=, >=, ==, != with different keys and values.
 */
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

/**
 * @brief Tests reverse iterator edge cases on empty and single-element SkipList.
 * Checks that reverse iterators behave correctly at boundaries.
 */
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

/**
 * @brief Tests comparison operators with SkipLists of different sizes.
 * Verifies relational operators behave correctly when sizes differ.
 */
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

