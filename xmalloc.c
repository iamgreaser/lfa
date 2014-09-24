#include "lfa.h"

void *pa_xmalloc(size_t l)
{
	return malloc(l);
}

void *pa_xmalloc0(size_t l)
{
	return calloc(1, l);
}

void *pa_xrealloc(void *p, size_t size)
{
	return realloc(p, size);
}

void pa_xfree(void *p)
{
	free(p);
}

