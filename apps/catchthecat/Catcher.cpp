#include "Catcher.h"
#include "World.h"

std::unordered_map<Point2D, std::function<std::vector<Point2D>(CatWorld* world, Point2D point)>> nextWallForTunnel = {
    {{Point2D(1, 1)}, [](CatWorld* w, Point2D p) 
        { return std::vector<Point2D>({w->NW(p), w->NE(p), w->W(p), w->E(p), w->SW(p), w->SE(p)}); }},
    {{Point2D(1, -1)}, [](CatWorld* w, Point2D p) 
        { return std::vector<Point2D>({w->SE(p), w->SW(p), w->E(p), w->W(p), w->NE(p), w->NW(p)}); }},
    {{Point2D(-1, -1)}, [](CatWorld* w, Point2D p)
        { return std::vector<Point2D>({w->SW(p), w->SE(p), w->W(p), w->E(p), w->NW(p), w->NE(p)}); }},
    {{Point2D(-1,  1)}, [](CatWorld* w, Point2D p) 
        { return std::vector<Point2D>({w->NE(p), w->NW(p), w->E(p), w->W(p), w->SE(p), w->SW(p)}); }},
};

Point2D getCorner(CatWorld* world)
{
    int halfSize = world->getWorldSideSize() / 2;
    if (!world->getContent({halfSize, halfSize}))
        return {halfSize, halfSize};
    else if (!world->getContent({halfSize, -halfSize}))
        return {halfSize, -halfSize};
    else if (!world->getContent({-halfSize, -halfSize}))
        return {-halfSize, -halfSize};
    else if (!world->getContent({-halfSize, halfSize}))
        return {-halfSize, halfSize};
    else
        return {0, 0};
}

Point2D getPointToCreateTunnel(CatWorld* world, const std::vector<Point2D>& path, const std::unordered_set<Point2D>& set)
{
    Point2D first = path.back();
    Point2D last = path.front();

    Point2D roughDirection = {(first.x >= last.x) ? 1 : -1, (first.y >= last.y) ? 1 : -1};

    for (int i = 0; i < path.size(); i++)
    {
        // try all combos depending on position
        Point2D currentPoint = path[i];
        for (auto n : nextWallForTunnel[roughDirection](world, currentPoint))
        {
            if (world->isValidPosition(n) && !world->getContent(n) && !set.contains(n) && world->getCat() != n)
            {
                return n;
            }
        }
    }
    for (auto n : nextWallForTunnel[roughDirection](world, world->getCat()))
    {
        if (world->isValidPosition(n) && !world->getContent(n) && !set.contains(n) && world->getCat() != n)
        {
            return n;
        }
    }
    return last;
}

bool canTrapCat(CatWorld* world, const Point2D& nextCatMove)
{
    int neighborCount = 0;
    Point2D lastOpen;
    for (int j = 0; j < 6; j++)
    {
        Point2D current = world->neighbors(nextCatMove)[j];
        if (world->getContent(current))
        {
            lastOpen = current;
            neighborCount++;
        }
    }
    if (neighborCount == 5)
        return true;
    return false;
}

Point2D Catcher::Move(CatWorld* world) {
  auto side = world->getWorldSideSize() / 2;
  for (;;) {
    // Create path and set for easy lookup
    std::vector<Point2D> path = generatePath(world);
    std::unordered_set<Point2D> pathSet;
    for (Point2D n : path)
        pathSet.emplace(n);

    auto dist = path.size();

    if (dist <= 0)
    {
        std::cout << "No Path found\n";
        return {};
    }

    Point2D cat = world->getCat();
    Point2D predictedLast = path.front();
    Point2D predictedFirst = path.back();

    Point2D nextPlacement = predictedLast;

    bool found = false;
    // block corners to start
    if (dist >= 8)
    {
        Point2D p = getCorner(world);
        if (p.x != 0 && p.y != 0)
        {
            nextPlacement = p;
            found = true;
        }
    }
    if (!found)
    {
        // make sure the cat wont win on its next move
        if (world->catWinsOnSpace(predictedFirst))
            nextPlacement = predictedFirst;
        else if (canTrapCat(world, predictedFirst))
            nextPlacement = predictedFirst;
        else
            nextPlacement = getPointToCreateTunnel(world, path, pathSet);
    }

    if (cat.x != nextPlacement.x || cat.y != nextPlacement.y && !world->getContent(nextPlacement))
    {
        world->lastMove = nextPlacement;
        return nextPlacement;
    }
  }
}
