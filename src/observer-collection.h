#ifndef NIGHTWING_OBSERVER_COLLECTION_H_
#define NIGHTWING_OBSERVER_COLLECTION_H_

// Wasn't tested.
// Wasn't even compiled! :D

#include "build.h"
#include "type-helpers.h"
#include <string.h>  // size_t

namespace nightwing {

template <typename ObserverType>
class ObserverCollection {
 public:
  ObserverCollection() : size_(0) {}

  void Add(ObserverType* observer) {
    bool not_collected = true;
    for (size_t i = 0; i < size_ && not_collected; ++i) {
      if (collection_[i] == observer) {
        not_collected = false;
      }
    }

    if (not_collected) collection_[size_++] = observer;
  }

  void Remove(ObserverType* observer) {
    for (size_t i = 0; i < size_; ++i)
      if (collection_[i] == observer) {
        if (i != size_ - 1) collection_[i] = collection_[size_ - 1];
        size_--;
      }
  }

  ObserverType* Get(size_t i) { return collection_[i]; }

  size_t get_size() const { return size_; }

 private:
  ObserverType* collection_[OBSERVABLE_COLLECTION_SIZE];
  size_t size_;
};

#define NIGHTWING_FOR_EACH_OBSERVER(collection, func)  \
  do {                                                 \
    for (size_t i = 0; i < collection.get_size(); ++i) \
        collection->Get(i)->func;                      \
  } while (0)

}  // namespace nightwing
#endif  // NIGHTWING_OBSERVER_COLLECTION_H_
