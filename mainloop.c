#include "lfa.h"

pa_io_event *mainloop_io_new(pa_mainloop_api *a, int fd, int events, pa_io_event_cb_t cb, void *userdata)
{
	dprintf(stderr, "mainloop_io_new: %p %i %08X %p %p\n", a, fd, events, cb, userdata);

	pa_io_event *ret = malloc(sizeof(pa_io_event));

	return ret;
}

void mainloop_io_enable(pa_io_event *e, int events)
{
	return;
}

void mainloop_io_free(pa_io_event *e)
{
	return;
}

void mainloop_io_set_destroy(pa_io_event *e, pa_io_event_destroy_cb_t cb)
{
	return;
}

pa_time_event *mainloop_time_new(pa_mainloop_api *a, const struct timeval *tv, pa_time_event_cb_t cb, void *userdata)
{
	return NULL;
}

void mainloop_time_restart(pa_time_event *e, const struct timeval *tv)
{
	return;
}

void mainloop_time_free(pa_time_event *e)
{
	return;
}

void mainloop_time_set_destroy(pa_time_event *e, pa_time_event_destroy_cb_t cb)
{
	return;
}

pa_defer_event *mainloop_defer_new(pa_mainloop_api *a, pa_defer_event_cb_t cb, void *userdata)
{
	return NULL;
}

void mainloop_defer_enable(pa_defer_event *e, int b)
{
	return;
}

void mainloop_defer_free(pa_defer_event *e)
{
	return;
}

void mainloop_defer_set_destroy(pa_defer_event *e, pa_defer_event_destroy_cb_t cb)
{
	return;
}

void mainloop_quit(pa_mainloop_api *a, int retval)
{
	return;
}

pa_mainloop_api mainloop_api = {
	.userdata = NULL,
	.io_new = mainloop_io_new,
	.io_enable = mainloop_io_enable,
	.io_free = mainloop_io_free,
	.io_set_destroy = mainloop_io_set_destroy,
	.time_new = mainloop_time_new,
	.time_restart = mainloop_time_restart,
	.time_free = mainloop_time_free,
	.time_set_destroy = mainloop_time_set_destroy,
	.defer_new = mainloop_defer_new,
	.defer_enable = mainloop_defer_enable,
	.defer_free = mainloop_defer_free,
	.defer_set_destroy = mainloop_defer_set_destroy,
	.quit = mainloop_quit,
};

pa_mainloop *pa_mainloop_new(void)
{
	dprintf(stderr, "pa_mainloop_new\n");

	pa_mainloop *m = malloc(sizeof(pa_mainloop));

	m->api = malloc(sizeof(pa_mainloop_api));
	memcpy(m->api, &mainloop_api, sizeof(pa_mainloop_api));
	m->api->m = m;
	m->c = NULL;

	return m;
}

void pa_mainloop_free(pa_mainloop *m)
{
	dprintf(stderr, "pa_mainloop_free: %p\n", m);
	free(m);
}

pa_mainloop_api *pa_mainloop_get_api(pa_mainloop *m)
{
	dprintf(stderr, "pa_mainloop_get_api: %p\n", m);

	return m->api;
}

int pa_mainloop_iterate(pa_mainloop *m, int block, int *retval)
{
	dprintf(stderr, "pa_mainloop_iterate: %p %i %p\n", m, block, retval);

	pa_context *c = m->c;

	if(c->state_cb != NULL)
	{
		c->state_cb(c, NULL);
		c->state_cb = NULL;
	}

	if(c->s != NULL && c->s->started && c->s->write_cb != NULL)
	{
		c->s->write_cb(c->s, 1024, NULL);
	}

	return 0;
}

int pa_mainloop_run(pa_mainloop *m, int *retval)
{
	dprintf(stderr, "pa_mainloop_run: %p %p\n", m, retval);

	int ret = 0;

	while((ret = pa_mainloop_iterate(m, 1, retval)) >= 0)
		;

	if(retval != NULL)
		*retval = ret;

	return ret;
}

