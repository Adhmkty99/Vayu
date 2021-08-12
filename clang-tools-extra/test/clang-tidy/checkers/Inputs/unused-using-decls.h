class MyClass {
public:
  template <template <typename> class S, typename T>
  S<T> *func1(T *a) {
    return new S<T>();
  }
  template <typename T, T (*S)()>
  void func2(T a) {
    S();
  }
};

QC1 *foo();

namespace Q2 {
class QC2 {};
} // namespace Q2
using Q2::QC2;
// not used QC2 should have warning only when .h file is selected with --header-filter
