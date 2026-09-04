#include "SeparationRule.h"
#include "imgui.h"
#include <glm/glm.hpp>

glm::vec2 SeparationRule::computeForce(const std::vector<BoidView>& neighborhood, const BoidView& boid) {
  glm::vec2 separatingForce(0.f);

  // the header have the desiredMinimalDistance member variable, which is the distance that the boids should try to maintain from each other.
  // glm::length(vec) returns the length of a vector,
  // glm::normalize(vec) returns the normalized vector (length 1) in the same direction as vec.
  // multiply by (desiredMinimalDistance / distance) is the proportionality factor that makes the force stronger when the boids are closer together, and weaker when they are farther apart.

  // begin solution
    float dist = 0;
    glm::vec2 force = glm::vec2(0);
    glm::vec2 offset = glm::vec2(0);

    for (auto b : neighborhood)
    {
        force = glm::vec2(0);
        offset = b.position - boid.position;

        force -= glm::normalize(offset); // dir (subtract to go the other way)
        dist = glm::length(offset);      // dist

        if (dist <= 0.0001f)
            dist = 0.0001f;

        force *= (desiredMinimalDistance / dist);

        separatingForce += force;
    }

  // end solution

  return separatingForce;
}

bool SeparationRule::drawImguiRuleExtra() {
  bool valueHasChanged = false;
  if (ImGui::DragFloat("Desired Separation", &desiredMinimalDistance, 0.05f)) {
    valueHasChanged = true;
  }
  return valueHasChanged;
}
