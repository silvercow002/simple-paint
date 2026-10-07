#include <stdlib.h>
#include <GL/freeglut.h>
#include "canvas_io.h"

static unsigned char *pixels;
static int saved_width, saved_height;

void canvas_capture(int width, int height)
{
    unsigned char *next = malloc((size_t)width * height * 3);

    GLint previous_buffer;
    glGetIntegerv(GL_READ_BUFFER, &previous_buffer);
    glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ROW_LENGTH, 0);
    glPixelStorei(GL_PACK_SKIP_ROWS, 0);
    glPixelStorei(GL_PACK_SKIP_PIXELS, 0);

    
    glReadBuffer(GL_BACK);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, next);
    glReadBuffer(previous_buffer);
    glPopClientAttrib();

    free(pixels);
    pixels = next;
    saved_width = width;
    saved_height = height;
}

void canvas_restore(int width, int height)
{
    glPushAttrib(GL_CURRENT_BIT | GL_PIXEL_MODE_BIT | GL_ENABLE_BIT);
    glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_SCISSOR_TEST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

    glPixelZoom((GLfloat)width / saved_width, (GLfloat)height / saved_height);
    glRasterPos2i(0, 0);
    glDrawPixels(saved_width, saved_height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glPopClientAttrib();
    glPopAttrib();
}

void canvas_free(void)
{
    free(pixels);
    pixels = NULL;
    saved_width = saved_height = 0;
}
