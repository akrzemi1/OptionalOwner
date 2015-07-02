// Copyright (C) 2015, Andrzej Krzemienski.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef AK_TOOLBOX_OPTIONAL_OWNER_HEADER_GUARD_
#define AK_TOOLBOX_OPTIONAL_OWNER_HEADER_GUARD_

#include <cassert>
#include <memory>
#include <utility>

namespace ak_toolbox {

template <typename T, typename D = std::default_delete<T>>
class optional_owner
{
  std::unique_ptr<T, D> _ownership;
  T*                    _access;
  
public:
  // precondition: owing_ptr != nullptr
  optional_owner(std::unique_ptr<T, D> owing_ptr) 
    : _ownership{std::move(owing_ptr)}, _access{_ownership.get()}
    { assert(_access); }

  optional_owner(T& nonowing_ptr)
    : _ownership{}, _access{&nonowing_ptr} {}
  
  // precondition: *this has not been moved from
  const T& get() const { assert(_access); return *_access; }
  T& get() { assert(_access); return *_access; }
  
  // precondition: *this has not been moved from
  bool owes() const { assert(_access); return _ownership != 0; }
};

} // namespace ak_toolbox

#endif //AK_TOOLBOX_OPTIONAL_OWNER_HEADER_GUARD_
