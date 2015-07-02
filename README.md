# OptionalOwner
A pointer-like handle that may or may not own the memory it points to.

```c++
struct Iface { ~Iface(); };
struct Impl : Iface {};
std::unique_ptr<Impl> make_iface();

Impl i;
optional_owner<Iface> ni = i;            // non-owing pointer to i
optional_owner<Iface> oi = make_iface(); // oi owns the pointer (will delete it)
```

## Requirements
This is a single-header, header-only library. It requires the C++ compiler taht supports rvalue references, move semantics, `std::move` and `std::unique_ptr`. A C++11 compiler meets the requirements, but even some pre-C++11 compilers are sufficient.

## Overview

Class template `optional_owner` has no default constructor. You initialize it by either providing a reference or a `unique_ptr`. In the former case the `optional_owner` doesn't own the resource: it is just like a raw pointer or a reference. In the latter case, it does own the pointer, and will call the appropriate deleter when its life-time ends.

You get to the pointed-to object by calling member function `get()`. Unlike for (smart or not) pointers, it propagates `const`-ness to the pointed-to object.

`optional_owner` in moveable. The moved-from `optional_owner` can be only destroyed or move-assigned to.

The size of type `optional_owner<T>` is that of two pointers.

## What for?

It is occasionally needed to replace `std::shared_ptr` in the cases where there is no real need for the shared ownership between objects of unsequenced life-times, but the design makes it impossible to use better alternatives, like `std::unique_ptr`.

When considering heap memory management issues, it is recommended that the programmer first tries to use `std::unique_ptr`, if it is impossible then she should try to use this library, and only if this is impossible also, should she resort to using `std::shared_ptr`.
