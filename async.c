#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>

#include <fcntl.h>

#include <sys/soundcard.h>

#include "lfa.h"

#if 0
#define dprintf(...) fprintf(stderr, ...)
#else
#define dprintf(...)
#endif

pa_io_event *mainloop_io_new(pa_mainloop_api *a, int fd, int events, pa_io_event_cb_t cb, void *userdata)
{
	dprintf("mainloop_io_new: %p %i %08X %p %p\n", a, fd, events, cb, userdata);

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
	dprintf("pa_mainloop_new\n");

	pa_mainloop *m = malloc(sizeof(pa_mainloop));

	m->api = malloc(sizeof(pa_mainloop_api));
	memcpy(m->api, &mainloop_api, sizeof(pa_mainloop_api));
	m->api->m = m;
	m->c = NULL;

	return m;
}

void pa_mainloop_free(pa_mainloop *m)
{
	dprintf("pa_mainloop_free: %p\n", m);
	free(m);
}

pa_mainloop_api *pa_mainloop_get_api(pa_mainloop *m)
{
	dprintf("pa_mainloop_get_api: %p\n", m);

	return m->api;
}

pa_context *pa_context_new_with_proplist(pa_mainloop_api *mainloop, const char *name, pa_proplist *proplist)
{
	dprintf("pa_context_new_with_proplist: %p %s %p\n", mainloop, name, proplist);
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
	dprintf("pa_context_new: %p %s\n", mainloop, name);

	return pa_context_new_with_proplist(mainloop, name, NULL);
}

pa_operation *pa_context_set_name(pa_context *c, const char *name, pa_context_success_cb_t cb, void *userdata)
{
	dprintf("pa_context_set_name: %p %s %p %p\n", c, name, cb, userdata);
	pa_operation *o = malloc(sizeof(pa_operation));
	o->refs = 1;
	o->state = PA_OPERATION_DONE;
	return o;
}

int pa_context_connect(pa_context *c, const char *server, int flags, const pa_spawn_api *api)
{
	dprintf("pa_context_connect: %p %s %08X %p\n", c, server, flags, api);

	c->m->c = c;

	return 0; // yeah it worked
}

void pa_context_disconnect(pa_context *c)
{
	// TODO: clean up streams
	dprintf("pa_context_disconnect: %p\n", c);
	//free(c);
}

int pa_context_errno(pa_context *c)
{
	dprintf("pa_context_errno: %p\n", c);

	return 0;
}

pa_context *pa_context_ref(pa_context *c)
{
	dprintf("pa_context_ref: %p\n", c);

	c->refs++;

	return c;
}

void pa_context_unref(pa_context *c)
{
	dprintf("pa_context_unref: %p\n", c);

	c->refs--;

	if(c->refs == 0)
	{
		pa_context_disconnect(c);
		free(c);
	}
}

int pa_context_get_state(pa_context *c)
{
	dprintf("pa_context_get_state: %p\n", c);
	return PA_CONTEXT_READY;
}

void pa_context_set_state_callback(pa_context *c, pa_context_notify_cb_t cb, void *userdata)
{
	dprintf("pa_context_set_state_callback: %p %p %p\n", c, cb, userdata);
	c->state_cb = cb;
}

void pa_context_set_event_callback(pa_context *c, pa_context_event_cb_t cb, void *userdata)
{
	dprintf("pa_context_set_event_callback: %p %p %p\n", c, cb, userdata);
	// TODO.
}

size_t pa_stream_writable_size(pa_stream *s)
{
	dprintf("pa_stream_writable_size: %p\n", s);
	return (1<<20); // 1MB should be enough
}

int pa_stream_begin_write(pa_stream *s, void **data, size_t *nbytes)
{
	//dprintf("pa_stream_begin_write: %p %p %p\n", s, data, nbytes);

	s->buflen = *nbytes;
	s->bufdata = realloc(s->bufdata, *nbytes);
	*data = s->bufdata;

	return 0;
}

int pa_stream_write(pa_stream *s, const void *data, size_t nbytes, pa_free_cb_t free_cb, int64_t offset, int seekmode)
{
	//dprintf("pa_stream_write: %p %p %i %p %i %i\n", s, data, (int)nbytes, free_cb, (int)offset, seekmode);

	write(s->fd, data, nbytes);

	if(data == s->bufdata)
	{
		s->buflen = 0;
		s->bufdata = realloc(s->bufdata, 0);
	}

	return 0;
}

int pa_stream_cancel_write(pa_stream *s)
{
	dprintf("pa_stream_cancel_write: %p\n", s);

	s->buflen = 0;
	s->bufdata = realloc(s->bufdata, 0);

	return 0;
}

int pa_stream_get_state(pa_stream *s)
{
	dprintf("pa_stream_get_state: %p\n", s);
	return PA_STREAM_READY;
}

