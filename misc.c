/*
 * XBoing - An X11 blockout style computer game
 *
 * (c) Copyright 1993, 1994, 1995, Justin C. Kibell, All Rights Reserved
 *
 * The X Consortium, and any party obtaining a copy of these files from
 * the X Consortium, directly or indirectly, is granted, free of charge, a
 * full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 * nonexclusive right and license to deal in this software and
 * documentation files (the "Software"), including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons who receive
 * copies from any such party to do so.  This license includes without
 * limitation a license to do the foregoing actions under any patents of
 * the party supplying this software to the X Consortium.
 *
 * In no event shall the author be liable to any party for direct, indirect,
 * special, incidental, or consequential damages arising out of the use of
 * this software and its documentation, even if the author has been advised
 * of the possibility of such damage.
 *
 * The author specifically disclaims any warranties, including, but not limited
 * to, the implied warranties of merchantability and fitness for a particular
 * purpose.  The software provided hereunder is on an "AS IS" basis, and the
 * author has no obligation to provide maintenance, support, updates,
 * enhancements, or modifications.
 */

/* 
 * =========================================================================
 *
 * $Id: misc.c,v 1.1.1.1 1994/12/16 01:36:47 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/misc.c,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:47 $
 *
 * $Log: misc.c,v $
 * Revision 1.1.1.1  1994/12/16  01:36:47  jck
 * The XBoing distribution requires configuration management. This is why the
 * cvs utility is being used. This is the initial import of all source etc..
 *
 *
 * =========================================================================
 */

/*
 *  Include file dependencies:
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/param.h>
#include <pwd.h>

// TODO: Remove functional reliance on X11
// #include <X11/Xlib.h>
// #include <X11/Xutil.h>
// #include <X11/Xos.h>

#include <netinet/in.h>
#include <string.h> // Added for implementation of strcpy()

#include "init.h"
#include "error.h"
#include "stage.h"
#include "dialogue.h"

#include "misc.h"

#ifdef NEED_USLEEP
/**
 * @brief Halts program execution for a specified number of microseconds
 * 
 * @param usec The number of microseconds to sleep
 */
void usleep(unsigned long usec)
{
#ifdef SYSV
#ifdef __clipper__
    struct timeval tv;
    tv.tv_sec=((usec)/1000);
    tv.tv_usec=(((usec)%1000)*1000);
    select(1,NULL,NULL,NULL,&tv);
#else
    poll((struct poll *) 0, (size_t) 0, usec / 1000);   /* ms resolution */
#endif
#else
    struct timeval timeout;
    timeout.tv_usec = usec % (unsigned long) 1000000;
    timeout.tv_sec = usec / (unsigned long) 1000000;
    select(0, (void *) 0, (void *) 0, (void *) 0, &timeout);
#endif
}
#endif

/**
 * @brief Stops program execution (usleep()) if needed to meet target framerate.
 * 
 * @param display The display of the current X11 window
 * @param ms The number of miliseconds to wait between each frame
 */
void sleepSync(Display *display, unsigned long ms)
{
    struct timeval st, et;
    long SyncTime;
    static unsigned long accu;// Unitialized static variable could lead to ambiguous behavior on first function call

    gettimeofday(&st, NULL);
    XSync(display, False);
    gettimeofday(&et, NULL);

    SyncTime = (((et.tv_sec - st.tv_sec) * 1000) +
               ((et.tv_usec - st.tv_usec) / 1000) );

/*    if ((ms) > ((1000 / 60) + SyncTime))
        usleep(ms - SyncTime);
*/
    if ((ms + accu) > ((1000 / 30) + SyncTime)) 
    {
        usleep(ms +accu - SyncTime);
        accu = 0;
    }
    else if (ms > SyncTime)
       accu += (ms - SyncTime);
}

