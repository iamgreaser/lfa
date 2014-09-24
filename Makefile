CFLAGS = -g -O2
LDFLAGS = -g -O2

INCLUDES = \
	lfa.h \
	#

CFILES = \
	async.c \
	context.c \
	mainloop.c \
	operation.c \
	stream.c \
	\
	simple.c \
	#

SYMLINKS = \
	libpulse-simple.so.0 \
	libpulse.so.0 \
	#

all: liblfa.so $(SYMLINKS)

clean:
	rm -f liblfa.so
	rm -f $(SYMLINKS)

liblfa.so: $(CFILES) $(INCLUDES)
	$(CC) -shared -fPIC $(CFLAGS) -o liblfa.so $(CFILES) $(LDFLAGS)

# Symlinks
libpulse-simple.so.0: liblfa.so
	ln -s liblfa.so libpulse-simple.so.0 || true

libpulse.so.0: liblfa.so
	ln -s liblfa.so libpulse.so.0 || true

