#include "menu.h"

#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>

#define MENU_HELLO 1
#define MENU_QUIT  2

// 使用者選取項目時，GLUT 會呼叫這個函式
static void menu_callback(int choice)
{
    switch (choice)
    {
        case MENU_HELLO:
            printf("Hello from menu!\n");
            break;

        case MENU_QUIT:
            exit(EXIT_SUCCESS);
    }
}

// 在 main.c 建立視窗後呼叫一次
void menu_init(void)
{
    glutCreateMenu(menu_callback);

    // 第一個參數是顯示文字，第二個是傳給 callback 的值
    glutAddMenuEntry("Hello", MENU_HELLO);
    glutAddMenuEntry("Quit", MENU_QUIT);

    // 把選單附加到目前視窗的滑鼠右鍵
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}