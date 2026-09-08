#include "CohesionRule.h"
#include <glm/glm.hpp>

glm::vec2 CohesionRule::computeForce(const std::vector<BoidView>& neighborhood, const BoidView& boid) {
  glm::vec2 cohesionForce(0.f);

  // glm::length(vec) returns the length of a vector,
  // glm::normalize(vec) returns the normalized vector (length 1) in the same direction as vec.

  // begin solution
    glm::vec2 avgPosition = glm::vec2(0);

    if (neighborhood.empty())
        return cohesionForce;

    for (auto b : neighborhood)
    {
        avgPosition += b.position;
    }

    // avg posiion
    avgPosition /= neighborhood.size();

    // get offset
    glm::vec2 offset = avgPosition - boid.position;

    // saftey to make sure distance is a meaningful amount
    float dist = glm::length(offset);
    if (dist <= 0.001f)
        return cohesionForce;

    // normalize to get the direction
    cohesionForce = glm::normalize(offset);

  // end solution

  return cohesionForce;
}
