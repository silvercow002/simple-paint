#include<stdio.h>
#include<stdlib.h>
#include<GL/freeglut.h>

#include"menu.h"
#include"draw.h"
#include"canvas_io.h"
#include"overlay.h"

#include<time.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

static int snapshot_ready;
static int save_bmp_pending;

static void request_save_bmp(void) {
    save_bmp_pending = 1;
    glutPostRedisplay();
}

int width, height;
static void init_window(void)
{
    // set matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // size
    // set left-down(0, 0), right-up(w, h)
    gluOrtho2D(0.0, (double)width, 0.0, (double)height);

    // all windows as painting area
    glViewport(0, 0, width, height);

    // reset
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    
    glDrawBuffer(GL_BACK);
    glReadBuffer(GL_BACK);
    // Tightly packed RGB rows for both snapshots and restoration.
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glutPostRedisplay();
}

static void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    if (snapshot_ready) {
        canvas_restore(width, height);
    }
    draw_render();

    if (save_bmp_pending) {
        char filename[96];
        static unsigned int save_number;
        snprintf(filename, sizeof filename, "canvas-%lld-%u.bmp",
                 (long long)time(NULL), ++save_number);
        canvas_capture(width, height);
        snapshot_ready = 1;
        draw_clear_saved();
        if (canvas_save_bmp(filename)) {
            char directory[4096];

        // complier in windows 
#ifdef _WIN32
            char *location = _getcwd(directory, sizeof directory);
            const char *separator = "\\";
#else
        // linxu
            char *location = getcwd(directory, sizeof directory);
            const char *separator = "/";
#endif
            if (location)
                fprintf(stderr, "saved BMP: %s%s%s\n", directory, separator, filename);
            else
                fprintf(stderr, "saved BMP: %s (could not resolve working directory)\n", filename);
        } else {
            fprintf(stderr, "couldnt save BMP: %s\n", filename);
        }
        save_bmp_pending = 0;
    }

    overlay_render(width, height);

    glutSwapBuffers();
    fprintf(stderr, "\n");
}

static void timer_handler(int value)
{
    // no use
    (void)value;

    // draw the pic again avoid show FPS
    glClear(GL_COLOR_BUFFER_BIT);

    if (snapshot_ready) {
        canvas_restore(width, height);
    }
    draw_render();
    canvas_capture(width, height);
    snapshot_ready = 1;
    draw_clear_saved();
    glutPostRedisplay();

    glutTimerFunc(1000, timer_handler, 0);
}

static void reshape(int nw, int nh) {
    width = nw;
    height = nh;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (double)width, 0.0, (double)height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);

    glutPostRedisplay();
}


static void keybord_handler(unsigned char key, int x, int y) {

    switch (key) {
    case 'V':
    case 'v':
        overlay_toggle_grid();
        glutPostRedisplay();
        break;
    case 'Q':
        exit(0);
        break;
    case 'q':
        exit(0);
        break;
    case 'C':
        break;
    case 'c':
        break;
    default:
        break;
    }
}

static void motion_handler(int x, int y) {
    if (draw_motion(x, y)) {
        return;
    }
    
}


static void mouse_handler(int button, int state, int x, int y) {
    if (draw_mouse(button, state, x, y)) {
        return;
    }

}


int main(int argc, char** argv) {
    width = 400;
    height = 400;
    if (argc >= 3) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
    }

    // hide
    argc = 1;
    argv[1] = NULL;

    // free after quit
    atexit(canvas_free);

    // glut init
    glutInit(&argc, argv);
    // performancnce frist, arg from google, i have no idea what it done
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);

    glutInitWindowSize(width, height);
    // glutInitWindowPosition(100, 50);


    glutCreateWindow("hw1");

    menu_init(draw_cancel_input, request_save_bmp);
    draw_init();

    init_window();

    glutDisplayFunc(display);
    glutKeyboardFunc(keybord_handler);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse_handler);
    glutMotionFunc(motion_handler);


    overlay_init();
    glutTimerFunc(1000, timer_handler, 0);
    glutMainLoop();
}