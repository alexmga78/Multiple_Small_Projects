C = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

TARGETS = photoshop
FILES = image_editor.c image_editor.h apply.c crop.c equalize.c histogram.c load.c rotate.c save.c select_all.c select.c warnings.c utils.c
RESULT = image_editor

build: $(TARGETS)

photoshop: $(FILES)
	$(C) $(CFLAGS) $(FILES) -o $(RESULT)

pack:
	zip -FSr image_editor.zip README Makefile *.c *.h * -r

clean:
	rm -f *.o $(RESULT)

.PHONY: pack clean



