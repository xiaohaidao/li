#ifndef SAFE_QUEUE_H
#define SAFE_QUEUE_H

#include <atomic>
#include <memory>

template<class T>
class SafeQueue {
 private:
  struct Node;
  struct NodePtr {
    int external_count_;
    Node *node_ptr_;
  };

  struct NodeCount {
    int internal_count_:30;
    unsigned external_counters_:2;
  };

  struct Node {
    std::atomic<T *> data_;
    std::atomic<NodeCount> count_;
    std::atomic<NodePtr> next_;

    Node() {
      NodeCount new_count;
      new_count.internal_count_ = 0;
      new_count.external_counters_ = 2; // tail_ and head_
      count_.store(new_count);

      NodePtr new_node = {0};
      next_.store(new_node);
    }
    void Release() {
      NodeCount old_counter = count_.load(std::memory_order_relaxed);
      NodeCount new_counter;
      do {
        new_counter = old_counter;
        --new_counter.internal_count_;

      } while(!count_.compare_exchange_strong(old_counter, new_counter,
            std::memory_order_acquire, std::memory_order_relaxed));

      if (!new_counter.internal_count_ && !new_counter.external_counters_) {
        delete this;
      }
    }
  };

  std::atomic<NodeCount> head_;
  std::atomic<NodeCount> tail_;

  static void IncreaseExternalCount(std::atomic<NodePtr> &counter,
      NodePtr &old_counter) {

    NodePtr new_counter;
    do {
      new_counter = old_counter;
      ++new_counter.external_count_;
    } while(!counter.compare_exchange_strong(old_counter, new_counter,
          std::memory_order_acquire, std::memory_order_relaxed));

    old_counter.external_count_ = new_counter.external_count_;
  }

  static void FreeExternalCount(NodePtr &old_node_ptr) {
    Node *const ptr = old_node_ptr.node_ptr_;
    const int count = old_node_ptr.external_count_ - 2;

    NodeCount old_counter = ptr->count_.load(std::memory_order_relaxed);
    NodeCount new_counter;
    do {
      new_counter = old_counter;
      --new_counter.external_counters_;
      new_counter.internal_count_ += count;
    } while(!ptr->count_.compare_exchange_strong(old_counter, new_counter,
          std::memory_order_acquire, std::memory_order_relaxed));

    if (!new_counter.internal_count_ && !new_counter.external_counters_) {
      delete ptr;
    }
  }

  void SetNewTail(NodePtr &old_tail, const NodePtr &new_tail) {
    Node *const current_tail_ptr = old_tail.node_ptr_;
    while(!tail_.compare_exchange_weak(old_tail, new_tail) &&
        old_tail.node_ptr_ == current_tail_ptr);

    if(old_tail.node_ptr_ == current_tail_ptr) {
      FreeExternalCount(old_tail);
    } else {
      current_tail_ptr->Release();
    }
  }

  SafeQueue(const SafeQueue&) = delete;
  SafeQueue &operator =(const SafeQueue&) = delete;

 public:
  SafeQueue() {
    NodePtr new_node;
    new_node.external_count_ = 1;
    new_node.node_ptr_ = new Node;

    head_.store(new_node);
    tail_.store(new_node);
  }

  ~SafeQueue() {
    while(Pop());
    delete head_.load().node_ptr_;
  }

  void Push(const T &data) {
    std::unique_ptr<T> new_data(new T(data));
    NodePtr new_next;
    new_next.node_ptr_ = new Node;
    new_next.external_count_ = 1;
    NodePtr old_tail = tail_.load();
    for (;;) {
      IncreaseExternalCount(tail_, old_tail);
      T *old_data = NULL;
      if (old_tail.node_ptr_->data_.compare_exchange_strong(
        old_data, new_data.get())) {

        NodePtr old_next = {0};
        if (!old_tail.node_ptr_->next_.compare_exchange_strong(
              old_next, new_next)) {

          delete new_next.node_ptr_;
          new_next = old_next;
        }
        SetNewTail(old_tail, new_next);
        new_data.release();
        break;
      } else {
        NodePtr old_next = {0};
        if (old_tail.node_ptr_->next_.compare_exchange_strong(old_next, new_next)) {
          old_next = new_next;
          new_next.node_ptr_ = new Node;
        }
        SetNewTail(old_tail, old_next);
      }
    }
  }

  std::unique_ptr<T> Pop() {
    NodePtr old_head = head_.load(std::memory_order_relaxed);
    for (;;) {
      IncreaseExternalCount(head_, old_head);
      Node *const ptr = old_head.node_ptr_;
      if (ptr == tail_.load().node_ptr_) {
        return std::unique_ptr<T>();
      }
      NodePtr next = ptr->next_.load();
      if (head_.compare_exchange_strong(old_head, next)) {
        T *const res = ptr->data_.exchange(NULL);
        FreeExternalCount(old_head);
        return std::unique_ptr<T>(res);
      }
      ptr->Release();
    }
  }

};

#endif /* ifndef SAFE_QUEUE_H */

