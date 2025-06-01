#pragma once

#include <algorithm>
#include <vector>
#include <random>
#include <ctime>
#include <stdexcept>
#include <memory>
#include <initializer_list>
#include <type_traits>

template <typename KeyType, typename ValueType, typename Allocator = std::allocator<std::pair<const KeyType, ValueType>>>
class SkipList {
private:
    struct SkipNode {
        KeyType node_key;
        ValueType node_value;
        SkipNode* back_link;
        
        std::vector<SkipNode*> forward_links;
        
        SkipNode(const KeyType& k, const ValueType& v, SkipNode* prev, size_t levels) 
            : node_key(k), node_value(v), back_link(prev), forward_links(levels, nullptr) {}
    };

    size_t max_height;
    double promotion_chance;
    size_t current_height;
    size_t item_count;
    
    std::mt19937 rng_engine;
    std::uniform_real_distribution<> probability_dist;
    Allocator data_allocator;
    
    SkipNode* header;
    SkipNode* terminator;
    
    size_t generate_random_level() {
        size_t lvl = 1;
        while (probability_dist(rng_engine) < promotion_chance && lvl < max_height) {
            lvl++;
        }
        
        return lvl;
    }

    SkipNode* locate_node(const KeyType& search_key) const {
        SkipNode* current = header;
        for (int lvl = current_height - 1; lvl >= 0; --lvl) {
            while (current->forward_links[lvl] != terminator && current->forward_links[lvl]->node_key < search_key) {
                current = current->forward_links[lvl];
            }
        }
        
        current = current->forward_links[0];
        return (current != terminator && current->node_key == search_key) ? current : nullptr;
    }

    std::vector<SkipNode*> find_predecessors(const KeyType& search_key) const {
        std::vector<SkipNode*> preds(current_height, nullptr);
        
        SkipNode* current = header;
        
        for (int lvl = current_height - 1; lvl >= 0; --lvl) {
            while (current->forward_links[lvl] != terminator && 
                   current->forward_links[lvl]->node_key < search_key) {
                current = current->forward_links[lvl];
            }
            
            preds[lvl] = current;
        }
        
        return preds;
    }

public:
    class iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = ValueType;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        iterator() : container(nullptr), current_node(nullptr) {}
        iterator(SkipList* cont, SkipNode* node) : container(cont), current_node(node) {}

        reference operator*() const {
            if (!current_node || current_node == container->header) 
                throw std::out_of_range("Invalid iterator dereference");
                
            return current_node->node_value;
        }

        pointer operator->() const {
            return &(operator*());
        }

        iterator& operator++() {
            if (current_node) {
                current_node = current_node->forward_links[0];
                if (current_node == container->terminator) {
                    current_node = nullptr;
                }
            }
            
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            
            return temp;
        }

        iterator& operator--() {
            if (current_node) {
                current_node = current_node->back_link;
                if (current_node == container->header) {
                    current_node = nullptr;
                }
            } else {
                current_node = container->terminator->back_link;
                if (current_node == container->header) {
                    current_node = nullptr;
                }
            }
            
            return *this;
        }

        iterator operator--(int) {
            iterator temp = *this;
            --(*this);
            
            return temp;
        }

        bool operator==(const iterator& other) const {
            return current_node == other.current_node;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }

        KeyType key() const {
            if (!current_node) throw std::out_of_range("Invalid key access");
            
            return current_node->node_key;
        }

