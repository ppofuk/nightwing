#ifndef _OBSERVER_H_
#define _OBSERVER_H_
namespace nightwing {

class Observable; 

class Observer {
 public:
  virtual void Notify(Observable* observable, void* arg) = 0;
};

} // namespace nightwing
#endif /* _OBSERVER_H_ */
