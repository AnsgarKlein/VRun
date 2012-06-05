
PACKAGES	=	--pkg gtk+-3.0

CC			=	valac
CFLAGS		=	$(PACKAGES) --thread
SOURCES		=	src/vlaunch.vala

BINARY		=	vlaunch


all: VLaunch
	#sucessfully compiled everything

install:
	cp $(BINARY) /usr/bin/$(BINARY)

uninstall:
	rm /usr/bin/$(BINARY)

clean:
	rm $(BINARY)



VLaunch: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(BINARY)

