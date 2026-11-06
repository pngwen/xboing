// TODO: typedefs we have to make to get past compiler errors, these all
//       indicate things we need to change
#ifndef FAKETYPES_H
#define FAKETYPES_H

//#include <raylib.h>

// Only define these types if X11 is not present
#ifndef X11_TYPES_DEFINED
#ifndef _X11_XLIB_H_
typedef int Display;
typedef int Window;
typedef int Colormap;
typedef int KeySym;
typedef int Pixmap;
typedef int GC;
typedef int Region;
typedef int Cursor;
typedef int Drawable;
typedef int Font;
typedef int Screen;
typedef int Atom;
typedef int Bool;
#endif
typedef unsigned long int u_long;
// Only define uid_t if not already defined
#ifndef _SYS_TYPES_H
typedef int uid_t;
#endif
#endif

// Raylib Types
// The archived X11 sources only reference textures as opaque handles, so an
// integer stand-in is enough for compile-only checks (and keeps `if (!texture)`
// in error.c valid). Do not include raylib.h here; its Texture2D is a struct.
typedef int Texture2D;
Texture2D LoadTexture(const char *fileName);
void DrawTexture(Texture2D texture, int posX, int posY, int tint);
void ClearBackground(int color);

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
#define BLACK 0
#define WHITE 0
#define CopyFromParent 0
#define CurrentTime 0
#define PointerRoot 0

// Cursor shapes
#define XC_watch   0
#define XC_plus    0
#define XC_hand2   0
#define XC_pirate  0

// Colormap / visual allocation
#define PseudoColor 0
#define DirectColor 1
#define TrueColor   2
#define AllocNone   0

// Pointer grabbing and close-down modes
#define GrabModeAsync 0
#define GrabSuccess   0
#define DestroyAll    0

// Graphics context macros
#define GXcopy 0
#define GXxor  1
#define GXand  2
#define GXor   3
#define FillTiled 0
#define GCFunction         (1L<<0)
#define GCFillStyle        (1L<<1)
#define GCGraphicsExposures (1L<<4)


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

// ...existing code...

// Only define XPoint and XColor if not already defined by X11
#ifndef _X11_XLIB_H_
typedef struct{
    short x, y;
} XPoint;

typedef struct {
    int pixel;
} XColor;
#endif

// GC Values (from X11)

// Only define these enums if not already defined by X11
#ifndef _X11_XLIB_H_
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
#endif

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

// CURSOR_PLUS / CURSOR_POINT are defined by init.h.

// Only define XButton and XEvent if not already defined by X11
#ifndef _X11_XLIB_H_
typedef struct {
    int button;
    int subwindow;
} XButton;

typedef struct {
    XButton xbutton;
} XEvent;
#endif
// Only define these X11-related structs if not already defined by X11
#ifndef _X11_XLIB_H_
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

typedef struct {
    int function;
    int fill_style;
    int graphics_exposures;
} XGCValues;

typedef struct {
    int visual;
    int visualid;
    int screen;
    int depth;
    int c_class;
    unsigned long red_mask;
    unsigned long green_mask;
    unsigned long blue_mask;
    int colormap_size;
    int bits_per_rgb;
} XVisualInfo;

typedef struct {
    int type;
    Display *display;
    Window resourceid;
    unsigned long serial;
    int error_code;
    int request_code;
    int minor_code;
} XErrorEvent;

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
//    int flags;
//} XWMHints;

// typedef struct{
//     Colormap colormap;
//     int backing_store;
// } XSetWindowAttributes;

/*
 * Compile-only Xlib declarations.
 *
 * The archived sources no longer include the real X11 headers. These
 * declarations (with unspecified arguments) let the code build on compilers
 * that reject implicit function declarations. They are never linked or run.
 */
int XAllocColor(Display *display, Colormap colormap, XColor *screen);
int XChangeWindowAttributes(Display *display, Window window,
	unsigned long valuemask, XSetWindowAttributes *attributes);
int XClearArea(Display *display, Window window, int x, int y,
	unsigned int width, unsigned int height, int exposures);
int XClearWindow(Display *display, Window window);
int XCopyArea(Display *display, Drawable src, Drawable dest, GC gc,
	int src_x, int src_y, unsigned int width, unsigned int height,
	int dest_x, int dest_y);
Colormap XCreateColormap(Display *display, Window window, int visual,
	int alloc);
Cursor XCreateFontCursor(Display *display, unsigned int shape);
GC XCreateGC(Display *display, Drawable drawable, unsigned long valuemask,
	XGCValues *values);
