appname := cpp-hello-world
srcfiles := cpp-hello-world.cpp
objects  := cpp-hello-world.o

all: $(appname)

$(appname): $(objects)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(appname) $(objects) $(LDLIBS)

install: $(appname)
	install -m 0755 -d ${DESTDIR}/usr/bin
	install -m 0755 $(appname) ${DESTDIR}/usr/bin

clean:
	$(RM) $(objects) $(appname)
