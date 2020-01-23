appname := gtk-hello-world
srcfiles := gtk-hello-world.c
objects  := gtk-hello-world.o

CFLAGS += -Wall $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS += $(shell pkg-config --libs gtk+-3.0)

all: $(appname)

$(appname): $(objects)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

install: $(appname)
	install -m 0755 -d ${DESTDIR}/usr/bin
	install -m 0755 $(appname) ${DESTDIR}/usr/bin

clean:
	$(RM) $(objects) $(appname)
