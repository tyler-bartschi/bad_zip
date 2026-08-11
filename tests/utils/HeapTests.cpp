#include <bad_zip/utils/Heap.hpp>
#include <catch2/catch_test_macros.hpp>
#include <memory>
using bad_zip::HuffmanNode, bad_zip::HuffmanPQ;

TEST_CASE("HuffmanNodes compare correctly", "[HuffmanNode]") {
    SECTION("HuffmanNodes are correctly equal to each other") {
        const auto node_a = HuffmanNode(12, 5, 5, 2);
        const auto node_b = HuffmanNode(12, 16, 5, 2);

        REQUIRE(node_a == node_b);
    }

    SECTION("HuffmanNodes are correctly not equal to each other") {
        const auto node_a = HuffmanNode(12, 3, 3, 2);
        const auto node_b = HuffmanNode(3, 12, 1, 1);

        REQUIRE(node_a != node_b);
    }

    SECTION("Huffman nodes compare correctly with frequencies differing") {
        const auto node_a = HuffmanNode(1, 2, 3, 4);
        const auto node_b = HuffmanNode(4, 5, 6, 7);

        REQUIRE(node_a < node_b);
        REQUIRE(node_b > node_a);
    }

    SECTION("Huffman nodes compare correctly with frequencies the same and min_symbol differing") {
        const auto node_a = HuffmanNode(4, 12, 3, 8);
        const auto node_b = HuffmanNode(4, 89, 8, 3);

        REQUIRE(node_a < node_b);
        REQUIRE(node_b > node_a);
    }

    SECTION(
        "Huffman nodes compare correctly with frequencies and min_symbol the same, order "
        "differing") {
        const auto node_a = HuffmanNode(4, 87, 9, 7);
        const auto node_b = HuffmanNode(4, 23, 9, 23);

        REQUIRE(node_a < node_b);
        REQUIRE(node_b > node_a);
    }
}

TEST_CASE("HuffmanPQ operates correctly", "[HuffmanPQ]") {
    auto pq = HuffmanPQ();

    SECTION("HuffmanPQ starts with size 0") {
        REQUIRE(pq.empty());
    }

    SECTION("HuffmanPQ clear method does not break when already empty") {
        REQUIRE_NOTHROW(pq.clear());
    }

    SECTION("HuffmanPQ push method does not throw") {
        REQUIRE_NOTHROW(pq.push(std::make_unique<HuffmanNode>(12, 12, 12, 12)));
        REQUIRE(pq.size() == 1);
    }

    SECTION("HuffmanPQ clear method works") {
        pq.push(std::make_unique<HuffmanNode>(12, 12, 12, 12));
        pq.clear();
        REQUIRE(pq.empty());
    }

    SECTION("HuffmanPQ pushes and pops 1 item at a time properly") {
        pq.push(std::make_unique<HuffmanNode>(12, 12, 12, 12));
        auto result = pq.pop();

        REQUIRE(pq.empty());
        REQUIRE((result->frequency == 12 && result->symbol == 12 && result->min_symbol == 12 &&
                 result->order == 12));

        pq.push(std::make_unique<HuffmanNode>(1, 1, 1, 1));
        result = pq.pop();
        REQUIRE(pq.empty());
        REQUIRE((result->frequency == 1 && result->symbol == 1 && result->min_symbol == 1 &&
                 result->order == 1));
    }

    SECTION("HuffmanPQ push ignores nullptr") {
        pq.push(nullptr);
        REQUIRE(pq.empty());
    }

    SECTION("HuffmanPQ pop return nullptr when pq empty") {
        auto result = pq.pop();
        REQUIRE(result == nullptr);
    }

    SECTION("Multiple pushes in order pop in the same order as received") {
        pq.push(std::make_unique<HuffmanNode>(1, 1, 1, 1));
        pq.push(std::make_unique<HuffmanNode>(2, 2, 2, 2));
        pq.push(std::make_unique<HuffmanNode>(3, 3, 3, 3));

        REQUIRE(pq.size() == 3);

        auto result = pq.pop();
        REQUIRE(*result == HuffmanNode(1, 1, 1, 1));
        REQUIRE(pq.size() == 2);

        result = pq.pop();
        REQUIRE(*result == HuffmanNode(2, 2, 2, 2));
        REQUIRE(pq.size() == 1);

        result = pq.pop();
        REQUIRE(*result == HuffmanNode(3, 3, 3, 3));
        REQUIRE(pq.empty());
    }

    SECTION("Multiple pushes out of order pop in the min order") {
        pq.push(std::make_unique<HuffmanNode>(5, 2, 2, 2));
        pq.push(std::make_unique<HuffmanNode>(5, 12, 1, 3));
        pq.push(std::make_unique<HuffmanNode>(10, 54, 2, 1));

        REQUIRE(pq.size() == 3);

        auto result = pq.pop();
        REQUIRE(*result == HuffmanNode(5, 12, 1, 3));
        REQUIRE(pq.size() == 2);

        result = pq.pop();
        REQUIRE(*result == HuffmanNode(5, 2, 2, 2));
        REQUIRE(pq.size() == 1);

        result = pq.pop();
        REQUIRE(*result == HuffmanNode(10, 54, 2, 1));
        REQUIRE(pq.empty());
    }

    SECTION("Alternating pushes and pops retain correct ordering") {
        pq.push(std::make_unique<HuffmanNode>(8, 23, 8, 1));
        pq.push(std::make_unique<HuffmanNode>(2, 12, 3, 4));

        auto result = pq.pop();
        REQUIRE(*result == HuffmanNode(2, 12, 3, 4));
        REQUIRE(pq.size() == 1);

        pq.push(std::make_unique<HuffmanNode>(100, 8, 1, 9));
        REQUIRE(pq.size() == 2);

        result = pq.pop();
        REQUIRE(*result == HuffmanNode(8, 23, 8, 1));
        REQUIRE(pq.size() == 1);

        pq.push(std::make_unique<HuffmanNode>(2, 98, 120, 3));
        pq.push(std::make_unique<HuffmanNode>(2, 91, 120, 2));
        REQUIRE(pq.size() == 3);

        result = pq.pop();
        REQUIRE(*result == HuffmanNode(2, 91, 120, 2));
        REQUIRE(pq.size() == 2);

        result = pq.pop();
        REQUIRE(*result == HuffmanNode(2, 98, 120, 3));
        REQUIRE(pq.size() == 1);

        result = pq.pop();
        REQUIRE(*result == HuffmanNode(100, 8, 1, 9));
        REQUIRE(pq.empty());
    }

    SECTION("Clear works") {
        pq.push(std::make_unique<HuffmanNode>(2, 1, 3, 4));
        pq.push(std::make_unique<HuffmanNode>(12, 43, 2, 1));

        pq.clear();

        REQUIRE(pq.empty());
    }
}