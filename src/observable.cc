#include "observable.h"
namespace nightwing {

Observable::Observable() : collection_size_(0) {
}

void Observable::Register(Observer* observer) {
  bool not_collected = true; 
  for (size_t i = 0; i < collection_size_ && not_collected; ++i) {
    if (collection_[i] == observer) {
      not_collected = false;
    }
  }
  
  if (not_collected) 
    collection_[collection_size_++] = observer;
}

void Observable::Unregister(Observer* observer) {
  for (size_t i = 0; i < collection_size_; ++i) 
    if (collection_[i] == observer) {
      if (i != collection_size_ - 1)
        collection_[i] = collection_[collection_size_];
      collection_size_--;
    }
}

void Observable::Broadcast(void *arg) {
  for (size_t i = 0; i < collection_size_; ++i)
    collection_[i]->Notify(arg);
}

} // namespace nightwing
