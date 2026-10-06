#include "Cat.h"
#include "World.h"
#include <stdexcept>

Point2D Cat::Move(CatWorld* world) {
  auto rand = Random::Range(0, 5);
  auto pos = world->getCat();

  auto path = generatePath(world);
  auto s = path.size();
  if (s <= 0)
  {
      std::cout << "No Path found\n";
      return {};
  }

  auto p = path.front();
  std::cout << "Cat ----------------\n";
  std::cout << "Cat moving from: " << pos.x << ", " << pos.y << '\n';
  std::cout << "Cat moving to: " << path.back().x << ", " << path.back().y << '\n';
  return path.back();
}
