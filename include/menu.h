#ifndef MENU_H
#define MENU_H

void menu_init(void (*on_tool_changed)(void), void (*on_save_bmp)(void),
               void (*on_type)(void), void (*on_eraser)(void));

#endif
