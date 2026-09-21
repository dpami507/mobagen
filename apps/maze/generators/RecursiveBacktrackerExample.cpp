#include "../World.h"
#include "../SeededRandom.h"
#include "RecursiveBacktrackerExample.h"
#include <climits>

// Recursive backtracker, in FORMAL units: (0, 0) is the top-left cell, x grows
// right, y grows down. The caller seeds SeededRandom before the first Step;
// every decision consumes the seed in order, so the maze is deterministic.
//
// Procedure per Step, on the cell at the top of the path stack:
//   1. mark it visited;
//   2. list its visitable (unvisited) neighbors in clockwise order starting
//      from the top: UP, RIGHT, DOWN, LEFT (getVisitables does this);
//   3. none        -> dead end: pop the stack (backtrack). Empty stack = done;
//   4. exactly one -> move to it, do not consume a random number;
//   5. two or more -> consume SeededRandom::next() and pick
//      next() % visitableCount;
//   6. moving opens the wall between the two cells
//      (World::SetNorth/SetEast/SetSouth/SetWest with false).

void RecursiveBacktrackerExample::Clear(World* world) {
	// todo: reset the walk
	// hint:
	//   clear visited and the path stack, then start the walk at the
	//   top-left cell in formal units: stack.push_back({0, 0})
	// begin solution

    visited.clear();

    for (int x = 0; x < world->GetWidth(); x++)
    {
        for (int y = 0; y < world->GetHeight(); y++)
        {
            visited[x].insert(std::make_pair(y, false));
        }
    }

    stack.push_back({0, 0});

    SeededRandom::setIndex(0);

	// end solution
}

bool RecursiveBacktrackerExample::Step(World* w) {
  // todo: implement one iteration of the recursive backtracker
  // hint:
  //   empty stack  -> the maze is done, return false
  //   otherwise, on the cell at the top of the stack (formal units):
  //   1. mark it visited;
  //   2. list its visitable neighbors with getVisitables
  //      (already in clockwise order: UP, RIGHT, DOWN, LEFT);
  //   3. none        -> dead end: pop the stack (backtrack);
  //   4. exactly one -> move to it, do not consume a random number;
  //   5. two or more -> consume SeededRandom::next() and pick
  //      next() % visitables.size();
  //   moving = opening the wall between the two cells, through the
  //   World coordinate translation:
  //     Point2D worldCurrent = w->ToWorldCoords(current);
  //     UP    -> w->SetNorth(worldCurrent, false)
  //     RIGHT -> w->SetEast(worldCurrent, false)
  //     DOWN  -> w->SetSouth(worldCurrent, false)
  //     LEFT  -> w->SetWest(worldCurrent, false)
  //   return true while there is still work (stack not empty after the move)
  // begin solution

	if (stack.empty())
      return false;

	// 1. mark it visited;
	Point2D point = stack.back();
    visited[point.x][point.y] = true;

	// 2. list its visitable neighbors with getVisitables
	auto visitable = getVisitables(w, point);
    Point2D nextPoint;
    Color color;
    if (visitable.size() <= 0) // 3. none -> dead end: pop the stack (backtrack);
    {
        color = Color(0, 0, 0);
        stack.pop_back();
    }
	else if (visitable.size() == 1) // 4. exactly one -> move to it, do not consume a random number;
	{
        color = Color(1, 0, 0);
        nextPoint = visitable[0];
        stack.push_back(nextPoint);
	}
	else // 5. two or more -> consume SeededRandom::next() and pick
	{
        color = Color(0.5, 0.5, 0.5);
        int index = SeededRandom::next() % visitable.size();
        nextPoint = visitable[index];
        stack.push_back(nextPoint);
	}
    w->SetNodeColor(w->ToWorldCoords(point), color);

    // If we moved set the wall we moved through to false
    if (visitable.size() > 0)
    {
        auto dir = std::make_pair(nextPoint.x - point.x, nextPoint.y - point.y);
        if (dir.first == 0 && dir.second == -1) // Up
        {
            w->SetNorth(w->ToWorldCoords(point), false);
        }
        else if (dir.first == 0 && dir.second == 1) // Down
        {
            w->SetSouth(w->ToWorldCoords(point), false);
        }
        else if (dir.first == -1 && dir.second == 0) // Left
        {
            w->SetWest(w->ToWorldCoords(point), false);
        }
        else if (dir.first == 1 && dir.second == 0) // Right
        {
            w->SetEast(w->ToWorldCoords(point), false);
        }
    }

	return true;
}

std::vector<Point2D> RecursiveBacktrackerExample::getVisitables(World* w, const Point2D& formalPoint) {
	// todo: list the unvisited neighbors of formalPoint, in clockwise order
	// hint:
	//   candidates in order: UP {x, y-1}, RIGHT {x+1, y}, DOWN {x, y+1}, LEFT {x-1, y}
	//   keep a candidate only if it is inside the grid
	//   (0 <= x < w->GetWidth(), 0 <= y < w->GetHeight()) and not visited
	// begin solution

	std::vector<Point2D> returnVisit;
    if (formalPoint.y - 1 >= 0)
    {
        Point2D newPoint(formalPoint.x, formalPoint.y - 1);

        if (!visited[formalPoint.x][formalPoint.y - 1])
        {
            returnVisit.push_back(newPoint);
        }
    }
	if (formalPoint.x + 1 < w->GetWidth())
	{
        Point2D newPoint(formalPoint.x + 1, formalPoint.y);

        if (!visited[formalPoint.x + 1][formalPoint.y])
        {
            returnVisit.push_back(newPoint);
        }
	}
    if (formalPoint.y + 1 < w->GetHeight())
    {
        Point2D newPoint(formalPoint.x, formalPoint.y + 1);

        if (!visited[formalPoint.x][formalPoint.y + 1])
        {
            returnVisit.push_back(newPoint);
        }
    }
    if (formalPoint.x - 1 >= 0)
    {
        Point2D newPoint(formalPoint.x - 1, formalPoint.y);

        if (!visited[formalPoint.x - 1][formalPoint.y])
        {
            returnVisit.push_back(newPoint);
        }
    }

	// end solution
	return returnVisit;
}
