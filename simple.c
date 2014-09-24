#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>

#include <fcntl.h>

#include <sys/soundcard.h>

#include "lfa.h"

pa_simple *pa_simple_new(const char *server, const char *name, int dir, const char *dev, const char *stream_name, const pa_sample_spec *ss, const pa_channel_map *map, const pa_buffer_attr *attr, int *error)
{
	int fmt;
	int stereo;
	pa_simple *pa;

	/* list of things we give no fucks about */
	(void)server;
	(void)name;
	(void)dev;
	(void)stream_name;
	(void)map;
	(void)attr;

	fprintf(stderr, "pa_simple_new OSS wrapper, proudly served by LFA\n");

	/* check format spec */
	if(ss == NULL) return NULL;
	switch(ss->format)
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
			return NULL;
	}

	switch(ss->channels)
	{
		case 1:
			stereo = 0;
			break;
		case 2:
			stereo = 1;
			break;
		default:
			/* i'd like to just keep this simpleish */
			return NULL;
	}

	/* act based on the stream direction */
	switch(dir)
	{
		case PA_STREAM_PLAYBACK:
			pa = malloc(sizeof(pa_simple));
			*pa = open("/dev/dsp", O_WRONLY);
			ioctl(*pa, SNDCTL_DSP_SPEED, &(ss->rate));
			ioctl(*pa, SNDCTL_DSP_SETFMT, &fmt);
			ioctl(*pa, SNDCTL_DSP_STEREO, &stereo);
			return pa;
		default:
			return NULL;
	}
}

void pa_simple_free(pa_simple *pa)
{
	close(*pa);
	free(pa);
}

int pa_simple_drain(pa_simple *pa, int *error)
{
	/* TODO */
	ioctl(*pa, SNDCTL_DSP_SYNC, NULL);
	return 0;
}

int pa_simple_flush(pa_simple *pa)
{
	/* TODO */
	return 0;
}

int pa_simple_write(pa_simple *pa, const void *data, size_t bytes, int *error)
{
	return write(*pa, data, bytes);
}

int pa_simple_read(pa_simple *pa, void *data, size_t bytes, int *error)
{
	return read(*pa, data, bytes);
}

