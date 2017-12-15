#ifndef SINGLETON_H
#define SINGLETON_H

#include "noncopyable.hpp"

template<class T>
class Singleton : public noncopyable {
 public:
  static T &GetInstance() {
    static T f;
    return f;
  }

};

#endif /* ifndef SINGLETON_H */