/**
 * @brief Draws a formatted line between 2 points using X11 library calls
 * 
 * @param display The display of the current X11 window
 * @param window  The X11 window to draw on
 * @param x The x-coordinate of the first point of the line
 * @param y The y-coordinate of the first point of the line
 * @param x2 The x-coordinate of the second point of the line
 * @param y2 The y-coordinate of the second point of the line
 * @param colour The color of the line
 * @param width The width of the line (in px)
 */
void DrawLine(Display *display, Window window, int x, int y, int x2, int y2, 
	int colour, int width)
{
	/* Change the width of the line */
	XSetLineAttributes(display, gcxor, width, LineSolid, CapProjecting, 
		JoinMiter);
	XSetLineAttributes(display, gcand, width, LineSolid, CapProjecting, 
		JoinMiter);

	/* Set to the desired colours */
	XSetBackground(display, gcxor, colour);
	XSetForeground(display, gcxor, colour);

	/* Now draw the line */
	XDrawLine(display, window, gcxor, x, y, x2, y2);
	XDrawLine(display, window, gcand, x, y, x2, y2);
	XDrawLine(display, window, gcxor, x, y, x2, y2);
}

/**
 * @brief Draws text centered with a 2px offset shadow
 * 
 * @param display The display of the current X11 window
 * @param window The X11 window to draw on
 * @param font The font of the text
 * @param string The text to draw
 * @param y The y-coordinate of the text
 * @param colour The color of the text
 * @param width The width of the area to draw the centered text in
 */
void DrawShadowCentredText(Display *display, Window window, XFontStruct *font,
	char *string, int y, int colour, int width)
{
    int plen, len, x;
 
	/* String length */
    len = strlen(string);

	/* Length of string in pixels */
    plen = XTextWidth(font, string, len);

	/* Start drawing so the text is centered */
    x = (width / 2) - (plen / 2);

	/* Draw the text with a shadow */
    DrawText(display, window, x+2, y + 2, font, black, string, -1);
    DrawText(display, window, x, y, font, colour, string, -1);
}

/**
 * @brief Draws text with a 2px offset shadow
 * 
 * @param display The display of the current X11 window
 * @param window The X11 window to draw on
 * @param font The font of the text
 * @param string The text to draw
 * @param x The x-coordinate of the text
 * @param y The y-coordinate of the text
 * @param colour The color of the text
 */
void DrawShadowText(Display *display, Window window, XFontStruct *font,
	char *string, int x, int y, int colour)
{
    int len;

	/* String length */
    len = strlen(string);

	/* Draw the text with a shadow */
    DrawText(display, window, x+2, y + 2, font, black, string, -1);
    DrawText(display, window, x, y, font, colour, string, -1);
}

/**
 * @brief Draws text (single layer)
 * 
 * @param display The display of the current X11 window
 * @param window The X11 window to draw on
 * @param font The font of the text
 * @param string The text to draw
 * @param x The x-coordinate of the text
 * @param y The y-coordinate of the text
 * @param colour The color of the text
 * @param numChar The number of characters of "string" to draw
 */
void DrawTextFast(Display *display, Window window, int x, int y, XFontStruct *font, 
	int colour, char *text, int numChar)
{
	int len = strlen(text);

	/* If numchar is passed > 0 then only draw numChar characters */
	if (numChar > 0) 
		len = numChar;

	/* Change to the new font */
	XSetFont(display, gccopy, font->fid);

	/* Change the drawing function */
	XSetForeground(display, gccopy, colour);

	/* Draw the string into the drawable */
	XDrawString(display, window, gccopy, x, y + font->ascent, text, len);
}

/**
 * @brief Draws text (multi-layer, smoother)
 * 
 * @param display The display of the current X11 window
 * @param window The X11 window to draw on
 * @param font The font of the text
 * @param string The text to draw
 * @param x The x-coordinate of the text
 * @param y The y-coordinate of the text
 * @param colour The color of the text
 * @param numChar The number of characters of "string" to draw
 */
