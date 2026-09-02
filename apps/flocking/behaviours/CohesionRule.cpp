#include "CohesionRule.h"
#include <glm/glm.hpp>

glm::vec2 CohesionRule::computeForce(const std::vector<BoidView>& neighborhood, const BoidView& boid) {
  glm::vec2 cohesionForce(0.f);

  // glm::length(vec) returns the length of a vector,
  // glm::normalize(vec) returns the normalized vector (length 1) in the same direction as vec.

  // begin solution
  glm::vec2 avgPosition = glm::vec2(0);
  int count = 0;
  for (auto b : neighborhood)
  {
    avgPosition += b.position;
    count++;
  }
  if (count > 0)
  {
    // avg posiion
    avgPosition /= count;

    // get direction to the position
    glm::vec2 dir = glm::normalize(avgPosition - boid.position);

    // calc force
    float dist = glm::length(avgPosition - boid.position);
    cohesionForce = dir * dist * this->weight; // Weight multiplication because its weak
  }

  // end solution

  return cohesionForce;
}
