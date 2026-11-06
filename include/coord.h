#ifndef _COORD_H_
#define _COORD_H_

#include <raylib.h>

/*
 * Integer cartesian coordinate.
 *
 * Plenty of positions in xboing are whole pixels - the origin of a block,
 * the corners of the play area, the point a ball spawns on - but raylib's
 * Vector2 stores floating point components. Coord ties the two components
 * together so that a location can be passed around as a single argument
 * instead of two loose ints.
 *
 * Floating point positions are still needed for the ball, which travels at
 * fractional speeds, so Vector2 remains in use wherever sub-pixel precision
 * matters.
 */
typedef struct {
    int x;
    int y;
} Coord;

/*
 * @brief Builds a coordinate from its components
 *
 * @param x horizontal component in pixels
 * @param y vertical component in pixels
 *
 * @return the coordinate
 */
Coord MakeCoord(int x, int y);

/*
 * @brief Converts a coordinate to raylib's float vector
 *
 * @param coord the coordinate to convert
 *
 * @return the equivalent Vector2
 */
Vector2 CoordToVector2(Coord coord);

/*
 * @brief Converts raylib's float vector to a coordinate
 *
 * Any fractional part is truncated towards zero.
 *
 * @param vector the vector to convert
 *
 * @return the equivalent coordinate
 */
Coord Vector2ToCoord(Vector2 vector);

#endif // _COORD_H_
