template <class T>
class C {
public:
  template <class R>
  explicit C(const R r, int *x = 0) : p(x) {}

private:
  int *p;
};
