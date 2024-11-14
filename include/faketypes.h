// TODO: typedefs we have to make to get past compiler errors, these all
//       indicate things we need to change
#ifndef FAKETYPES_H
#define FAKETYPES_H

//#include <raylib.h>

typedef int Display;
typedef int Window;
typedef int Colormap;
typedef unsigned long int u_long;
typedef int KeySym;
// typedef int XEvent; // replaced with struct below
typedef int uid_t;
typedef int Pixmap;

// Raylib Types
//typedef int Texture2D;

// Boolean macros
#define False 0
#define True 1

// Random macros
#define None 0
#define CWWidth (1<<2)
#define CWHeight (1<<3)
#define CoordModeOrigin 0
#define Always 1
#define ParentRelative 0
#define XBLACK 0
#define XWHITE 0
#define CopyFromParent 0


//SizeHint macros
#define PPosition (1L<<2)
#define PSize (1L<<3)
#define PMinSize (1L<<4)
#define PMaxSize (1L<<5)
#define NormalState 1

//Wm_Hints macros
#define StateHint (1L<<1)
#define InputHint (1L<<0)
#define IconPixmapHint (1L<<2)
#define IconWindowHint (1L<<3)

//Window Attributes
#define CWColormap (1L<<13)
#define CWBackingStore (1L<<6)

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

// Key Masks

#define KeyPressMask           0
#define KeyReleaseMask         0
#define ButtonPressMask        0
#define ButtonReleaseMask      0 
#define ButtonMotionMask       0
#define ExposureMask           0
#define StructureNotifyMask    0
#define PointerMotionHintMask  0

#define XK_C      0
#define XK_c      0
#define XK_H      0
#define XK_h      0
#define XK_J      0
#define XK_j      0
#define XK_K      0
#define XK_k      0
#define XK_L      0
#define XK_l      0
#define XK_N      0
#define XK_n      0
#define XK_P      0
#define XK_p      0
#define XK_Q      0
#define XK_q      0
#define XK_R      0
#define XK_r      0
#define XK_S      0
#define XK_s      0
#define XK_T      0
#define XK_t      0
#define XK_V      0
#define XK_v      0

#define XK_Left   0
#define XK_Right  0



// Mouse Events

#define Button1  0
#define Button2  0
#define Button3  0

#define CURSOR_PLUS   0
#define CURSOR_POINT  0

typedef struct {
    int button;
    int subwindow;
} XButton;

typedef struct {
    XButton xbutton;
} XEvent;

// Uncategorized

typedef unsigned long int u_long;

#define EvenOddRule  0

typedef struct {
    int ascent;
    int descent;
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

typedef int KeySym;

typedef struct{
    int flags;
    int min_width;
    int min_height;
    int max_width;
    int max_height;
} XSizeHints;

typedef struct{
    char *res_name;
    char *res_class;
} XClassHint;

typedef int XTextProperty;

typedef struct{
    int initial_state;
    int input;
    int icon_pixmap;
    int icon_window;
    int flags;
} XWMHints;

typedef struct{
    Colormap colormap;
    int backing_store;
} XSetWindowAttributes;


#endif
