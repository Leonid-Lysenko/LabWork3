#pragma once

#include <algorithm>
#include <vector>
#include <random>
#include <ctime>
#include <stdexcept>
#include <memory>
#include <initializer_list>

template <typename KeyType, typename ValueType, typename Allocator = std::allocator<std::pair<const KeyType, ValueType>>>
class SkipList {
private:
    struct SkipNode {
        KeyType node_key;
        ValueType node_value;
        SkipNode* back_link;
        
        std::vector<SkipNode*> forward_links;
        
        SkipNode(const KeyType& k, const ValueType& v, SkipNode* prev, size_t levels) 
            : node_key(k),node_value(v), back_link(prev), forward_links(levels, nullptr) {}
    };

    size_t max_height;
    size_t current_height;
    size_t item_count;
    
    double promotion_chance;
    
    SkipNode* header;
    SkipNode* terminator;
    
    std::mt19937 rng_engine;
    std::uniform_real_distribution<> probability_dist;
    Allocator data_allocator;
    
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
    };
};
