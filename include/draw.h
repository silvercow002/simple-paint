#ifndef DRAW_H
#define DRAW_H

void draw_init(void);
void draw_cancel_input(void);
int draw_motion(int x, int y);
int draw_mouse(int button, int state, int x, int y);
void draw_render();

void draw_string(float x, float y, const char* str);

extern int width;
extern int height;
#endif