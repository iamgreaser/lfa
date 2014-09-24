#include "lfa.h"

size_t pa_stream_readable_size(pa_stream *s)
{
	dprintf("pa_stream_readable_size: %p\n", s);
	return (1<<20); // 1MB should be enough
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

	// XXX: handle offset?
	pa_simple_write(s->fd, data, nbytes, NULL);

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

pa_operation *pa_stream_cork(pa_stream *s, int b, pa_stream_success_cb_t cb, void *userdata)
{
	dprintf("pa_stream_cork: %p\n", s);

	// TODO

	pa_operation *o = malloc(sizeof(pa_operation));
	o->refs = 1;
	o->state = PA_OPERATION_DONE;
	return o;
}

pa_operation *pa_stream_trigger(pa_stream *s, pa_stream_success_cb_t cb, void *userdata)
{
	dprintf("pa_stream_trigger: %p\n", s);

	// TODO

	pa_operation *o = malloc(sizeof(pa_operation));
	o->refs = 1;
	o->state = PA_OPERATION_DONE;
	return o;
}

int pa_stream_drop(pa_stream *s)
{
	dprintf("pa_stream_drop: %p\n", s);

	// TODO: recording

	return 0;
}

int pa_stream_peek(pa_stream *s, const void **data, size_t *nbytes)
{
	dprintf("pa_stream_peek: %p %p %p\n", s, data, nbytes);

	// TODO: recording
	if(data   != NULL) *data   = NULL;
	if(nbytes != NULL) *nbytes = 0;

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
	pa_simple_drain(s->fd, NULL);
	pa_operation *o = malloc(sizeof(pa_operation));
	o->refs = 1;
	o->state = PA_OPERATION_DONE;
	return o;
}

pa_operation *pa_stream_flush(pa_stream *s, pa_stream_success_cb_t cb, void *userdata)
{
	dprintf("pa_stream_flush: %p %p %p\n", s, cb, userdata);
	pa_simple_flush(s->fd);
	pa_operation *o = malloc(sizeof(pa_operation));
	o->refs = 1;
	o->state = PA_OPERATION_DONE;
	return o;
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

	s->fd = NULL;

	return s;
}

pa_stream *pa_stream_new(pa_context *c, const char *name, const pa_sample_spec *ss, const pa_channel_map *map)
{
	return pa_stream_new_with_proplist(c, name, ss, map, NULL);
}

int pa_stream_connect_record(pa_stream *s, const char *dev, const pa_buffer_attr *attr, int flags)
{
	dprintf("pa_stream_connect_record: %p %s %p %08X\n", s, dev, attr, flags);

	s->fire_connect = 1;
	s->started = 1;

	s->fd = pa_simple_new(NULL, s->name, PA_STREAM_RECORD, NULL, s->name, &(s->ss), NULL, NULL, NULL);
	if(s->fd == NULL)
		return -1;

	return 0;
}

int pa_stream_connect_playback(pa_stream *s, const char *dev, const pa_buffer_attr *attr, int flags, const pa_cvolume *volume, pa_stream *sync_stream)
{
	dprintf("pa_stream_connect_playback: %p %s %p %08X %p %p\n", s, dev, attr, flags, volume, sync_stream);
	if(attr != NULL)
		dprintf("\tattr: %i %i %i %i %i\n", attr->maxlength, attr->tlength, attr->prebuf, attr->minreq, attr->fragsize);

	s->fire_connect = 1;
	s->started = 1;

	s->fd = pa_simple_new(NULL, s->name, PA_STREAM_PLAYBACK, NULL, s->name, &(s->ss), NULL, NULL, NULL);
	if(s->fd == NULL)
		return -1;

	return 0;
}

int pa_stream_disconnect(pa_stream *s)
{
	dprintf("pa_stream_disconnect: %p\n", s);

	if(s == NULL)
		return -1;

	if(s->fd == NULL)
		return 0;

	pa_simple_free(s->fd);
	s->fd = NULL;
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
		if(s->name != NULL) free(s->name);
		free(s);
	}
}

int pa_stream_get_latency(pa_stream *s, pa_usec_t *r_usec, int *negative)
{
	return -1;
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

void pa_stream_set_latency_update_callback(pa_stream *s, pa_stream_notify_cb_t cb, void *userdata)
{
	dprintf("pa_stream_set_latency_update_callback: %p %p %p\n", s, cb, userdata);
	// TODO: cb
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

