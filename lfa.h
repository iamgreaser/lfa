#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>

#include <pthread.h>
#include <signal.h>
#include <fcntl.h>

#include <sys/soundcard.h>

#if 0
#define dprintf(...) fprintf(stderr, ...)
#else
#define dprintf(...)
#endif

enum
{
	PA_SAMPLE_U8,
	PA_SAMPLE_ALAW,
	PA_SAMPLE_ULAW,
	PA_SAMPLE_S16LE,
	PA_SAMPLE_S16BE,
	PA_SAMPLE_FLOAT32LE,
	PA_SAMPLE_FLOAT32BE,
	PA_SAMPLE_S32LE,
	PA_SAMPLE_S32BE,
	PA_SAMPLE_S24LE,
	PA_SAMPLE_S24BE,
	PA_SAMPLE_S24_32LE,
	PA_SAMPLE_S24_32BE,
	PA_SAMPLE_MAX,

	PA_SAMPLE_INVALID,
};

enum
{
	PA_STREAM_NODIRECTION,
	PA_STREAM_PLAYBACK,
	PA_STREAM_RECORD,
	PA_STREAM_UPLOAD,
};

enum
{
	PA_OPERATION_RUNNING,
	PA_OPERATION_DONE,
	PA_OPERATION_CANCELLED,
};

enum
{
	PA_SEEK_RELATIVE,
	PA_SEEK_ABSOLUTE,
	PA_SEEK_RELATIVE_ON_READ,
	PA_SEEK_RELATIVE_END,
};

enum
{
	PA_IO_EVENT_NULL = 0,
	PA_IO_EVENT_INPUT = 1,
	PA_IO_EVENT_OUTPUT = 2,
	PA_IO_EVENT_HANGUP = 4,
	PA_IO_EVENT_ERROR = 8,
};

enum
{
	PA_CONTEXT_UNCONNECTED,
	PA_CONTEXT_CONNECTING,
	PA_CONTEXT_AUTHORIZING,
	PA_CONTEXT_SETTING_NAME,
	PA_CONTEXT_READY,
	PA_CONTEXT_FAILED,
	PA_CONTEXT_TERMINATED,
};

enum
{
	PA_STREAM_UNCONNECTED,
	PA_STREAM_CREATING,
	PA_STREAM_READY,
	PA_STREAM_FAILED,
	PA_STREAM_TERMINATED,
};

enum
{
	PA_CHANNEL_MAP_AIFF,
	PA_CHANNEL_MAP_AUX,
	PA_CHANNEL_MAP_WAVEEX,
	PA_CHANNEL_MAP_DEF_MAX,
	PA_CHANNEL_MAP_DEFAULT,
};

