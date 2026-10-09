#include "Cat.h"
#include "World.h"
#include <stdexcept>

#include <unordered_map>
#include <unordered_set>
#include <queue>

using namespace std;

std::vector<Point2D> generateCatPath(CatWorld* w);

Point2D Cat::Move(CatWorld* world) {
  auto rand = Random::Range(0, 5);
  auto pos = world->getCat();

  auto path = generateCatPath(world);
  auto s = path.size();
  if (s <= 0)
  {
      std::cout << "No Path found\n";
      return {};
  }

  world->lastMove = path.back();
  return path.back();
}

struct Node
{
    Point2D point;
    uint8_t hueristic;
};
inline bool operator>(const Node& a, const Node& b)
{
    return (a.hueristic > b.hueristic);
}

const int SEARCH_DIST = 1;

float calcCatHeuristic(CatWorld* world, Point2D p)
{
    int distFromEdge = min((world->getWorldSideSize() / 2 - abs(p.x)), (world->getWorldSideSize() / 2 - abs(p.y)));
    
    int nearbyWalls = 0;
    for (int i = -SEARCH_DIST; i < SEARCH_DIST; i++)
    {
        for (int j = -SEARCH_DIST; j < SEARCH_DIST; j++)
        {
            if (!world->isValidPosition({p.x + i, p.y + j})) continue;

            if (world->getContent({p.x + i, p.y + j}))
                nearbyWalls++;
        }
    }
    
    return distFromEdge + nearbyWalls;
}

// returns a vector of neighbors that are not visited, not cat, not block, not in the queue
std::vector<Point2D> getVisitableNeightbors(CatWorld* w, const Point2D& p, const std::unordered_set<Point2D>& f, const std::unordered_map<Point2D, bool>& v)
{
    std::vector<Point2D> neighbors;

    std::random_device rd;
    std::mt19937 g(rd());

    auto neigs = w->neighbors(p);
    std::shuffle(neigs.begin(), neigs.end(), g);

    for (auto pos : neigs)
    {
        // push back if
        if (w->isValidPosition(pos) && // its a valid move
            !w->getContent(pos) &&     // there is no wall
            w->getCat() != pos &&      // the cat isn't there
            v.at(pos) == false &&      // not visited
            !f.contains(pos)           // not in the frontier
        )
        {
            neighbors.push_back(pos);
        }
    }

    return neighbors;
}

std::vector<Point2D> generateCatPath(CatWorld* w)
{
    unordered_map<Point2D, Point2D> cameFrom;                             // path
    priority_queue<Node, std::vector<Node>, std::greater<Node>> frontier; // aka open list
    unordered_set<Point2D> frontierSet;                                   // also open list
    unordered_map<Point2D, bool> visited;                                 // aka closed list

    cameFrom.clear();
    frontier = {};
    frontierSet.clear();
    visited.clear();

    // bootstrap state
    auto catPos = w->getCat();

    // set up the frontier
    frontier.push({catPos, 0});
    frontierSet.insert(catPos);
    std::optional<Point2D> borderExit;

    // set up visited
    float halfSize = w->getWorldSideSize() / 2.0f;
    for (int i = -halfSize; i < halfSize; i++)
        for (int j = -halfSize; j < halfSize; j++)
            visited[Point2D(i, j)] = false;

    while (!frontier.empty())
    {
        // get the current from frontier
        auto current = frontier.top().point;

        // if we find a visitable border, break the loop
        // early exit
        if (w->catWinsOnSpace(current))
        {
            std::cout << "cat win found: " << current.x << ", " << current.y << "\n";
            borderExit = current;
            break;
        }

        // remove the current from frontierset
        frontierSet.erase(current);
        frontier.pop();
        // mark current as visited
        visited[current] = true;
        // getVisitableNeightbors(world, current) returns a vector of neighbors that are not visited, not cat, not block, not in the queue
        auto neighbors = getVisitableNeightbors(w, current, frontierSet, visited);
        // iterate over the neighs:
        if (neighbors.size() == 0)
            continue;
        for (auto n : neighbors)
        {
            // for every neighbor set the cameFrom
            cameFrom[n] = current;

            // enqueue the neighbors to frontier and frontierset
            float h = calcCatHeuristic(w, n);
            frontier.push(Node(n, h));
            frontierSet.emplace(n);
        }
    }

    if (!borderExit.has_value())
        return {};

    Point2D cursor = borderExit.value();
    std::vector<Point2D> path;
    while (cursor != catPos)
    {
        path.push_back(cursor);
        cursor = cameFrom[cursor];
    }
    // if the border is not infinity, build the path from border to the cat using the camefrom map
    // if there isnt a reachable border, just return empty vector
    // if your vector is filled from the border to the cat, the first element is the catcher move, and the last element is the cat move
    return path;
}
