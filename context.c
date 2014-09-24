#include "lfa.h"

pa_context *pa_context_new_with_proplist(pa_mainloop_api *mainloop, const char *name, pa_proplist *proplist)
{
	dprintf(stderr, "pa_context_new_with_proplist: %p %s %p\n", mainloop, name, proplist);
	fprintf(stderr, "pa_context_new_with_proplist wrapper, proudly served by LFA\n");

	pa_context *c = malloc(sizeof(pa_context));

	c->m = mainloop->m;
	c->s = NULL;
	c->state_cb = NULL;
	c->refs = 1;

	return c;
}

pa_context *pa_context_new(pa_mainloop_api *mainloop, const char *name)
{
	dprintf(stderr, "pa_context_new: %p %s\n", mainloop, name);

	return pa_context_new_with_proplist(mainloop, name, NULL);
}

pa_operation *pa_context_set_name(pa_context *c, const char *name, pa_context_success_cb_t cb, void *userdata)
{
	dprintf(stderr, "pa_context_set_name: %p %s %p %p\n", c, name, cb, userdata);
	pa_operation *o = malloc(sizeof(pa_operation));
	o->refs = 1;
	o->state = PA_OPERATION_DONE;
	return o;
}

int pa_context_connect(pa_context *c, const char *server, int flags, const pa_spawn_api *api)
{
	dprintf(stderr, "pa_context_connect: %p %s %08X %p\n", c, server, flags, api);

	c->m->c = c;

	return 0; // yeah it worked
}

void pa_context_disconnect(pa_context *c)
{
	// TODO: clean up streams
	dprintf(stderr, "pa_context_disconnect: %p\n", c);
	//free(c);
}

int pa_context_errno(pa_context *c)
{
	dprintf(stderr, "pa_context_errno: %p\n", c);

	return 0;
}

pa_context *pa_context_ref(pa_context *c)
{
	dprintf(stderr, "pa_context_ref: %p\n", c);

	c->refs++;

	return c;
}

void pa_context_unref(pa_context *c)
{
	dprintf(stderr, "pa_context_unref: %p\n", c);

	c->refs--;

	if(c->refs == 0)
	{
		pa_context_disconnect(c);
		free(c);
	}
}

int pa_context_get_state(pa_context *c)
{
	dprintf(stderr, "pa_context_get_state: %p\n", c);
	return PA_CONTEXT_READY;
}

void pa_context_set_state_callback(pa_context *c, pa_context_notify_cb_t cb, void *userdata)
{
	dprintf(stderr, "pa_context_set_state_callback: %p %p %p\n", c, cb, userdata);
	c->state_cb = cb;
}

void pa_context_set_event_callback(pa_context *c, pa_context_event_cb_t cb, void *userdata)
{
	dprintf(stderr, "pa_context_set_event_callback: %p %p %p\n", c, cb, userdata);
	// TODO.
}

