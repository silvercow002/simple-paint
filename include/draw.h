#ifndef DRAW_H
#define DRAW_H

void draw_init(void);
int draw_keybord(unsigned char key, int x, int y);
int draw_mouse(int button, int state, int x, int y);

#endif