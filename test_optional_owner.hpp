// Copyright (C) 2015, Andrzej Krzemienski.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "optional_owner.hpp"
#include <cassert>
#include <utility>

using namespace ak_toolbox;

template <typename T>
void ignore(T&&) {}

struct oracle
{
  static int counter;
  int value;
  explicit oracle(int i = 0) : value(i) {}
  ~oracle() { ++counter; }
};
int oracle::counter = 0;

void test_oracle()
{
  oracle::counter = 0;
  {
    oracle o (1);
    ignore(o);
  }
  assert (oracle::counter == 1);
}

void test_nonowing_usge()
{
  oracle::counter = 0;
  oracle o (1);
  {
    optional_owner<oracle> ooo = o;
    assert (oracle::counter == 0);
    assert (!ooo.owes());
    assert (ooo.get().value == 1);
    
    optional_owner<oracle> oo2 = std::move(ooo);
    assert (oracle::counter == 0);
    assert (!oo2.owes());
    assert (oo2.get().value == 1);
    
    ooo = std::move(oo2);
    assert (oracle::counter == 0);
    assert (!ooo.owes());
    assert (ooo.get().value == 1);
    
    ooo.get().value = 2;
  }
  assert (oracle::counter == 0);
  assert (o.value == 2);
}

void test_owing_usage()
{
  oracle::counter = 0;
  {
    optional_owner<oracle> ooo = std::unique_ptr<oracle>(new oracle(3));
    assert (oracle::counter == 0);
    assert (ooo.owes());
    assert (ooo.get().value == 3);
    
    ooo.get().value = 4;
    optional_owner<oracle> oo2 = std::move(ooo);
    assert (oracle::counter == 0);
    assert (oo2.owes());
    assert (oo2.get().value == 4);
    
    oo2.get().value = 5;
    ooo = std::move(oo2);
    assert (oracle::counter == 0);
    assert (ooo.owes());
    assert (ooo.get().value == 5);
    
    {
      optional_owner<oracle> oo3 = ooo.get();
      assert (oracle::counter == 0);
      assert (!oo3.owes());
      assert (ooo.owes());
      oo3.get().value = 6;
    }
    assert (ooo.get().value == 6);
  }
  assert (oracle::counter == 1);
}

int main()
{
  test_nonowing_usge();
  test_owing_usage();
}