void DrawText(Display *display, Window window, int x, int y, XFontStruct *font, 
	int colour, char *text, int numChar)
{
	int len = strlen(text);

	/* If numchar is passed > 0 then only draw numChar characters */
	if (numChar > 0) 
		len = numChar;

	/* Change to the new font */
	XSetFont(display, gcxor, font->fid);
	XSetFont(display, gcand, font->fid);

	/* Change the drawing function */
	XSetBackground(display, gcxor, colour);
	XSetForeground(display, gcxor, colour);

	/* Draw the string into the drawable */
	XDrawString(display, window, gcxor, x, y + font->ascent, text, len);
	XDrawString(display, window, gcand, x, y + font->ascent, text, len);
	XDrawString(display, window, gcxor, x, y + font->ascent, text, len);
}

/**
 * @brief Draws a pixmap (and can also clear background)
 * 
 * @param display The display of the current X11 window
 * @param window The X11 window to draw on
 * @param pixmap The pixmap to draw
 * @param mask The clip-mask of the pixmap
 * @param x The x-coordinate of the pixmap
 * @param y The y-coordinate of the pixmap
 * @param w The width of the pixmap
 * @param h The height of the pixmap
 * @param clear Clear the background before drawing?
 */
void RenderShape(Display *display, Window window, Pixmap pixmap, 
	Pixmap mask, int x, int y, int w, int h, int clear)
{
	/* Clear the background first? */
    if (clear) XClearArea(display, window, x, y, w, h, False);

	/* Set to dest x and y clip origin */
    XSetClipOrigin(display, gc, x, y); 

	/* Set the clipping mask */
    XSetClipMask(display, gc, mask);   

    XCopyArea(display, pixmap, window, gc, 0, 0, w, h, x, y);

	/* Unset clip (or add a clip gc) */
    XSetClipMask(display, gc, None);   
}

/**
 * @brief Obtains color data from X11 library (Used for initialization)
 * 
 * @param window The X11 window to draw on
 * @param pixmap The pixmap to draw
 * @param colourName The name of the color to find color data for
 * @return int The color-data of the given color name
 */
int ColourNameToPixel(Display *display, Colormap colormap, char *colourName)
{
    XColor colour;

    /* Obtain the exact colour from the colour name */
    if (XParseColor(display, DefaultColormap(display,
        XDefaultScreen(display)), colourName, &colour) != 0)
    {
        /* Now allocate the colour */
        if (XAllocColor(display, colormap, &colour) != 0)
		{
        	/* Success - return the pixel id */
        	return colour.pixel;
		}
    }

    /* Obviously a problem so barf */
    ShutDown(display, 1, "Error while parsing colours.");

    /* NOT REACHED */
    return 1;
}

/**
 * @brief Parses the user's name from their password file
 * 
 * @return char* The user's name 
 */
char *getUsersFullName(void)
{
    struct passwd *pass;
    char *comma;
    char *cp1, *cp2;
    static char fullname[80];

    /* Get user information from password file */
    if (!(pass = getpwuid(getuid())))
        return("Anonymous?");       /* Unknown user oops. */

    /* find a comma indicating further info after name */
    comma = strchr(pass->pw_gecos, ',');

    /* NULL out the comma */
    if (comma) *comma = '\0';

    /* Use the nickname if not null otherwise password file name */
    cp1 = pass->pw_gecos;
    cp2 = fullname;

    /* Search through the gecos field looking for an '&' which on very
     * old UNIX systems is supposed to be the users user name with the
     * first letter uppercased.
     */
    while(*cp1)
    {
        /* Look for the '&' symbol */
        if(*cp1 != '&')
            *cp2++ = *cp1++;
        else
        {
            /* A ha. Now copy the users name to be in place of '&' */
            strcpy(cp2, pass->pw_name);
       
            /* Convert the first letter to uppercase. */
            if(islower(*cp2))
                *cp2 = toupper(*cp2);

            /* Continue with the remaining string */
            while(*cp2) cp2++;
                cp1++;
        }
    }

    /* Return their name without any trailing stuff */
    return(fullname);
}

/**
 * @brief Returns the path of the home directory
 * 
 * @return char* The path of the Home directory
 */
