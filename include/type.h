#ifndef TYPE_H
#define TYPE_H

void type_init(void);
void type_begin(void);
void type_cancel(void);
void type_commit(void);
int type_keyboard(unsigned char key);
int type_mouse(int button, int state, int x, int y);
void type_render(int height);
void type_preview(int height);
void type_clear_saved(void);

#endif
