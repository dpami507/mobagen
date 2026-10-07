#include "Catcher.h"
#include "World.h"

std::unordered_map<Point2D, std::function<std::vector<Point2D>(CatWorld* world, Point2D point)>> wallOffDir = {
    {{Point2D( 0,  1)}, [](CatWorld* w, Point2D p) { return std::vector<Point2D>({w->NW(p), w->SE(p)}); }},
    {{Point2D( 1,  0)}, [](CatWorld* w, Point2D p) { return std::vector<Point2D>({w->NW(p), w->SW(p)}); }},
    {{Point2D( 0, -1)}, [](CatWorld* w, Point2D p) { return std::vector<Point2D>({w->NE(p), w->SW(p)}); }},
    {{Point2D(-1, -1)}, [](CatWorld* w, Point2D p) { return std::vector<Point2D>({w->NW(p), w->SE(p)}); }},
    {{Point2D(-1,  0)}, [](CatWorld* w, Point2D p) { return std::vector<Point2D>({w->NE(p), w->SE(p)}); }},
    {{Point2D(-1,  1)}, [](CatWorld* w, Point2D p) { return std::vector<Point2D>({w->NE(p), w->SW(p)}); }},
};
std::vector<Point2D> getWallBasedOnDir(CatWorld* world, Point2D point, Point2D dir)
{
    if (wallOffDir.find(dir) != wallOffDir.end())
        return wallOffDir[dir](world, point);
    else
        return {world->NE(point), world->SW(point)};
}

Point2D Catcher::Move(CatWorld* world) {
  auto side = world->getWorldSideSize() / 2;
  for (;;) {
    auto path = generatePath(world);
    auto dist = path.size();

    if (dist <= 0)
    {
        std::cout << "No Path found\n";
        return {};
    }

    Point2D predictedLast = path.front();
    Point2D predictedFirst = path.back();

    Point2D nextPlacement = predictedLast;

    bool found = false;
    for (int i = 0; i < dist - 1; i++)
    {
        // break if we found a wall placement
        if (found) break;

        if (dist >= 4)
        {
            Point2D p = path[i];
            Point2D lp = path[i + 1];

            Point2D dir(lp.x - p.x, lp.y - p.y);
            for (auto w : getWallBasedOnDir(world, p, dir))
            {
                // if its not valid return
                if (!world->isValidPosition(w))
                    continue;

                if (!world->getContent(w))
                {
                    nextPlacement = w;
                    found = true;
                    break;
                }
            }
        }
        else
        {
            // check how many exits it has
            // if its one then box them in
            // if its more do some shenanagins to trap it in 4 moves
        }
    }

    Point2D cat = world->getCat();

    if (cat.x != nextPlacement.x || cat.y != nextPlacement.y && !world->getContent(nextPlacement))
    {
        return nextPlacement;
    }
  }
}
