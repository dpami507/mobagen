#include "Catcher.h"
#include "World.h"

Point2D Catcher::Move(CatWorld* world) {
  auto side = world->getWorldSideSize() / 2;
  for (;;) {
    auto path = generatePath(world);
    auto s = path.size();
    if (s <= 0)
    {
        std::cout << "No Path found\n";
        return {};
    }

    // check through all points and see if there is one with one exit
        // if there is, block in the cat
    // if there isn't try and make one

    auto p = path.back();
    auto cat = world->getCat();

    if (cat.x != p.x || cat.y != p.y && !world->getContent(p))
    {
        return p;
    }
  }
}
