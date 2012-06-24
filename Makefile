
PACKAGES	=	--pkg gtk+-3.0

CC			=	valac
CFLAGS		=	$(PACKAGES) --thread
SOURCES		=	src/vstart.vala

BINARY		=	vstart


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

