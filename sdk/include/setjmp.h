// Copyright Microsoft and CHERIoT Contributors.
// SPDX-License-Identifier: MIT
#pragma once

/**
 * This is a minimal implementation of setjmp/longjmp.
 *
 * CHERIoT cannot store a `jmp_buf` anywhere other than the stack without
 * clearing tags (which will then cause `longjmp` to fail).
 */

#include <cdefs.h>
#include <stddef.h>
#include <stdint.h>

/**
 * Jump buffer for setjmp/longjmp.
 */
struct __jmp_buf
{
	uintptr_t __cs0;
	uintptr_t __cs1;
	uintptr_t __csp;
	uintptr_t __cra;
};

/**
 * C requires that `setjmp` and `longjmp` take a `jmp_buf` by reference and so
 * this ends up being defined as an array of one element, which allows it to
 * both be allocated and passed by reference.
 */
typedef struct __jmp_buf jmp_buf[1];

#include <setjmp-assembly.h>

__BEGIN_DECLS

/**
 * C `setjmp` function.  Returns (up to) twice.  First returns 0, returns a
 * value passed to `longjmp` on the second return.
 */
__attribute__((returns_twice)) __cheri_libcall int
setjmp(jmp_buf env) __asm__("setjmp");

/**
 * C `longjmp` function.  Does not return, jumps back to the `setjmp` call.
 */
__attribute__((__noreturn__)) __cheri_libcall void
longjmp(jmp_buf env, int val) __asm__("longjmp");

__END_DECLS
