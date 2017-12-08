#ifndef NONCOPYALBE_H
#define NONCOPYALBE_H

class noncopyable {
 protected:
  noncopyable() = default;
  ~noncopyable() = default;

  noncopyable(const noncopyable &) = delete;
  noncopyable &operator =(const noncopyable &) = delete;
};

#endif /* ifndef NONCOPYALBE_H */