Pixmap XCreatePixmap(Display *display, Drawable drawable, unsigned int width,
	unsigned int height, unsigned int depth);
Cursor XCreatePixmapCursor(Display *display, Pixmap source, Pixmap mask,
	XColor *foreground, XColor *background, unsigned int x, unsigned int y);
Window XCreateSimpleWindow(Display *display, Window parent, int x, int y,
	unsigned int width, unsigned int height, unsigned int border_width,
	unsigned long border, unsigned long background);
Colormap XDefaultColormap(Display *display, int screen);
int XDefaultScreen(Display *display);
int XDefaultScreenOfDisplay(Display *display);
int DefaultColormap(Display *display, int screen);
int DefaultDepth(Display *display, int screen);
int DefaultScreen(Display *display);
int DoesBackingStore(int screen);
int XDefineCursor(Display *display, Window window, Cursor cursor);
int XDrawLine(Display *display, Drawable drawable, GC gc, int x1, int y1,
	int x2, int y2);
int XDrawLines(Display *display, Drawable drawable, GC gc, XPoint *points,
	int npoints, int mode);
int XDrawString(Display *display, Drawable drawable, GC gc, int x, int y,
	const char *string, int length);
void XFlush(Display *display);
void XFreeCursor(Display *display, Cursor cursor);
void XFreeFont(Display *display, XFontStruct *font);
void XFreeGC(Display *display, GC gc);
void XFreePixmap(Display *display, Pixmap pixmap);
int XGetErrorText(Display *display, int code, char *buffer, int length);
int XGetWindowAttributes(Display *display, Window window,
	XWindowAttributes *attributes);
int XGrabPointer(Display *display, Window window, int owner_events,
	unsigned int event_mask, int pointer_mode, int keyboard_mode,
	Window confine_to, Cursor cursor, unsigned long time);
int XInstallColormap(Display *display, Colormap colormap);
XFontStruct *XLoadQueryFont(Display *display, const char *name);
int XMapWindow(Display *display, Window window);
int XMatchVisualInfo(Display *display, int screen, int depth, int c_class,
	XVisualInfo *visual_info);
long XMaxRequestSize(Display *display);
int XMoveWindow(Display *display, Window window, int x, int y);
Display *XOpenDisplay(const char *display_name);
int XParseColor(Display *display, Colormap colormap, const char *spec,
	XColor *color);
int XQueryColor(Display *display, Colormap colormap, XColor *color);
int XQueryPointer(Display *display, Window window, Window *root,
	Window *child, int *root_x, int *root_y, int *win_x, int *win_y,
	unsigned int *mask);
int XReconfigureWMWindow(Display *display, Window window, int screen,
	unsigned int value_mask, XWindowChanges *changes);
Window RootWindow(Display *display, int screen);
int XSelectInput(Display *display, Window window, long event_mask);
int XSetBackground(Display *display, GC gc, unsigned long background);
int XSetClipMask(Display *display, GC gc, Pixmap pixmap);
int XSetClipOrigin(Display *display, GC gc, int x, int y);
int XSetCloseDownMode(Display *display, int close_mode);
int XSetErrorHandler(int (*handler)(Display *, XErrorEvent *));
int XSetFont(Display *display, GC gc, Font font);
int XSetForeground(Display *display, GC gc, unsigned long foreground);
int XSetLineAttributes(Display *display, GC gc, unsigned int line_width,
	int line_style, int cap_style, int join_style);
void XSetWMNormalHints(Display *display, Window window,
	XSizeHints *hints);
void XSetWMProperties(Display *display, Window window,
	XTextProperty *window_name, XTextProperty *icon_name, char **argv,
	int argc, XSizeHints *normal_hints, XWMHints *wm_hints,
	XClassHint *class_hints);
int XSetWindowBackground(Display *display, Window window,
	unsigned long background);
int XSetWindowBackgroundPixmap(Display *display, Window window,
	Pixmap pixmap);
int XSetWindowBorder(Display *display, Window window,
	unsigned long border);
int XStringListToTextProperty(void *list, int count, XTextProperty *prop);
int XSync(Display *display, int discard);
int XSynchronize(Display *display, int onoff);
int XTextWidth(XFontStruct *font, const char *string, int count);
int XUngrabPointer(Display *display, unsigned long time);
int XUninstallColormap(Display *display, Colormap colormap);
int WhitePixel(Display *display, int screen);

#endif
