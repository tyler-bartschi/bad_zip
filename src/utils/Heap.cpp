#include <bad_zip/utils/Heap.hpp>
#include <memory>
#include <utility>

namespace bad_zip {
HuffmanNode::HuffmanNode(const unsigned long long frequency, const int symbol, const int min_symbol,
                         const unsigned long long order)
    : frequency(frequency),
      symbol(symbol),
      min_symbol(min_symbol),
      order(order),
      left_child(nullptr),
      right_child(nullptr) {}

bool HuffmanNode::operator==(const HuffmanNode& other) const {
    return frequency == other.frequency && min_symbol == other.min_symbol && order == other.order;
}

bool HuffmanNode::operator<(const HuffmanNode& other) const {
    if (frequency != other.frequency) {
        return frequency < other.frequency;
    }

    if (min_symbol != other.min_symbol) {
        // uses min_symbol as a tiebreaker for heap comparison, so the subtree with smaller values
        // wins
        return min_symbol < other.min_symbol;
    }

    return order < other.order;
}

bool HuffmanNode::operator>(const HuffmanNode& other) const {
    if (frequency != other.frequency) {
        return frequency > other.frequency;
    }

    if (min_symbol != other.min_symbol) {
        return min_symbol > other.min_symbol;
    }

    return order > other.order;
}

HuffmanPQ::HuffmanPQ() = default;

void HuffmanPQ::push(std::unique_ptr<HuffmanNode> node) {
    if (!node) {
        return;
    }

    nodes_.push_back(std::move(node));
    percolate_up(size() - 1);
}

std::unique_ptr<HuffmanNode> HuffmanPQ::pop() {
    if (nodes_.empty()) {
        return nullptr;
    }

    auto result = std::move(nodes_[0]);

    if (nodes_.size() == 1) {
        nodes_.pop_back();
        return result;
    }

    nodes_[0] = std::move(nodes_.back());
    nodes_.pop_back();

    percolate_down(0);

    return result;
}

[[nodiscard]] size_t HuffmanPQ::size() const {
    return nodes_.size();
}

[[nodiscard]] bool HuffmanPQ::empty() const {
    return nodes_.empty();
}

void HuffmanPQ::clear() {
    nodes_.clear();
}

size_t HuffmanPQ::left_child(const size_t index) {
    return 2 * index + 1;
}

size_t HuffmanPQ::right_child(const size_t index) {
    return 2 * index + 2;
}

size_t HuffmanPQ::parent_node(const size_t index) {
    return (index - 1) / 2;
}

void HuffmanPQ::percolate_up(const size_t index) {
    // if it is smaller than the parent, then swap
    if (index == 0) {
        return;
    }

    if (const size_t parent_index = parent_node(index); *nodes_[index] < *nodes_[parent_index]) {
        swap(index, parent_index);
        percolate_up(parent_index);
    }
}

void HuffmanPQ::percolate_down(const size_t index) {
    // if it is larger than either child, swap with the smaller child
    if (nodes_.empty()) {
        return;
    }

    size_t smallest = index;

    if (const size_t left_index = left_child(index);
        left_index < size() && *nodes_[left_index] < *nodes_[smallest]) {
        smallest = left_index;
    }
    if (const size_t right_index = right_child(index);
        right_index < size() && *nodes_[right_index] < *nodes_[smallest]) {
        smallest = right_index;
    }

    if (smallest != index) {
        swap(index, smallest);
        percolate_down(smallest);
    }
}

void HuffmanPQ::swap(const size_t index_a, const size_t index_b) {
    std::swap(nodes_[index_a], nodes_[index_b]);
}
}  // namespace bad_zip
