#include "lfa.h"

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

size_t pa_sample_size_of_format(int f)
{
	switch(f)
	{
		case PA_SAMPLE_U8:
		case PA_SAMPLE_ALAW:
		case PA_SAMPLE_ULAW:
			return 1;

		case PA_SAMPLE_S16LE:
		case PA_SAMPLE_S16BE:
			return 2;

		case PA_SAMPLE_S24LE:
		case PA_SAMPLE_S24BE:
			return 3;

		case PA_SAMPLE_S24_32LE:
		case PA_SAMPLE_S24_32BE:
		case PA_SAMPLE_FLOAT32LE:
		case PA_SAMPLE_FLOAT32BE:
		case PA_SAMPLE_S32LE:
		case PA_SAMPLE_S32BE:
			return 4;

		default:
			return 1;
	}
}

size_t pa_sample_size(const pa_sample_spec *spec)
{
	return spec->channels * pa_sample_size_of_format(spec->format);
}

size_t pa_usec_to_bytes(pa_usec_t t, const pa_sample_spec *spec)
{
	return (size_t)(((t * spec->rate) / (uint64_t)1000000) * pa_sample_size(spec));
}

char *pa_path_get_filename(const char *p)
{
	const char *v;

	if(p == NULL) return NULL;

	for(v = p + strlen(p) - 1; v >= p; v--)
		if(*v == '/')
			break;

	v++;
	return strdup(v);
}

char *pa_get_binary_name(char *s, size_t l)
{
	dprintf("pa_get_binary_name: %s %i\n", s, (int)l);

	// TODO!
	return NULL;
}


