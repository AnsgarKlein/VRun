
PACKAGES	=	--pkg gtk+-3.0

CC			=	valac
CFLAGS		=	$(PACKAGES) --thread
SOURCES		=	src/vrun.vala

BUILDPATH	=	build/
BINARY		=	vrun


all: VStart
	#sucessfully compiled everything

install:
	cp $(BUILDPATH)$(BINARY) /usr/bin/$(BINARY)

uninstall:
	rm /usr/bin/$(BINARY)

clean:
	rm $(BUILDPATH)$(BINARY)



VStart: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(BUILDPATH)$(BINARY)

