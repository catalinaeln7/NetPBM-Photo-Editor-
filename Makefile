CC=gcc
CFLAGS=-Wall -Wextra -std=c99
TARGETS=image_editor

build: $(TARGETS)

image_editor: image_editor.c functions.c
	$(CC) $(CFLAGS) image_editor.c functions.c -o image_editor

pack:
	zip -FSr 315CA_CatalinaPopa_Tema3.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