pa_operation *pa_stream_drain(pa_stream *s, pa_stream_success_cb_t cb, void *userdata)
{
	dprintf("pa_stream_drain: %p %p %p\n", s, cb, userdata);
	pa_operation *o = malloc(sizeof(pa_operation));
	o->refs = 1;
	o->state = PA_OPERATION_DONE;
	return o;
}

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

int pa_mainloop_iterate(pa_mainloop *m, int block, int *retval)
{
	dprintf("pa_mainloop_iterate: %p %i %p\n", m, block, retval);

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
	dprintf("pa_mainloop_run: %p %p\n", m, retval);

	int ret = 0;

	while((ret = pa_mainloop_iterate(m, 1, retval)) >= 0)
		;

	return ret;
}

pa_stream *pa_stream_new_with_proplist(pa_context *c, const char *name, const pa_sample_spec *ss, const pa_channel_map *map, pa_proplist *p)
{
	dprintf("pa_stream_new_with_proplist: %p %s %p %p %p\n", c, name, ss, map, p);

	pa_stream *s = malloc(sizeof(pa_stream));

	s->c = c;

	s->name = (name == NULL ? NULL : strdup(name));
	memcpy(&(s->ss), ss, sizeof(pa_sample_spec));
	s->bufdata = NULL;
	s->buflen = 0;
	s->refs = 1;

	s->event_cb = NULL;
	s->buffer_attr_cb = NULL;
	s->state_cb = NULL;
	s->started_cb = NULL;
	s->overflow_cb = NULL;
	s->underflow_cb = NULL;
	s->moved_cb = NULL;
	s->read_cb = NULL;
	s->write_cb = NULL;

	s->fire_connect = 0;
	s->started = 0;

	s->fd = -1;

	return s;
}

pa_stream *pa_stream_new(pa_context *c, const char *name, const pa_sample_spec *ss, const pa_channel_map *map)
{
	return pa_stream_new_with_proplist(c, name, ss, map, NULL);
}

int pa_stream_connect_playback(pa_stream *s, const char *dev, const pa_buffer_attr *attr, int flags, const pa_cvolume *volume, pa_stream *sync_stream)
{
	dprintf("pa_stream_connect_playback: %p %s %p %08X %p %p\n", s, dev, attr, flags, volume, sync_stream);
	if(attr != NULL)
		dprintf("\tattr: %i %i %i %i %i\n", attr->maxlength, attr->tlength, attr->prebuf, attr->minreq, attr->fragsize);

	s->fire_connect = 1;
	s->started = 1;

	int fmt;
	int stereo;

	/* check format spec */
	switch(s->ss.format)
	{
		case PA_SAMPLE_U8:
			fmt = AFMT_U8;
			break;
		case PA_SAMPLE_S16LE:
			fmt = AFMT_S16_LE;
			break;
		case PA_SAMPLE_S16BE:
			fmt = AFMT_S16_BE;
			break;
		default:
			/* seriously why the fuck would you want anything else you Vorpal */
			return -1;
	}

	switch(s->ss.channels)
	{
		case 1:
			stereo = 0;
			break;
		case 2:
			stereo = 1;
			break;
		default:
			/* i'd like to just keep this simpleish */
			return -1;
	}

	s->fd = open("/dev/dsp", O_WRONLY);
	ioctl(s->fd, SNDCTL_DSP_SPEED, &(s->ss.rate));
	ioctl(s->fd, SNDCTL_DSP_SETFMT, &fmt);
	ioctl(s->fd, SNDCTL_DSP_STEREO, &stereo);
	s->c->s = s;

	return 0;
}

int pa_stream_disconnect(pa_stream *s)
{
	dprintf("pa_stream_disconnect: %p\n", s);

	if(s == NULL)
		return -1;

	if(s->fd == -1)
		return 0;

	close(s->fd);
	s->fd = -1;
	s->fire_connect = 0;
	s->started = 0;

	return 0;
}

pa_stream *pa_stream_ref(pa_stream *s)
{
	dprintf("pa_stream_ref: %p\n", s);

	s->refs++;

	return s;
}

void pa_stream_unref(pa_stream *s)
{
	dprintf("pa_stream_unref: %p\n", s);

	s->refs--;

	if(s->refs == 0)
	{
		pa_stream_disconnect(s);
		free(s);
	}
}

void pa_stream_set_event_callback(pa_stream *s, pa_stream_event_cb_t cb, void *userdata)
{
	dprintf("pa_stream_set_event_callback: %p %p %p\n", s, cb, userdata);
	s->event_cb = cb;
}

void pa_stream_set_buffer_attr_callback(pa_stream *s, pa_stream_notify_cb_t cb, void *userdata)
{
	dprintf("pa_stream_set_buffer_attr_callback: %p %p %p\n", s, cb, userdata);
	s->buffer_attr_cb = cb;
}

