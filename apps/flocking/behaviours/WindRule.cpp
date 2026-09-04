#include "WindRule.h"
#include "imgui.h"
#include <cmath>
#include <iostream>

glm::vec2 WindRule::computeForce(const std::vector<BoidView>& neighborhood, const BoidView& boid) {
  // std::cos and std::sin return the cosine and sine of an angle in radians, respectively.
  // windAngle is the angle of the wind direction in degrees, so we need to convert it to radians by multiplying it by (pi / 180).
  
  // begin solution
  glm::vec2 dir = glm::normalize(glm::vec2(cos(windAngle), sin(windAngle)));

  return dir;
  // end solution
}

bool WindRule::drawImguiRuleExtra() {
  bool valueHasChanged = false;
  if (ImGui::SliderAngle("Wind Direction", &windAngle, 0)) {
    valueHasChanged = true;
  }
  return valueHasChanged;
}
