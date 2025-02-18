#include <setjmp.h>

__attribute__((returns_twice, naked)) int setjmp(jmp_buf env)
{
	__asm__("	csc	cs0, 0(ca0)\n"
	        "	csc	cs1, 8(ca0)\n"
	        "	csc	csp, 16(ca0)\n"
	        "	csc	cra, 24(ca0)\n"
	        "	li	a0, 0\n"
	        "	cret\n");
}

__attribute__((__noreturn__, naked)) void longjmp(jmp_buf env, int val)
{
	__asm__("	clc	cs0, 0(ca0)\n"
	        "	clc	cs1, 8(ca0)\n"
	        "	clc	csp, 16(ca0)\n"
	        "	clc	cra, 24(ca0)\n"
	        "	mv	a0, a1\n"
	        "	cjr	cra\n");
}