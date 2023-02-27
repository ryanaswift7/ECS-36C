#include <iostream>
#include "json.hpp"
#include "priorityqueue.h"


// the underlying structure holding the data is the nodes_ vector

PriorityQueue::PriorityQueue(std::size_t max_size) :
	nodes_(max_size + 1, KeyValuePair()),
	max_size_(max_size),
	size_(0) {
}

void PriorityQueue::insert(Key k) {
	insert(std::make_pair(k, std::make_pair(0, 0)));
}

void PriorityQueue::insert(KeyValuePair kv) {
    nodes_[++size_] = kv; // append the new node to end of heap
    heapifyUp(size_); // heapify new node into proper location
}

KeyValuePair PriorityQueue::min() {
    return nodes_[ROOT];
}

KeyValuePair PriorityQueue::removeMin() {
    KeyValuePair tmp = nodes_[ROOT]; // save the root node to return
    removeNode(ROOT); // remove root and repair heap
    return tmp; // return root of min heap
}

bool PriorityQueue::isEmpty() const {
    // cannot just check if empty bc heap starts at index 1
    // but vector starts at index 0 (check if something is at index 1 or beyond)
    if (nodes_.size() > 1) 
        return false;
    else
        return true;
}

size_t PriorityQueue::size() const {
	return nodes_.size()-1; // 1 less than size bc ignore index 0
}

nlohmann::json PriorityQueue::JSON() const {
	nlohmann::json result;
  for (size_t i = 1; i <= size_; i++) {
      nlohmann::json node;
      KeyValuePair kv = nodes_[i];
      node["key"] = kv.first;
      node["value"] = kv.second;
      if (i != ROOT) {
          node["parent"] = std::to_string(i / 2);
      }
      if (2 * i <= size_) {
          node["leftChild"] = std::to_string(2 * i);
      }
      if (2 * i + 1 <= size_) {
          node["rightChild"] = std::to_string(2 * i + 1);
      }
      result[std::to_string(i)] = node;
  }
  result["metadata"]["max_size"] = max_size_;
  result["metadata"]["size"] = size_;
	return result;
}

void PriorityQueue::heapifyUp(size_t i) {
        if (nodes_[floor(i/2.0)].first > nodes_[i].first){
            // if parent node key greater than child, swap
            std::swap(nodes_[i], nodes_[floor(i/2.0)]);

            // continue heapifyUp at next level up
            heapifyUp(floor(i/2.0));
            heapifyUp(size_);
        }
    //}
}

void PriorityQueue::heapifyDown(size_t i) {
    if ((2*i) <= size_){ // does i have at least 1 child
        if ((2*i)+1 <= size_){ // does i have 2 children
            // are either of the 2 children smaller than parent?
            if ((nodes_[i].first > nodes_[2*i].first) || (nodes_[i].first > nodes_[(2*i)+1].first)){
                // find the smaller of the children and swap with parent
                if (nodes_[2*i].first > nodes_[(2*i)+1].first){
                    std::swap(nodes_[i], nodes_[(2*i)+1]);
                    heapifyDown((2*i)+1);
                }
                else {
                    std::swap(nodes_[i], nodes_[2*i]);
                    heapifyDown(2*i);
                }    
            }
        }
        else { // know only 1 child
            // if child key smaller than parent, swap
            if (nodes_[i].first > nodes_[2*i].first){
                std::swap(nodes_[i], nodes_[2*i]);
                heapifyDown(2*i);
            }
        }
    }
}

void PriorityQueue::removeNode(size_t i) {
    std::swap(nodes_[size_], nodes_[i]);
    --size_;

    // Since the node that should be last is now out of place,
    // heapify down to remove violations and put it back at the end
    heapifyDown(i);
}

Key PriorityQueue::getKey(size_t i) {
    return nodes_[i].first; // KeyValuePair is defined as type pair with the key first
}
