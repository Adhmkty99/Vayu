class ThreadId {
public:
  ThreadId(const ThreadId &) {}
  ThreadId(ThreadId &&) {}
};

struct A {
  A(const ThreadId &tid) : threadid(tid) {}
  ThreadId threadid;
};

struct Movable {
  int a, b, c;
  Movable() = default;
  Movable(const Movable &) {}
  Movable(Movable &&) {}
};
