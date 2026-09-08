#include "JohnConway.h"

// Reference: https://playgameoflife.com/info
void JohnConway::Step(World& world) {
  // todo: implement
    int sideLength = world.SideSize();
  for (int x = 0; x < sideLength; x++)
  {
      for (int y = 0; y < sideLength; y++)
      {
          Point2D point(x, y);
          int neighborCount = CountNeighbors(world, point);
      }
  }
}

int JohnConway::CountNeighbors(World& world, Point2D point) {
  // todo: implement
    int startX = point.x - 1;
    int endX = point.x + 1;

    int startY = point.y - 1;
    int endY = point.y + 1;

    int count = 0;
    for (int x = startX; x <= endX; x++)
    {
        for (int y = startY; y < endY; y++)
        {
            Point2D _point(x, y);
            if (world.Get(_point))
                count++;
        }
    }
  return count;
}
