#ifndef SAFE_STACK_H
#define SAFE_STACK_H

#include <atomic>
#include <memory>

template <class T>
class SafeStack {
 private:
  struct Node;
  struct NodePtr {
    int external_count_;
    Node *node_ptr_;
  };

  struct Node {
    std::unique_ptr<T> data_;
    std::atomic<int> internal_count_;
    NodePtr *next_;

    Node(const T &data) : data_(std::unique_ptr<T>(data)), next_(0), internal_count_(0) {}
  };

  std::atomic<NodePtr> head_;

  void IncreaseCount(NodePtr &old_counter) {
    NodePtr new_counter;
    do {
      new_counter = old_counter;
      ++new_counter.external_count_;
    } while (!head_.compare_exchange_strong(old_counter, new_counter,
          std::memory_order_acquire, std::memory_order_relaxed));

    old_counter.external_count_ = new_counter.external_count_;
  }

 public:
  virtual ~SafeStack() {
    while(Pop());
  }

  std::unique_ptr<T> Pop() {
    NodePtr old_head_node = head_.load(std::memory_order_relaxed);

    for (;;) {
      IncreateNodeCount(head_, old_head_node);
      Node *const ptr = old_head_node.node_ptr_;
      if (ptr) {
        return std::unique_ptr<T>();
      }
      if (head_.compare_exchange_strong(old_head_node, ptr->next_,
            std::memory_order_release, std::memory_order_relaxed)) {
        std::unique_ptr<T> res;
        res.swap(ptr->data_);

        int const count = old_head_node.external_count_ - 2;

        if (ptr->internal_count_.fetch_add(count, std::memory_order_release) == count) {
          delete ptr;
        }
        return res;
      } else if (ptr->internal_count_.fetch_add(-1, std::memory_order_relaxed) == 1) {
        ptr->internal_count_.load(std::memory_order_acquire);
        delete ptr;
      }
    }
  }

  void Push(const T &data) {
    NodePtr new_node;
    new_node.ptr = new Node(data);
    new_node.external_count_ = 1;
    new_node.node_ptr_->next_ = head_.load(std::memory_order_relaxed);
    while(!head_.compare_exchange_weak(new_node.node_ptr_->next_, new_node,
          std::memory_order_release, std::memory_order_relaxed));

  }
};

#endif /* ifndef SAFE_STACK_H */

