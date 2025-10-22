#ifndef _PREVIEW_H_
#define _PREVIEW_H_

/**
 * @file preview.h
 * @author Justin C. Kibell (jck@techrescue.org)
 * @brief Manages the preview mode for xboing
 * @version 1.1.1.1
 * @date 16 Dec 1994
 * 
 * @copyright Copyright (c) 1993, 1994, 1995, Justin C. Kibell, All Rights Reserved 
 * @copyright (see COPYRIGHT file for full text)
 * 
 * preview.h/c manage the preview mode for xboing. The files call functions to load a level and set up the preview.
 *  
 */

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
 * $Id: preview.h,v 1.1.1.1 1994/12/16 01:36:55 jck Exp $
 * $Source: /usr5/legends/jck/xb/master/xboing/include/preview.h,v $
 * $Revision: 1.1.1.1 $
 * $Date: 1994/12/16 01:36:55 $
 *
 * $Log: preview.h,v $
 * Revision 1.1.1.1  1994/12/16  01:36:55  jck
 * The XBoing distribution requires configuration management. This is why the
 * cvs utility is being used. This is the initial import of all source etc..
 *
 *
 * =========================================================================
 */

/*
 *  Dependencies on other include files:
 */

//#include <X11/Xlib.h>
#include "faketypes.h"

/*
 *  Constants and macros:
 */

/*
 *  Type declarations:
 */

enum PreviewStates 
{ 
	PREVIEW_LEVEL, 
	PREVIEW_TEXT, 
	PREVIEW_WAIT, 
	PREVIEW_FINISH 
};

/*
 *  Function prototypes:
 */


/**
 * @brief Calls ResetPreviewLevel function in order to set the view mode back to preview mode.
 *
 * @param Display *display X11 display
 * @param Window window X11 window ID
 * @param Colormap colormap X11 colormap
 *
 * @todo Remove reliance on X11
 *
 */
void SetUpPreviewLevel(Display *display, Window window, Colormap colormap);

/**
 * @brief Creates the preview level by loading the level, setting up the display, and drawing the special blocks
 *
 * @param Display *display X11 display
 * @param Window window X11 window ID
 * @param Colormap colormap X11 colormap
 *
 * @todo Remove reliance on X11
 * @todo Possibly split this function into smaller functions
 *
 */
void PreviewLevel(Display *display, Window window);

/**
 * Calls DoLoadLevel and DoText in order to load the level and display the preview text
 *
 * @param Display *display X11 display
 * @param Window window X11 window ID
 *
 * @todo Remove reliance on X11
 *
 */
void RedrawPreviewLevel(Display *display, Window window);

/**
 * @brief Frees memory for preview level
 *
 * @param Display *display X11 display
 * @param Window window X11 window ID
 *
 * @todo Remove reliance on X11
 * @todo Consider deleting this function
 *
 */
void FreePreviewLevel(Display *display);

/**
 * @brief Sets the preview state to preview mode
 *
 */
void ResetPreviewLevel(void);
extern 	enum PreviewStates PreviewState;


#endif