char *GetHomeDir(void)
{
    int uid;
    struct passwd *pw;
    register char *ptr;
    static char dest[MAXPATHLEN];

    /* This function will return the home directory of the user
     * by either using the HOME environment variable or constructing
     * it through the USER environment variable.
     */

    if ((ptr = getenv("HOME")) != NULL)
        (void) strcpy(dest, ptr);
    else
    {
        /* HOME variable is not present so get USER var */
        if ((ptr = getenv("USER")) != NULL)
            pw = getpwnam(ptr);
        else
        {
            /* Obtain user id etc. */
            uid = getuid();
            pw = getpwuid(uid);
        }

        if (pw)
            (void) strcpy(dest, pw->pw_dir);
        else
            *dest = '\0';
    }

    /* This will be NULL on error or the actual path */
    return dest;
}

/**
 * @brief Resizes the main window (returns the success of resize)
 * 
 * @param display The display of the current X11 window
 * @param window The X11 window to draw on
 * @param width The width of the window
 * @param height The height of the window
 * @return int Whether the window could be resized or not. (0/1)
 */
int ResizeMainWindow(Display *display, Window window, int width, int height)
{
    XWindowChanges values;
    unsigned int value_mask;
	int screen = XDefaultScreen(display);

    values.width = width;
    values.height = height;
    value_mask = CWWidth | CWHeight;

    if (XReconfigureWMWindow(display, window, screen, value_mask, &values) == 0)
		return False;

	sleepSync(display, 100);
	return True;
}

/**
 * @brief Obtains the current window size & propogates it (width&height) to parameter variables.
 * 
 * @param display The display of the current X11 window
 * @param window The X11 window to draw on
 * @param width The variable to propogate the window width to
 * @param height The variable to propogate the window height to
 * @return int (nothing)
 * 
 * @todo Add return values for function or make return type void
 */
int ObtainWindowWidthHeight(Display *display, Window window, 
	int *width, int *height)
{
    XWindowAttributes attributes;

    if (XGetWindowAttributes(display, window, &attributes) == 0)
		ErrorMessage("Unable to obtain window attributes.");

    *width = attributes.width;
    *height = attributes.height;
}

/**
 * @brief Updates parameter varibles with mouse position (set to 0 if cursor is off-window)
 * 
 * @param display The display of the current X11 window
 * @param window The X11 window to draw on
 * @param x The variable to propogate the mouse x-position to
 * @param y The variable to propogate the mouse y-position to
 * @return int Whether or not the mouse is on the window
 */
int ObtainMousePosition(Display *display, Window window, int *x, int *y)
{
    int rx, ry, x1, y1;
    unsigned int mask;
    Window root, child;

    *x = 0;
    *y = 0;

    /* Obtain the position of the pointer in window */
    if (XQueryPointer(display, window, &root, &child,
        &rx, &ry, &x1, &y1, &mask) == True)
    {
        *x = x1;
        *y = y1;
		return True;
    }

	return False;
}

/**
 * @brief Prints a message to the user and prompts them with y/n. (Returns user answer)
 * 
 * @param display The display of the current X11 window
 * @param message The message to prompt the user with
 * @return int The user's response
 */
int YesNoDialogue(Display *display, char *message)
{
    char str[80];

    str[0] = '\0';

    /* Obtain a string from the user. Should contain NULL or numbers */
    strcpy(str, UserInputDialogueMessage(display, message, TEXT_ICON, 
		YES_NO_ENTRY));

    /* Nothing input so just return */
    if (str[0] == '\0') return False;

	DEBUG("YesNoDialogue() got an answer.");

	if (tolower(str[0]) == 'y') return True;
	if (tolower(str[0]) == 'n') return False;

	/* Just in case */
	return False;
}

/**
 * @brief Creates a scaled copy of the source Pixmap
 * 
 * @param display The display of the current X11 window
 * @param window The X11 window to draw on
 * @param source The source pixmap
 * @param swidth The width of the source pixmap
 * @param sheight The height of the source pixmap
 * @param dwidth The width of the output pixmap
 * @param dheight The height of the output pixmap
 * @return Pixmap The new scaled-copy of the source pixmap
 */
