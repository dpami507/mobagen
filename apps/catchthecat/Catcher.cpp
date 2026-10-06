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
    auto p = path.front();

    std::cout << "Catcher ----------------\n";
    std::cout << s << '\n';
    std::cout << p.x << ", " << p.y << '\n';
    auto cat = world->getCat();

    if (cat.x != p.x || cat.y != p.y && !world->getContent(p))
    {
        return p;
    }
  }
}
