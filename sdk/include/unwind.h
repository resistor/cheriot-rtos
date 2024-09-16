#pragma once
#include <cdefs.h>
#include <setjmp.h>

/**
 * On-stack linked list of cleanup handlers.
 */
struct CleanupList
{
	/// Next pointer.
	CleanupList *next;
	/// Jump buffer to return to.
	__jmp_buf env;
};

/**
 * Head of the cleanup list.
 *
 * This is stored in the space that the switcher reserves at the top of the
 * stack.  The stack is zeroed on entry to a compartment and so this will be
 * null until explicitly written to.
 */
__always_inline static inline struct CleanupList **cleanup_list_head()
{
	void     *csp = __builtin_cheri_stack_get();
	ptraddr_t top =
	  __builtin_cheri_base_get(csp) + __builtin_cheri_length_get(csp);
	csp = __builtin_cheri_address_set(csp, top - 8);
	return (struct CleanupList **)csp;
}

/**
 * Unwind the stack to the most recent `CHERIOT_HANDLER` block.
 */
__always_inline static inline void cleanup_unwind(void)
{
	CleanupList **__head = cleanup_list_head();
	CleanupList  *__top  = *__head;
	*__head              = __top->next;
	longjmp(&__top->env, 1);
}

/**
 * Simple error handling macros.  These are modelled on the OpenStep exception
 * macros and are similarly built on top of `setjmp`.  Code between
 * `CHERIOT_DURING` and `CHERIOT_HANDLER` corresponds to a `try` block.  Code
 * between `CHERIOT_HANDLER` and `CHERIOT_END_HANDLER` corresponds to a `catch`
 * block, though no exception value is actually thrown.
 *
 * Any automatic-storage values accessed in both blocks must be declared
 * `volatile`.
 */
#define CHERIOT_DURING                                                         \
	{                                                                          \
		CleanupList cleanupListEntry;                                          \
		auto      **__head    = cleanup_list_head();                           \
		cleanupListEntry.next = *__head;                                       \
		*__head               = &cleanupListEntry;                             \
		if (setjmp(&cleanupListEntry.env) == 0)                                \
		{
/// See CHERIOT_DURING.
#define CHERIOT_HANDLER                                                        \
	*__head = cleanupListEntry.next;                                           \
	}                                                                          \
	else                                                                       \
	{                                                                          \
		*__head = cleanupListEntry.next;

/// See CHERIOT_DURING.
#define CHERIOT_END_HANDLER                                                    \
	}                                                                          \
	}

#ifdef __cplusplus

/**
 * On-error helper.  Invokes `fn` and, if `cleanup_unwind` is called, invokes
 * `err`.  Destructors in between `fn` and the frame that calls
 * `cleanup_unwind` are not called, but this function returns normally and so
 * destructors of objects above this on the stack will be called normally.
 */
void on_error(auto fn, auto err)
{
	CHERIOT_DURING
	fn();
	CHERIOT_HANDLER
	err();
	CHERIOT_END_HANDLER
}

/**
 * On-error helper with no error handler (returns normally from forced unwind).
 */
void on_error(auto fn)
{
	on_error(fn, []() {});
}
#endif