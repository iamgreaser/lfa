CFLAGS = -g -O2
LDFLAGS = -g -O2

SYMLINKS = \
	libpulse-simple.so.0 \
	libpulse.so.0 \
	#

all: liblfa.so $(SYMLINKS)

clean:
	rm -f liblfa.so
	rm -f $(SYMLINKS)

liblfa.so: async.c simple.c lfa.h
	$(CC) -shared -fPIC $(CFLAGS) -o liblfa.so async.c simple.c $(LDFLAGS)

# Symlinks
libpulse-simple.so.0: liblfa.so
	ln -s liblfa.so libpulse-simple.so.0 || true

libpulse.so.0: liblfa.so
	ln -s liblfa.so libpulse.so.0 || true

