
PACKAGES	=	--pkg gtk+-3.0

CC			=	valac
CFLAGS		=	$(PACKAGES) --thread
SOURCES		=	src/vrun.vala

BINARY		=	vrun


all: VStart
	#sucessfully compiled everything

install:
	cp $(BINARY) /usr/bin/$(BINARY)

uninstall:
	rm /usr/bin/$(BINARY)

clean:
	rm $(BINARY)



VStart: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(BINARY)

