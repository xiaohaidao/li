#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <atomic>
#include <functional>
#include <future>
#include <queue>
#include <thread>
#include <vector>

#include "safe_queue.hpp"
#include "singleton.hpp"

class JoinThreads {
 private:
  std::vector<std::thread> &threads_;

 public:
  explicit JoinThreads(std::vector<std::thread> &threads) : threads_(threads){}
  ~JoinThreads() {
    for (unsigned i = 0; i < threads_.size(); ++i) {
      if (threads_[i].joinable()) {
        threads_[i].join();
      }
    }
  }
};

class FunctionWrapper : public noncopyable {
 private:
  struct ImplBase {
    virtual void Call() = 0;
    virtual ~ImplBase();
  };

  template<class F>
  struct ImplType : ImplBase {
    F f_;
    ImplType(F &&f): f_(std::move(f)) {}
    void Call() { f_(); }
  };

  std::unique_ptr<ImplBase> impl_;

 public:
  template<class F>
  FunctionWrapper(F &&f): impl_(new ImplType<F>(std::move(f))) {}
  FunctionWrapper(FunctionWrapper &&other): impl_(std::move(other.impl_)) {}
  FunctionWrapper &operator =(FunctionWrapper &&other) {
    impl_ = std::move(other.impl_);
    return *this;
  }
  void operator ()() { impl_->Call(); }
};

template<class T>
class ThreadPool : public Singleton<ThreadPool<T> > {
 private:
  std::atomic_bool done_;
  SafeQueue<FunctionWrapper> work_queue_;
  std::vector<std::thread> threads_;
  JoinThreads joiner_;

  typedef std::queue<FunctionWrapper> local_queue_type;
  static thread_local std::unique_ptr<local_queue_type> local_work_queue_;

  void RunPendingTask() {
    std::unique_ptr<FunctionWrapper> task;
    if (local_work_queue_ && !local_work_queue_->empty()) {
      task = std::move(local_work_queue_->front());
      local_work_queue_->pop();
      task();
    } else if (task = work_queue_.Pop()) {
      task();
    } else {
      std::this_thread::yield();
    }
  }

  void WorkThread() {
    local_work_queue_.reset(new local_queue_type);
    while (!done_) {
      RunPendingTask();
    }
  }
 public:
  ThreadPool() : done_(false), joiner_(threads_) {
    const unsigned thread_count = std::thread::hardware_concurrency();
    try {
      for (size_t i = 0; i < thread_count; ++i) {
        threads_.push_back(std::thread(&ThreadPool::WorkThread, this));
      }
    }
    catch (...) {
      done_ = true;
      throw;
    }
  }

  ~ThreadPool() {
    done_ = true;
  }

  template<class FunType>
  std::future<typename std::result_of<FunType()>::type> submit(const FunType &f) {
    typedef typename std::result_of<FunType()>::type result_type;
    std::packaged_task<result_type()> task(std::move(f));
    std::future<result_type> res(task.get_future());
    if (local_work_queue_) {
      local_work_queue_->push(std::move(task));
    } else {
      work_queue_.push(std::move(task));
    }
    return res;
  }

};

#endif /* ifndef THREADPOOL_H */

