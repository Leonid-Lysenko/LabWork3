#pragma once
#include <algorithm>
#include <vector>
#include <random>
#include <ctime>
#include <stdexcept>
#include <memory>
#include <initializer_list>

template <typename KeyType, typename ValueType, 
          typename Allocator = std::allocator<std::pair<const KeyType, ValueType>>>
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

};
