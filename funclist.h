#ifndef FUNCLIST_H
#define FUNCLIST_H

#include <functional>
#include <sstream>

namespace flist {

  namespace detail {
    auto of_range_helper(auto &r, auto it, auto f, auto a) {
      if (it == std::end(r)) {
        return a;
      }
      return f(*it, of_range_helper(r, std::next(it), f, a));
    }
  }  // namespace detail

  // Empty list
  auto empty = [](auto f, auto a) {
    (void)f;  // Casting in order to avoid unused parameter warning.
    return a;
  };

  // Adds element x to the beginning of list l.
  auto cons = [](auto x, auto l) {
    return [=](auto f, auto a) {
      return f(x, l(f, a));
    };
  };

  // Creates a list from given elements.
  auto create = [](auto... args) {
    return [=](auto f, auto a) {
      (..., (a = f(args, a)));
      return a;
    };
  };

  // Creates a list of elements in r.
  auto of_range = [](auto r) {
    using unwrapped = typename std::unwrap_reference<decltype(r)>::type;
    unwrapped r_ = r;

    return [=](auto f, auto a) {
      auto it = std::begin(r_);
      if (it == std::end(r_)) {
        return a;
      }
      return f(*it, detail::of_range_helper(r_, std::next(it), f, a));
    };
  };

  // Concatenates two lists.
  auto concat = [](auto l, auto k) {
    return [=](auto f, auto a) {
      return l(f, k(f, a));
    };
  };

  // Return reversed list.
  auto rev = [](auto l) {
    return [=](auto f, auto a) {
      using A = decltype(a);
      using func_A_A = std::function<A(A)>;
      return l(
          [=](auto x, func_A_A g) {
            return static_cast<func_A_A>(
                [=](A a) { return g(f(x, a)); });
          },
          static_cast<func_A_A>([=](A a) -> A { return a; }))
          (a);
    };
  };

  // Applies function m to each element of the list.
  auto map = [](auto m, auto l) {
    return [=](auto f, auto a_to_be_evaluated) {
      return l(
        [=](auto x, auto a) { return f(m(x), a); }
        , a_to_be_evaluated);
    };
  };

  // Returns list with only those elements that satisfy the predicate.
  auto filter = [](auto p, auto l) {
    return [=](auto f, auto a) {
      return l(
        [=](auto x, auto a) { return p(x) ? f(x, a) : a; }
        , a);
    };
  };

  // Flattens the list.
  auto flatten = [](auto l) {
    return [=](auto f, auto a) {
      return l([=](auto x, auto a) { return x(f, a); }, a);
    };
  };

  // Returns list as string.
  auto as_string = [](const auto &l) {
    std::ostringstream os;
    os << "[";
    bool is_first = true;
    auto prt = [&](auto &x, auto a) {
      if (!is_first) os << ", ";
      is_first = false;
      os << x;
      return a;
    };
    auto l_rev = rev(l);
    l_rev(prt, NULL);
    os << "]";
    return os.str();
  };

}  // namespace flist

#endif  // FUNCLIST_H