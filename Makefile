CC=       	gcc
CFLAGS= 	-g -std=gnu99 -Wall -Iinclude
LDFLAGS=
LIBRARIES=      lib/smalloc-ff.so \
		lib/smalloc-bf.so \
		lib/smalloc-wf.so
HEADERS=	$(wildcard include/malloc/*.h)
SOURCES=	$(wildcard src/*.c)

all:    $(LIBRARIES) 

lib/smalloc-ff.so:   	$(SOURCES) $(HEADERS)
	@echo "Building $@"
	@$(CC) -shared -fPIC $(CFLAGS) -DFIT=0 -o $@ $(SOURCES) $(LDFLAGS)

lib/smalloc-wf.so:   	$(SOURCES) $(HEADERS)
	@echo "Building $@"
	@$(CC) -shared -fPIC $(CFLAGS) -DFIT=1 -o $@ $(SOURCES) $(LDFLAGS)

lib/smalloc-bf.so:   	$(SOURCES) $(HEADERS)
	@echo "Building $@"
	@$(CC) -shared -fPIC $(CFLAGS) -DFIT=2 -o $@ $(SOURCES) $(LDFLAGS)

clean:
	@echo "Removing libraries"
	@rm -f $(LIBRARIES)
	@echo "Removing tests"
	@rm -f $(TESTS) test.log

.PHONY: all clean