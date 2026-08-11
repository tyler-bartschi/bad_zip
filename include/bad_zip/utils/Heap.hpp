#pragma once

#include <memory>
#include <vector>

namespace bad_zip {

struct HuffmanNode {
    const unsigned long long frequency;
    const int symbol;                // 0..255 for leaf nodes (actual symbols), -1 for internal node
    const int min_symbol;            // min symbol in subtree starting at this node
    const unsigned long long order;  // order created, assigned as last deterministic tiebreaker

    std::unique_ptr<HuffmanNode> left_child;
    std::unique_ptr<HuffmanNode> right_child;

    explicit HuffmanNode(unsigned long long frequency, int symbol, int min_symbol,
                         unsigned long long order);

    bool operator==(const HuffmanNode& other) const;  // node heap priority are the same
    bool operator<(const HuffmanNode& other) const;
    bool operator>(const HuffmanNode& other) const;
};

class HuffmanPQ {
public:
    explicit HuffmanPQ();

    void push(std::unique_ptr<HuffmanNode> node);
    std::unique_ptr<HuffmanNode> pop();
    [[nodiscard]] size_t size() const;
    [[nodiscard]] bool empty() const;
    void clear();

private:
    std::vector<std::unique_ptr<HuffmanNode>> nodes_;  // vector of nodes

    static size_t left_child(size_t index);
    static size_t right_child(size_t index);
    static size_t parent_node(size_t index);

    void percolate_up(size_t index);
    void percolate_down(size_t index);

    void swap(size_t index_a, size_t index_b);
};
}  // namespace bad_zip
