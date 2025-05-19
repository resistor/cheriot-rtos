-- Copyright Microsoft and CHERIoT Contributors.
-- SPDX-License-Identifier: MIT

debugOption("cpp_string")

library("cpp_string")
  set_default(false)
  add_rules("cheriot.component-debug")
  add_defines("LIBCXX_BUILDING_LIBCXXABI")
  add_defines("LIBC_NAMESPACE=__llvm_libc_common_utils")
  add_defines("_LIBCPP_BUILDING_LIBRARY")
  add_defines("_LIBCPP_BUILD_STATIC")
  add_defines("_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER")
  add_defines("_LIBCPP_REMOVE_TRANSITIVE_INCLUDES")
  add_defines("__STDC_CONSTANT_MACROS")
  add_defines("__STDC_FORMAT_MACROS")
  add_defines("__STDC_LIMIT_MACROS")
  add_files("string.cpp")
