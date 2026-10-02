#include "Agent.h"
#include <climits>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "World.h"

using namespace std;

std::vector<Point2D> Agent::generatePath(CatWorld* w) {
  unordered_map<Point2D, Point2D> cameFrom;  // to build the flowfield and build the path
  queue<Point2D> frontier;                   // to store next ones to visit
  unordered_set<Point2D> frontierSet;        // OPTIMIZATION to check faster if a point is in the queue
  unordered_map<Point2D, bool> visited;      // use .at() to get data, if the element dont exist [] will give you wrong results

  // bootstrap state
  auto catPos = w->getCat();
  frontier.push(catPos);
  frontierSet.insert(catPos);
  Point2D borderExit = {INT32_MAX, INT32_MAX};  // sentinel: no border found yet

  // set visited
  visited.clear();
  for (int i = -w->getWorldSideSize(); i < w->getWorldSideSize(); i++)
  {
      for (int j = -w->getWorldSideSize(); j < w->getWorldSideSize(); j++)
      {
          visited[Point2D(i, j)] = false;
      }
  }

  while (!frontier.empty()) {
    // get the current from frontier
    auto current = frontier.front();
    // remove the current from frontierset
    frontierSet.erase(current);
    // mark current as visited
    visited[current] = true;
    // getVisitableNeightbors(world, current) returns a vector of neighbors that are not visited, not cat, not block, not in the queue
    auto neighbors = getVisitableNeightbors(w, current);
    // iterate over the neighs:
    if (neighbors.size() == 0)
        continue;
    for (auto n : neighbors)
    {
        if (visited.at(n) == true)
            continue;

        // for every neighbor set the cameFrom
        cameFrom[n] = current;

        // enqueue the neighbors to frontier and frontierset
        frontier.emplace(n);
        frontierSet.emplace(n);
    }

    // do this up to find a visitable border and break the loop
    if (w->catWinsOnSpace(current))
    {
        borderExit = current;
        break;
    }
    frontier.pop();
  }

  Point2D cursor = borderExit;
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

std::vector<Point2D> Agent::getVisitableNeightbors(CatWorld* w, Point2D p)
{
    std::vector<Point2D> neighbors;

    for (auto pos : w->neighbors(p))
    {
        // push back if its a valid move and there is no wall
        if (w->isValidPosition(pos) && !w->getContent(pos))
            neighbors.push_back(pos);
    }

    return neighbors;
}