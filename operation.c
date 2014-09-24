#include "lfa.h"

pa_operation *pa_operation_ref(pa_operation *o)
{
	if(o == NULL)
		return NULL;

	o->refs++;

	return o;
}

void pa_operation_unref(pa_operation *o)
{
	o->refs--;

	if(o->refs == 0)
		free(o);
}

void pa_operation_cancel(pa_operation *o)
{
	o->state = PA_OPERATION_CANCELLED;
}

int pa_operation_get_state(pa_operation *o)
{
	return o->state;
}