enum
{
	PA_CHANNEL_POSITION_INVALID,
	PA_CHANNEL_POSITION_MONO,
	PA_CHANNEL_POSITION_FRONT_LEFT,
	PA_CHANNEL_POSITION_FRONT_RIGHT,
	PA_CHANNEL_POSITION_FRONT_CENTER,
	PA_CHANNEL_POSITION_REAR_CENTER,
	PA_CHANNEL_POSITION_REAR_LEFT,
	PA_CHANNEL_POSITION_REAR_RIGHT,
	PA_CHANNEL_POSITION_LFE,
	PA_CHANNEL_POSITION_FRONT_LEFT_OF_CENTER,
	PA_CHANNEL_POSITION_FRONT_RIGHT_OF_CENTER,
	PA_CHANNEL_POSITION_SIDE_LEFT,
	PA_CHANNEL_POSITION_SIDE_RIGHT,
	PA_CHANNEL_POSITION_AUX0,
	PA_CHANNEL_POSITION_AUX1,
	PA_CHANNEL_POSITION_AUX2,
	PA_CHANNEL_POSITION_AUX3,
	PA_CHANNEL_POSITION_AUX4,
	PA_CHANNEL_POSITION_AUX5,
	PA_CHANNEL_POSITION_AUX6,
	PA_CHANNEL_POSITION_AUX7,
	PA_CHANNEL_POSITION_AUX8,
	PA_CHANNEL_POSITION_AUX9,
	PA_CHANNEL_POSITION_AUX10,
	PA_CHANNEL_POSITION_AUX11,
	PA_CHANNEL_POSITION_AUX12,
	PA_CHANNEL_POSITION_AUX13,
	PA_CHANNEL_POSITION_AUX14,
	PA_CHANNEL_POSITION_AUX15,
	PA_CHANNEL_POSITION_AUX16,
	PA_CHANNEL_POSITION_AUX17,
	PA_CHANNEL_POSITION_AUX18,
	PA_CHANNEL_POSITION_AUX19,
	PA_CHANNEL_POSITION_AUX20,
	PA_CHANNEL_POSITION_AUX21,
	PA_CHANNEL_POSITION_AUX22,
	PA_CHANNEL_POSITION_AUX23,
	PA_CHANNEL_POSITION_AUX24,
	PA_CHANNEL_POSITION_AUX25,
	PA_CHANNEL_POSITION_AUX26,
	PA_CHANNEL_POSITION_AUX27,
	PA_CHANNEL_POSITION_AUX28,
	PA_CHANNEL_POSITION_AUX29,
	PA_CHANNEL_POSITION_AUX30,
	PA_CHANNEL_POSITION_AUX31,
	PA_CHANNEL_POSITION_TOP_CENTER,
	PA_CHANNEL_POSITION_TOP_FRONT_LEFT,
	PA_CHANNEL_POSITION_TOP_FRONT_RIGHT,
	PA_CHANNEL_POSITION_TOP_FRONT_CENTER,
	PA_CHANNEL_POSITION_TOP_REAR_LEFT,
	PA_CHANNEL_POSITION_TOP_REAR_RIGHT,
	PA_CHANNEL_POSITION_TOP_REAR_CENTER,
	PA_CHANNEL_POSITION_MAX,
};

typedef uint64_t pa_usec_t;

typedef struct pa_sample_spec
{
	int format;
	uint32_t rate;
	uint8_t channels;
} pa_sample_spec;

typedef struct pa_buffer_attr
{
	uint32_t maxlength;
	uint32_t tlength;
	uint32_t prebuf;
	uint32_t minreq;
	uint32_t fragsize;
} pa_buffer_attr;

#define PA_CHANNELS_MAX 32U
#define PA_RATE_MAX (48000U*4U)

typedef struct pa_cvolume 
{
	uint8_t channels;
	uint32_t values[PA_CHANNELS_MAX];
} pa_cvolume;

typedef struct pa_channel_map
{
	uint8_t channels;
	int map[PA_CHANNELS_MAX];
} pa_channel_map;

// Simple API
typedef int pa_simple;

pa_simple *pa_simple_new(const char *server, const char *name, int dir, const char *dev, const char *stream_name, const pa_sample_spec *ss, const pa_channel_map *map, const pa_buffer_attr *attr, int *error);
void pa_simple_free(pa_simple *pa);
int pa_simple_drain(pa_simple *pa, int *error);
int pa_simple_flush(pa_simple *pa);
int pa_simple_write(pa_simple *pa, const void *data, size_t bytes, int *error);

// Async API
typedef struct pa_mainloop_api pa_mainloop_api;
typedef struct pa_mainloop pa_mainloop;
typedef struct pa_threaded_mainloop pa_threaded_mainloop;

// Async API: Proplist
typedef struct pa_proplist
{
	int dummy; // TODO.
} pa_proplist;

// Async API: Operations
typedef struct pa_operation pa_operation;
typedef void (*pa_operation_notify_cb_t)(pa_operation *o, void *userdata);

struct pa_operation
{
	int refs;
	int state;
};

// Async API: Context
typedef struct pa_context pa_context;
typedef struct pa_stream pa_stream;

typedef void(*pa_context_notify_cb_t)(pa_context *c, void *userdata);
typedef void(*pa_context_event_cb_t)(pa_context *c, const char *name, pa_proplist *p, void *userdata);
typedef void(*pa_context_success_cb_t)(pa_context *s, int success, void *userdata);
typedef void(*pa_free_cb_t)(void *p);

struct pa_context
{
	pa_mainloop *m;
	pa_stream *s;
	pa_context_notify_cb_t state_cb;
	int refs;
};

