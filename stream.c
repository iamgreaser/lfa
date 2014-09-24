#include "lfa.h"

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

