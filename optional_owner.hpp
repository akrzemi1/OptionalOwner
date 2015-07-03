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

#if defined AK_TOOLBOX_NO_CXX11
#  define AK_TOOLBOX_NOEXCEPT
#  define AK_TOOLBOX_CONSTEXPR
#  define AK_TOOLBOX_EXPLICIT_CONV
#else
#  define AK_TOOLBOX_NOEXCEPT noexcept 
#  define AK_TOOLBOX_CONSTEXPR constexpr 
#  define AK_TOOLBOX_EXPLICIT_CONV explicit 
#endif


namespace ak_toolbox {

template <typename T, typename D = std::default_delete<T>>
class optional_owner
{
  std::unique_ptr<T, D> _ownership;
  T*                    _access;
  
public:

  AK_TOOLBOX_CONSTEXPR optional_owner() AK_TOOLBOX_NOEXCEPT
    : _ownership(), _access() {}
    
  optional_owner(std::unique_ptr<T, D> owing_ptr) 
    : _ownership{std::move(owing_ptr)}, _access{_ownership.get()} {}

  optional_owner(T& nonowing_ptr)
    : _ownership{}, _access{&nonowing_ptr} {}
    
  optional_owner(optional_owner&& other) AK_TOOLBOX_NOEXCEPT
    : _ownership{std::move(other._ownership)}, _access{other._access}
    { other._access = 0; }
    
  optional_owner& operator=(optional_owner&& other) AK_TOOLBOX_NOEXCEPT
    { _ownership = std::move(other._ownership);
      _access = other._access;
      other._access = 0;
    }
  
  AK_TOOLBOX_EXPLICIT_CONV operator bool () const { return _access != 0; }
  bool operator! () const { return _access == 0; }
  
  // precondition: *this has not been moved from
  T* operator->() const { assert(_access); return _access; }
  
  // precondition: *this has not been moved from
  T& operator*() const { assert(_access); return *_access; }
  
  T* get() const { return _access; }
  
  bool owes() const { return _ownership != 0; }
};

} // namespace ak_toolbox

#endif //AK_TOOLBOX_OPTIONAL_OWNER_HEADER_GUARD_
