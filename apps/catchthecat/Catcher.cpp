#include "Catcher.h"
#include "World.h"

Point2D Catcher::Move(CatWorld* world) {
  auto side = world->getWorldSideSize() / 2;
  for (;;) {
    Point2D p = generatePath(world).front();
    auto cat = world->getCat();

    if (cat.x != p.x || cat.y != p.y && !world->getContent(p))
    {
        return p;
    }
  }
}
