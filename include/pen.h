#ifndef PEN_H
#define PEN_H

typedef enum {
    BRUSH_DOT,
    BRUSH_SQUARE,
} Brush_e;
extern Brush_e brush;

typedef enum {
    SHAPE_NONE,
    SHAPE_LINE,    
    SHAPE_MISUMI
} Shape_e;
extern Shape_e shape;


#endif