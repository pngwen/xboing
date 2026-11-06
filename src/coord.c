#include "coord.h"

Coord MakeCoord(int x, int y)
{
    Coord coord;

    coord.x = x;
    coord.y = y;

    return coord;
}

Vector2 CoordToVector2(Coord coord)
{
    Vector2 vector;

    vector.x = (float)coord.x;
    vector.y = (float)coord.y;

    return vector;
}

Coord Vector2ToCoord(Vector2 vector)
{
    return MakeCoord((int)vector.x, (int)vector.y);
}