void pa_stream_set_state_callback(pa_stream *s, pa_stream_notify_cb_t cb, void *userdata)
{
	dprintf("pa_stream_set_state_callback: %p %p %p\n", s, cb, userdata);
	s->state_cb = cb;
}

void pa_stream_set_started_callback(pa_stream *s, pa_stream_notify_cb_t cb, void *userdata)
{
	dprintf("pa_stream_set_started_callback: %p %p %p\n", s, cb, userdata);
	s->started_cb = cb;
}

void pa_stream_set_overflow_callback(pa_stream *s, pa_stream_notify_cb_t cb, void *userdata)
{
	dprintf("pa_stream_set_overflow_callback: %p %p %p\n", s, cb, userdata);
	s->overflow_cb = cb;
}

void pa_stream_set_underflow_callback(pa_stream *s, pa_stream_notify_cb_t cb, void *userdata)
{
	dprintf("pa_stream_set_underflow_callback: %p %p %p\n", s, cb, userdata);
	s->underflow_cb = cb;
}

void pa_stream_set_moved_callback(pa_stream *s, pa_stream_notify_cb_t cb, void *userdata)
{
	dprintf("pa_stream_set_moved_callback: %p %p %p\n", s, cb, userdata);
	s->moved_cb = cb;
}

void pa_stream_set_suspended_callback(pa_stream *s, pa_stream_notify_cb_t cb, void *userdata)
{
	dprintf("pa_stream_set_suspended_callback: %p %p %p\n", s, cb, userdata);
	s->suspended_cb = cb;
}

void pa_stream_set_read_callback(pa_stream *s, pa_stream_request_cb_t cb, void *userdata)
{
	dprintf("pa_stream_set_read_callback: %p %p %p\n", s, cb, userdata);
	s->read_cb = cb;
}

void pa_stream_set_write_callback(pa_stream *s, pa_stream_request_cb_t cb, void *userdata)
{
	dprintf("pa_stream_set_write_callback: %p %p %p\n", s, cb, userdata);
	s->write_cb = cb;
}

pa_channel_map *pa_channel_map_init_auto(pa_channel_map *m, unsigned channels, int def)
{
	int i;

	// TODO: not ignore def

	if(channels == 0 || channels > PA_CHANNELS_MAX)
		return NULL;

	m->channels = channels;
	// Most of this pain would have been avoided if you'd used a vector, Lennart.
	switch(channels)
	{
		case 1:
			m->map[0] = PA_CHANNEL_POSITION_MONO;
			break;

		case 2:
			m->map[0] = PA_CHANNEL_POSITION_FRONT_LEFT;
			m->map[1] = PA_CHANNEL_POSITION_FRONT_RIGHT;
			break;

		case 3:
			m->map[0] = PA_CHANNEL_POSITION_FRONT_LEFT;
			m->map[1] = PA_CHANNEL_POSITION_FRONT_RIGHT;
			m->map[2] = PA_CHANNEL_POSITION_REAR_CENTER;
			break;

		case 4:
			m->map[0] = PA_CHANNEL_POSITION_FRONT_LEFT;
			m->map[1] = PA_CHANNEL_POSITION_FRONT_RIGHT;
			m->map[2] = PA_CHANNEL_POSITION_REAR_LEFT;
			m->map[3] = PA_CHANNEL_POSITION_REAR_RIGHT;
			break;

		case 5:
			m->map[0] = PA_CHANNEL_POSITION_FRONT_LEFT;
			m->map[1] = PA_CHANNEL_POSITION_FRONT_RIGHT;
			m->map[2] = PA_CHANNEL_POSITION_REAR_LEFT;
			m->map[3] = PA_CHANNEL_POSITION_REAR_RIGHT;
			m->map[4] = PA_CHANNEL_POSITION_FRONT_CENTER;
			break;

		default:
		case 6:
			m->map[0] = PA_CHANNEL_POSITION_FRONT_LEFT;
			m->map[1] = PA_CHANNEL_POSITION_FRONT_RIGHT;
			m->map[2] = PA_CHANNEL_POSITION_REAR_LEFT;
			m->map[3] = PA_CHANNEL_POSITION_REAR_RIGHT;
			m->map[4] = PA_CHANNEL_POSITION_FRONT_CENTER;
			m->map[5] = PA_CHANNEL_POSITION_LFE; // I assume this is the Low Frequency Emitter.

			for(i = 6; i < channels; i++)
				m->map[i] = PA_CHANNEL_POSITION_AUX0 + i;
			break;
	}

	return m;
}

const char *pa_get_library_version(void)
{
	dprintf("pa_get_library_version\n");
	return "5.0.0";
}

const char *pa_strerror(int error)
{
	dprintf("pa_strerror\n");
	return "IT BROKE";
}


