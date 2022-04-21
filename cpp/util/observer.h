#include <util/logger.h>
#include <util/detail.h>

#include <tuple>
#include <algorithm>
#include <vector>



/*
At the very outset(at compile time), we have a list of classes that
need to be observed`in subject::_observedClasses
*/
template<typename C>
struct fieldEnum;

class observedClass1;
class observedClass2;

enum class fieldEnumClass1
{
  A,
  B
};

template<>
struct fieldEnum<observedClass1>
{
  using type = fieldEnumClass1;
};

enum class fieldEnumClass2
{
  C,
  OBJS
};


template<>
struct fieldEnum<observedClass2>
{
  using type = fieldEnumClass2;
};

enum eventType{
  create = 1 << 1,
  premodify = 1 << 2,
  postmodify =  1 << 3,
  destroy = 1 << 4
};


class observer;

class subject
{
  public:

  using oc1 = observedClass1;
  using oc2 = observedClass2;
  using of1 = typename fieldEnum<observedClass1>::type;
  using of2 = typename fieldEnum<observedClass2>::type;
  using classList = std::tuple<oc1, oc2>;
  // needed for notifying observers based on certain events
  using event = eventType;

  void subscribe(observer *obs) {
    _observers.push_back(obs);
  }

  void unSubscribe(observer *obs) {
    auto it = std::find(_observers.begin(), _observers.end(), obs);
    if (it != _observers.end()) {
      _observers.erase(it);
    }
  }

  template<typename C>
  void notify(event e, const C *ptr, typename fieldEnum<C>::type f);

  private:
    std::vector<observer *> _observers;
};


class observer
{
  public:
  using classList = subject::classList;
  using event = subject::event;
  using oc1 = subject::oc1;
  using oc2 = subject::oc2;
  using of1 = subject::of1;
  using of2 = subject::of2;

  observer() {
    disableAll();
  }
  
  virtual void postCreate(const oc1 *obj, of1 field) {}
  virtual void preModify(const oc1 *obj, of1 field) {}
  virtual void postModify(const oc1 *obj, of1 field) {}
  virtual void preDestroy(const oc1 *obj, of1 field) {}
  virtual void postCreate(const oc2 *obj, of1 field) {}
  virtual void preModify(const oc2 *obj, of2 field) {}
  virtual void postModify(const oc2 *obj, of2 field) {}
  virtual void preDestroy(const oc2 *obj, of2 field) {}

  template<typename T>
    // requires T to be part of classList
  bool isEnabled(event e) {
    size_t index = detail::tupleIndex<T, classList>::value;
    return _enableFlags[index] & e;
  }

  template<typename T>
    // requires T to be part of classList
  void enable(event e){
    size_t index = detail::tupleIndex<T, classList>::value;
    _enableFlags[index] |= e;
  }

  template<typename T>
    // requires T to be part of classList
  void disable(event e){
    size_t index = detail::tupleIndex<T, classList>::value;
    _enableFlags[index]  &= ~e;
  }

  void disableAll (){
    size_t n = std::tuple_size_v<classList>;
    for (int i = 0; i < n; ++i) {
      _enableFlags[0] = 0;
    }
  };

  private:
    std::array<int, std::tuple_size_v<classList>> _enableFlags;
};

//! @brief notifies the event to all the subscribed observers for
//!        which this event is enabled
//!
template<typename C>
void subject::notify(event e, const C *c, typename fieldEnum<C>::type f) {
  INFO("notify\n");
  for (auto observer : _observers) {
    if (observer->isEnabled<C>(e)) {
      INFO("observer\n");
      if (e == event::create) {
        observer->postCreate(c, f);
      } else if (e == event::premodify) {
        observer->preModify(c, f);
      } else if (e == event::postmodify) {
        observer->postModify(c, f);
      } else {
        observer->preDestroy(c, f);
      }
    }
  }
}

class notification
{
  public:
  notification() : _subject{new subject()} {}
  notification(const notification&) = delete;
  notification& operator=(const notification &) = delete; 
  ~notification() { delete _subject;}

  subject *getSubject() { return _subject;}
  private:
    subject *_subject;
};