Pixmap ScalePixmap(Display *display, Window window, Pixmap source, 
	int swidth, int sheight, int dwidth, int dheight)
{
   	Pixmap 	temp, dest;
   	int 		j, end;
   	float 	i;
   	float 	xscale, yscale;

   	xscale = (float) swidth / (float) dwidth;         /* Scaling factors */
   	yscale = (float) sheight / (float) dheight;

   	dest = XCreatePixmap(display, window, dwidth, dheight, 
		DefaultDepth(display, XDefaultScreen(display)));
   	temp = XCreatePixmap(display, window, dwidth, sheight, 
		DefaultDepth(display, XDefaultScreen(display)));

   	j = 0;
   	end = dwidth * xscale;

   	/* Scale width of source into temp pixmap */
   	for(i = 0; i < end; i += xscale)
	{
      XCopyArea(display, source, temp, gccopy, 
		(int) i, 0, 1, sheight, j++, 0);
	}

   	j = 0;
   	end = dheight * yscale;

   	/* Scale height of temp into dest pixmap */
   	for(i = 0; i < end; i += yscale)
	{
      	XCopyArea(display, temp, dest, gccopy, 
			0, (int) i, dwidth, 1, 0, j++);
	}

   	XFreePixmap(display, temp);

   	return (Pixmap) dest;
}

/**
 * @brief Generates a list of points for a 4-point curve and draws lines connecting them
 * 
 * @param display The display of the current X11 window
 * @param window The X11 window to draw on
 * @param p The array of points of the 4-point curve
 * @param num_steps The number of steps of the curve (more steps = higher quality)
 */
void Draw4PointCurve(Display *display, Window window, XPoint *p, int num_steps)
{
    double t, t_sq, t_cb, incr;
    double r1, r2, r3, r4;
    ushort curve_x, curve_y;
	int ptIndex;
	long maxLines;
	XPoint 	*pts;

	/* 
	 * First check and see if the maximum number of points the X Server can
     * draw is less than the number of steps wanted - most likely to be ok.
     */

	maxLines = (double) (XMaxRequestSize(display) - 3) / 2.0;
	if (num_steps > maxLines)
		num_steps = maxLines;
	
    incr = 1.0 / (double) num_steps;

	if ((pts = (XPoint *) malloc(num_steps * sizeof(XPoint))) == 
		(XPoint *)0)
	{
    	/* Obviously a problem so barf */
    	ShutDown(display, 1, "Unable to malloc memory for Draw4PointCurve().");
	}

	ptIndex = 0;

	pts[ptIndex].x = p[0].x;
	pts[ptIndex].y = p[0].y;

    for (t = incr; t <= 1.01; t += incr)
    {
        t_sq = t * t;
        t_cb = t * t_sq;

        r1 = (1 - 3*t + 3*t_sq -   t_cb) * (double) p[0].x;
        r2 = (    3*t - 6*t_sq + 3*t_cb) * (double) p[1].x;
        r3 = (          3*t_sq - 3*t_cb) * (double) p[2].x;
        r4 = (                     t_cb) * (double) p[3].x;
        curve_x = (ushort) (r1 + r2 + r3 + r4);

        r1 = (1 - 3*t + 3*t_sq -   t_cb) * (double) p[0].y;
        r2 = (    3*t - 6*t_sq + 3*t_cb) * (double) p[1].y;
        r3 = (          3*t_sq - 3*t_cb) * (double) p[2].y;
        r4 = (                     t_cb) * (double) p[3].y;
        curve_y = (ushort) (r1 + r2 + r3 + r4);

		ptIndex++;
		pts[ptIndex].x = curve_x;
		pts[ptIndex].y = curve_y;
    }

	/* Now draw the curve based on a whole heap of lines */
	XDrawLines(display, window, gc, pts, ptIndex, CoordModeOrigin);

	/* Don't forget to free points */
	free(pts);
}