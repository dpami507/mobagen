#include "AlignmentRule.h"
#include <glm/glm.hpp>

glm::vec2 AlignmentRule::computeForce(const std::vector<BoidView>& neighborhood, const BoidView& boid) {
  glm::vec2 averageVelocity(0.f);
  // glm::vec2 can be divided by a float, which will divide each component of the vector by that float.

  // begin solution
  int count = 0;
  for (auto b : neighborhood) {
    count++;
    averageVelocity += b.velocity;
  }
  if(count > 0)
    averageVelocity /= count;

  return averageVelocity;
  // end solution
}
