#include "draw.h"

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include<GL/freeglut.h>

void init() {
    glReadBuffer(GL_BACK);
    glDrawBuffer(GL_BACK);
    glPixelStoref(GL_PACK_ALIGNMENT, 1);
    gl
}