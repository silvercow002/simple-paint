#ifndef PEN_H
#define PEN_H

typedef enum {
    BRUSH_NONE,
    BRUSH_DOT,
    BRUSH_SQUARE,
} Brush_e;
extern Brush_e brush;

typedef enum {
    SHAPE_NONE,
    SHAPE_LINE,    
    SHAPE_MISUMI,
    SHAPE_CIRCLE
} Shape_e;
extern Shape_e shape;

typedef enum{
    red,
    blue,
} color_e;
extern color_e pnt_color;

extern float pnt_size;



#endif