    private:
        SkipList* container;
        SkipNode* current_node;
        friend class SkipList;
    };

    class const_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = const ValueType;
        using difference_type = std::ptrdiff_t;
        using pointer = const ValueType*;
        using reference = const ValueType&;
    
        const_iterator() : container(nullptr), current_node(nullptr) {}
        const_iterator(const SkipList* cont, const SkipNode* node) 
            : container(cont), current_node(node) {}
    
        const_iterator(const iterator& other) 
            : container(other.container), current_node(other.current_node) {}

        reference operator*() const {
            if (!current_node || current_node == container->header) 
                throw std::out_of_range("Invalid const_iterator dereference");
                
            return current_node->node_value;
        }
    
        pointer operator->() const {
            return &(operator*());
        }

        const_iterator& operator++() {
            if (current_node) {
                current_node = current_node->forward_links[0];
                if (current_node == container->terminator) {
                    current_node = nullptr;
                }
            }
            
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator temp = *this;
            ++(*this);
            
            return temp;
        }
        
        const_iterator& operator--() {
            if (current_node) {
                current_node = current_node->back_link;
                if (current_node == container->header) {
                    current_node = nullptr;
                }
            } else {
                current_node = container->terminator->back_link;
                if (current_node == container->header) {
                    current_node = nullptr;
                }
            }
            
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator temp = *this;
            --(*this);
            return temp;
        }

        bool operator==(const const_iterator& other) const {
            return current_node == other.current_node;
        }

        bool operator!=(const const_iterator& other) const {
            return !(*this == other);
        }
   
        const KeyType& key() const {
            if (!current_node) throw std::out_of_range("Invalid key access");
            return current_node->node_key;
        }

    private:
        const SkipList* container;
        const SkipNode* current_node;
        friend class SkipList;
    };

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    SkipList(size_t height_limit = 16, double promotion_prob = 0.5, 
             const Allocator& alloc = Allocator())
        : max_height(height_limit),
          promotion_chance(promotion_prob),
          current_height(1),
          item_count(0),
          rng_engine(std::time(nullptr)),
          probability_dist(0.0, 1.0),
          data_allocator(alloc) {
        
        header = new SkipNode(KeyType(), ValueType(), nullptr, max_height);
        terminator = new SkipNode(KeyType(), ValueType(), header, max_height);
        
        for (size_t lvl = 0; lvl < max_height; ++lvl) {
            header->forward_links[lvl] = terminator;
        }
        terminator->back_link = header;
    }

    SkipList(std::initializer_list<std::pair<KeyType, ValueType>> init_list,
             size_t height_limit = 16, double promotion_prob = 0.5)
        : SkipList(height_limit, promotion_prob) {
        for (const auto& pair : init_list) {
            insert(pair.first, pair.second);
        }
    }

    SkipList(const SkipList& other) 
        : max_height(other.max_height),
          promotion_chance(other.promotion_chance),
          current_height(1),
          item_count(0),
          rng_engine(std::time(nullptr)),
          probability_dist(0.0, 1.0),
          data_allocator(other.data_allocator) {
        
        header = new SkipNode(KeyType(), ValueType(), nullptr, max_height);
        terminator = new SkipNode(KeyType(), ValueType(), header, max_height);
        
        for (size_t lvl = 0; lvl < max_height; ++lvl) {
            header->forward_links[lvl] = terminator;
        }
        
        for (auto it = other.begin(); it != other.end(); ++it) {
            insert(it.key(), *it);
        }
    }

    SkipList& operator=(const SkipList& other) {
        if (this != &other) {
            SkipList temp(other);
            swap(temp);
        }
        
        return *this;
    }

    ~SkipList() {
        purge();
        delete header;
        delete terminator;
    }

    iterator begin() noexcept {
        return iterator(this, header->forward_links[0] != terminator ? header->forward_links[0] : nullptr);
    }

    iterator end() noexcept {
        return iterator(this, nullptr);
    }

    const_iterator begin() const noexcept {
        return const_iterator(this, header->forward_links[0] != terminator ? header->forward_links[0] : nullptr);
    }

    const_iterator end() const noexcept {
        return const_iterator(this, nullptr);
    }

    const_iterator cbegin() const noexcept {
        return begin();
    }

    const_iterator cend() const noexcept {
        return end();
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(begin());
    }

    void insert(const KeyType& new_key, const ValueType& new_value) {
        auto predecessors = find_predecessors(new_key);
        
        if (predecessors[0]->forward_links[0] != terminator && 
            predecessors[0]->forward_links[0]->node_key == new_key) {
            predecessors[0]->forward_links[0]->node_value = new_value;
            
            return;
        }
        
        size_t new_level = generate_random_level();
        
        if (new_level > current_height) {
            predecessors.resize(new_level, header);
            current_height = new_level;
        }
        
        SkipNode* new_node = new SkipNode(new_key, new_value, predecessors[0], new_level);
        
        for (size_t lvl = 0; lvl < new_level; ++lvl) {
            new_node->forward_links[lvl] = predecessors[lvl]->forward_links[lvl];
            predecessors[lvl]->forward_links[lvl] = new_node;
        }
        
        new_node->back_link = predecessors[0];
        if (new_node->forward_links[0] != terminator) {
            new_node->forward_links[0]->back_link = new_node;
        } else {
            terminator->back_link = new_node;
        }
        
        item_count++;
    }

    void erase(const KeyType& key) {
        auto predecessors = find_predecessors(key);
        SkipNode* target = predecessors[0]->forward_links[0];
        
        if (target == terminator || target->node_key != key) return;
        
        for (size_t lvl = 0; lvl < target->forward_links.size(); ++lvl) {
            if (predecessors[lvl]->forward_links[lvl] == target) {
                predecessors[lvl]->forward_links[lvl] = target->forward_links[lvl];
            }
        }
        
        if (target->forward_links[0] != terminator) {
            target->forward_links[0]->back_link = target->back_link;
        } else {
            terminator->back_link = target->back_link;
        }
        
        delete target;
        item_count--;
        
        while (current_height > 1 && header->forward_links[current_height-1] == terminator) {
            current_height--;
        }
    }

    iterator find(const KeyType& key) {
        SkipNode* node = locate_node(key);
        return iterator(this, node);
    }

    const_iterator find(const KeyType& key) const {
        SkipNode* node = locate_node(key);
        return const_iterator(this, node);
    }

    ValueType& operator[](const KeyType& key) {
        SkipNode* node = locate_node(key);
        if (!node) {
            insert(key, ValueType());
            node = locate_node(key);
        }
        
        return node->node_value;
    }

    const ValueType& operator[](const KeyType& key) const {
        return at(key);
    }

    const ValueType& at(const KeyType& key) const {
        const SkipNode* node = locate_node(key);
        if (!node) throw std::out_of_range("Key not found");
        
        return node->node_value;
    }

    size_t size() const noexcept {
        return item_count;
    }

    bool empty() const noexcept {
        return item_count == 0;
    }

    void purge() noexcept {
        SkipNode* current = header->forward_links[0];
        while (current != terminator) {
            SkipNode* next = current->forward_links[0];
            delete current;
            current = next;
        }
        
        for (size_t lvl = 0; lvl < max_height; ++lvl) {
            header->forward_links[lvl] = terminator;
        }
        terminator->back_link = header;
        
        current_height = 1;
        item_count = 0;
    }

    void swap(SkipList& other) noexcept {
        std::swap(max_height, other.max_height);
        std::swap(promotion_chance, other.promotion_chance);
        std::swap(current_height, other.current_height);
        std::swap(item_count, other.item_count);
        std::swap(header, other.header);
        std::swap(terminator, other.terminator);
        std::swap(data_allocator, other.data_allocator);
    }

    friend bool operator<(const SkipList& lhs, const SkipList& rhs) {
        auto lhs_it = lhs.begin();
        auto rhs_it = rhs.begin();
    
        while (lhs_it != lhs.end() && rhs_it != rhs.end()) {
            if (lhs_it.key() < rhs_it.key()) return true;
            if (rhs_it.key() < lhs_it.key()) return false;
        
            if (*lhs_it < *rhs_it) return true;
            if (*rhs_it < *lhs_it) return false;
        
            ++lhs_it;
            ++rhs_it;
        }

        return lhs.size() < rhs.size();
    }

    friend bool operator>(const SkipList& lhs, const SkipList& rhs) {
        return rhs < lhs;
    }

    friend bool operator<=(const SkipList& lhs, const SkipList& rhs) {
        return !(rhs < lhs);
    }
    
    friend bool operator>=(const SkipList& lhs, const SkipList& rhs) {
        return !(lhs < rhs);
    }

    friend bool operator==(const SkipList& lhs, const SkipList& rhs) {
        if (lhs.size() != rhs.size()) return false;
        
        auto lit = lhs.begin();
        auto rit = rhs.begin();
        
        while (lit != lhs.end() && rit != rhs.end()) {
            if (*lit != *rit)
                return false;
            
            ++lit;
            ++rit;
        }
    
        return true;
    }

    friend bool operator!=(const SkipList& lhs, const SkipList& rhs) {
        return !(lhs == rhs);
    }

};
