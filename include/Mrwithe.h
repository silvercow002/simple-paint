#ifndef MRWITHE_H
#define MRWITHE_H

void eraser_init(int *ready);
void eraser_begin(void);
void eraser_end(void);
int eraser_keyboard(unsigned char key);
void eraser_motion(int x, int y);
int eraser_mouse(int button, int state, int x, int y);
void eraser_capture(void);
void eraser_preview(int height);

#endif