// Async API: Spawn
typedef struct pa_spawn_api pa_spawn_api;
struct pa_spawn_api
{
	void (*prefork)(void);
	void (*postfork)(void);
	void (*atfork)(void);
};

// Async API: Stream

typedef void (*pa_stream_notify_cb_t)(pa_stream *s, void *userdata);
typedef void (*pa_stream_request_cb_t)(pa_stream *s, size_t nbytes, void *userdata);
typedef void (*pa_stream_event_cb_t)(pa_stream *s, const char *name, pa_proplist *pl, void *userdata);
typedef void (*pa_stream_success_cb_t)(pa_stream *s, int success, void *userdata);

struct pa_stream
{
	pa_context *c;

	char *name;
	pa_sample_spec ss;
	void *bufdata;
	int buflen;
	int refs;

	// how many fucking callbacks does this bloatware need
	pa_stream_event_cb_t event_cb;
	pa_stream_notify_cb_t buffer_attr_cb;
	pa_stream_notify_cb_t state_cb;
	pa_stream_notify_cb_t started_cb;
	pa_stream_notify_cb_t overflow_cb;
	pa_stream_notify_cb_t underflow_cb;
	pa_stream_notify_cb_t moved_cb;
	pa_stream_notify_cb_t suspended_cb;
	pa_stream_request_cb_t read_cb;
	pa_stream_request_cb_t write_cb;

	// oh and we need to wait for a return or some shit like that
	int fire_connect;
	int started;

	pa_simple *fd;
};

// Async API: Mainloop

typedef struct pa_io_event
{
	// TODO.
} pa_io_event;

typedef struct pa_time_event
{
	// TODO.
} pa_time_event;

typedef struct pa_defer_event
{
	// TODO.
} pa_defer_event;

struct pa_mainloop
{
	pa_context *c;
	pa_mainloop_api *api;
};

struct pa_threaded_mainloop
{
	pthread_t pthread;
	int retval;
	pa_mainloop *main;
};

typedef void(*pa_io_event_cb_t)(pa_mainloop_api *a, pa_io_event *e, int fd, int events, void *userdata);
typedef void(*pa_io_event_destroy_cb_t)(pa_mainloop_api *a, pa_io_event *e, void *userdata);
typedef void(*pa_time_event_cb_t)(pa_mainloop_api *a, pa_time_event *e, const struct timeval *tv, void *userdata);
typedef void(*pa_time_event_destroy_cb_t)(pa_mainloop_api *a, pa_time_event *e, void *userdata);
typedef void(*pa_defer_event_cb_t)(pa_mainloop_api *a, pa_defer_event *e, void *userdata);
typedef void(*pa_defer_event_destroy_cb_t)(pa_mainloop_api *a, pa_defer_event *e, void *userdata);

struct pa_mainloop_api
{
	void *userdata;
	pa_io_event *(*io_new)(pa_mainloop_api *a, int fd, int events, pa_io_event_cb_t cb, void *userdata);
	void (*io_enable)(pa_io_event *e, int events);
	void (*io_free)(pa_io_event *e);
	void (*io_set_destroy)(pa_io_event *e, pa_io_event_destroy_cb_t cb);
	pa_time_event *(*time_new)(pa_mainloop_api *a, const struct timeval *tv, pa_time_event_cb_t cb, void *userdata);
	void (*time_restart)(pa_time_event *e, const struct timeval *tv);
	void (*time_free)(pa_time_event *e);
	void (*time_set_destroy)(pa_time_event *e, pa_time_event_destroy_cb_t cb);
	pa_defer_event *(*defer_new)(pa_mainloop_api *a, pa_defer_event_cb_t cb, void *userdata);
	void (*defer_enable)(pa_defer_event *e, int b);
	void (*defer_free)(pa_defer_event *e);
	void (*defer_set_destroy)(pa_defer_event *e, pa_defer_event_destroy_cb_t cb);
	void (*quit)(pa_mainloop_api *a, int retval);
	
	pa_mainloop *m;
};

pa_mainloop *pa_mainloop_new(void);
void pa_mainloop_free(pa_mainloop *m);
int pa_mainloop_run(pa_mainloop *m, int *retval);
pa_mainloop_api *pa_mainloop_get_api(pa_mainloop *m);

