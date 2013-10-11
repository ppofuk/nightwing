#ifndef _OBSERVABLE_H_
#define _OBSERVABLE_H_

#include "build.h"
#include "observer.h"

#include <string.h>

namespace nightwing {

// TODO: class description goes here.
class Observable {
 public:
  Observable();

  virtual void Register(Observer* observer);
  virtual void Unregister(Observer* observer);
  virtual void Broadcast(void *arg = 0);

  // Getters
  size_t get_collection_size() { return collection_size_; }

 protected:
  Observer* collection_[OBSERVABLE_COLLECTION_SIZE];
  size_t collection_size_; 
};


} // namespace nightwing


#endif /* _OBSERVABLE_H_ */
