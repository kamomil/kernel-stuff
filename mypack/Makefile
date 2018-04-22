CFLAGS += -g 

INSTALL ?= install

PROG ?= divide_err
all: $(PROG)

install: all
		$(INSTALL) -D -m 755 $(PROG) $(DESTDIR)/usr/bin/
clean:
		rm *.o $(PROG)
SOURCES_C = $(PROG).c

$(SOURCES_C:%.c=%.o):
		$(CC) $(CFLAGS) -o $@ -c $(@:%.o=%.c)

$(PROG): $(SOURCES_C:%.c=%.o)
		$(CC) -o $@ $^ $(LIBS) 
echo:
		echo ${FLAGS} ${CC} ${LDFLAGS} ${INSTALL} BB
			echo "OK"
