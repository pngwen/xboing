// TODO: typedefs we have to make to get past compiler errors, these all
//       indicate things we need to change
#ifndef FAKETYPES_H
#define FAKETYPES_H

// Raylib Types
typedef int Texture2D;

// Boolean macros
#define False 0
#define True 1

// Random macros
#define None 0
#define CWWidth (1<<2)
#define CWHeight (1<<3)
#define CoordModeOrigin 0
#define Always 1

// XPM error macros
#define XpmColorError    1
#define XpmSuccess       0
#define XpmOpenFailed   -1
#define XpmFileInvalid  -2
#define XpmNoMemory     -3
#define XpmColorFailed  -4

// Relating to the X11 graphical context
typedef int Display;
typedef int Window;
typedef int GC; // graphics context

// Drawing / graphical manipulations
typedef int Colormap;
typedef int Pixmap;
typedef int Region; // A region of pixels
typedef struct{
    short x, y;
} XPoint;
typedef struct {
    int pixel;
} XColor;

// GC Values (from X11)
typedef enum {
    LineSolid,
    LineOnOffDash,
    LineDoubleDash
} line_style;
typedef enum {
    CapNotLast,
    CapButt,
    CapRound,
    CapProjecting
} cap_style;
typedef enum{
    JoinMiter,
    JoinRound,
    JoinBevel
} join_style;

// Uncategorized
typedef unsigned long int u_long;
typedef int XEvent;
typedef struct {
    int ascent;
    int fid;
} XFontStruct;
typedef struct {
    int x, y;
    int width, height;
    int border_width;
    int sibling;
    int stack_mode;
} XWindowChanges;
typedef struct {
    int width;
    int height;
} XWindowAttributes;

#endif
