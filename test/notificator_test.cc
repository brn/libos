#include <string.h>
#include <stdio.h>
#include <deque>
#include <gtest/gtest.h>
#include "../include/type_convertor.h"
#include "../include/lib/bind.h"
#include "../include/logging.h"
#include "../include/notificator/notificator.h"
class Event {
 public :
  Event() : type_("test event"){}
  ~Event(){}
  const char* type() const {return type_;}
 private :
  const char* type_;
};

class TestListener1 {
 public :
  void operator()(Event e) {
    ASSERT_EQ(0, strcmp(e.type(), "test event"));
  }
};

class TestListener2 {
 public :
  void operator()(Event e) {
    ASSERT_EQ(0, strcmp(e.type(), "test event"));
  }
};


class TestListener3 {
 public :
  void operator()(Event e) {
    ASSERT_EQ(0, strcmp(e.type(), "test event"));
  }
};


class TestListener4 {
 public :
  void operator()(Event e) {
    ASSERT_EQ(0, strcmp(e.type(), "test event"));
  }
};


class TestMemListener {
 public :
  void Reciever(Event e) {
    ASSERT_EQ(0, strcmp(e.type(), "test event"));
  }
};

void Testfn1(Event e) {
  ASSERT_EQ(0, strcmp(e.type(), "test event"));
}

void Testfn2(Event e) {
  ASSERT_EQ(0, strcmp(e.type(), "test event"));
}


TEST(NotificatorTest, FunctorTest) {
  os::Logging::Initialize(stdout);
  os::Notificator<Event> notificator;
  TestListener1 t;
  notificator.AddListener("TestListener", &t);
  notificator.AddListener("TestListener", TestListener2());
  notificator.AddListener("TestListener", TestListener3());
  notificator.NotifyForKey("TestListener", Event());
  notificator.NotifyAll(Event());
}

TEST(NotificatorTest, FunctionPtrTest) {
  os::Notificator<Event> notificator;
  notificator.AddListener("TestListener", Testfn1);
  notificator.AddListener("TestListener", Testfn2);
  notificator.NotifyForKey("TestListenerFn", Event());
  notificator.NotifyAll(Event());
}

TEST(NotificatorTest, MemberFunctionPtrTest) {
  os::Notificator<Event> notificator;
  TestMemListener mem;
  notificator.AddListener("TestListener", os::bind(&TestMemListener::Reciever, &mem, _1));
  notificator.NotifyForKey("TestListener", Event());
  notificator.NotifyAll(Event());
}


TEST(NotificatorTest, TooManyListenerTest) {
  os::Notificator<Event> notificator;
  for (int i = 0; i < 10000; i++) {
    char tmp[100];
    sprintf(tmp , "TestListener%d", i);
    notificator.AddListener(tmp, TestListener4());
    notificator.AddListener("TestListener", TestListener4());
  }
  notificator.NotifyForKey("TestListener", Event());
  notificator.NotifyAll(Event());
}

TEST(SimpleNotificatorTest, TooManyListenerTest) {
  os::SimpleNotificator<Event> notificator;
  for (int i = 0; i < 1000000; i++) {
    TestListener4 test = TestListener4();
    notificator.AddListener(test);
  }
  notificator.Notify(Event());
}

TEST(SimpleNotificatorTest, TooManyListenerTest2) {
  typedef os::SimpleNotificator<Event>::ListenerHandle LHandle;
  os::SimpleNotificator<Event, std::list<LHandle> > notificator;
  for (int i = 0; i < 1000000; i++) {
    TestListener4 test = TestListener4();
    notificator.AddListener(test);
  }
  notificator.Notify(Event());
}

TEST(SimpleNotificatorTest, TooManyListenerTest3) {
  typedef os::SimpleNotificator<Event>::ListenerHandle LHandle;
  os::SimpleNotificator<Event, std::deque<LHandle> > notificator;
  for (int i = 0; i < 1000000; i++) {
    TestListener4 test = TestListener4();
    notificator.AddListener(test);
  }
  notificator.Notify(Event());
}
