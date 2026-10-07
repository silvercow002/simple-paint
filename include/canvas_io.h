#ifndef CANVAS_IO_H
#define CANVAS_IO_H

void canvas_capture(int width, int height);
void canvas_restore(int width, int height);
void canvas_free(void);
int canvas_save_bmp(const char *filename);

#endif
