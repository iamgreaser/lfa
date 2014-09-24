#include "lfa.h"

// XXX: how much of this should be redesigned?

pa_threaded_mainloop *pa_threaded_mainloop_new(void)
{
	pa_mainloop *main = pa_mainloop_new();

	if(main == NULL)
		return NULL;

	pa_threaded_mainloop *m = malloc(sizeof(pa_threaded_mainloop));
	m->main = main;
	m->pthread = NULL;
	m->retval = 0;

	return m;
}

void pa_threaded_mainloop_free(pa_threaded_mainloop *m)
{
	pa_mainloop_free(m->main);
	free(m);
}

static void *lfa_threaded_main_run(void *userdata)
{
	pa_threaded_mainloop *m = (pa_threaded_mainloop *)userdata;

	pa_mainloop_run(m->main, &m->retval);

	return NULL;
}

int pa_threaded_mainloop_start(pa_threaded_mainloop *m)
{
	if(m->pthread != NULL) return 0;
	m->retval = 0;
	pthread_create(&m->pthread, NULL, lfa_threaded_main_run, m);

	return 0;
}

void pa_threaded_mainloop_stop(pa_threaded_mainloop *m)
{
	if(m->pthread == NULL) return;
	pthread_kill(m->pthread, SIGTERM);
}

void pa_threaded_mainloop_lock(pa_threaded_mainloop *m)
{
	// TODO!
}

void pa_threaded_mainloop_unlock(pa_threaded_mainloop *m)
{
	// TODO!
}

void pa_threaded_mainloop_signal(pa_threaded_mainloop *m, int wait_for_accept)
{
	// TODO!
}

void pa_threaded_mainloop_accept(pa_threaded_mainloop *m)
{
	// TODO!
}

void pa_threaded_mainloop_wait(pa_threaded_mainloop *m)
{
	// TODO!
}

pa_mainloop_api *pa_threaded_mainloop_get_api(pa_threaded_mainloop *m)
{
	return pa_mainloop_get_api(m->main);
}

