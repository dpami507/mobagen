#include "BoundedAreaRule.h"
#include "imgui.h"
#include <glm/glm.hpp>
#include <algorithm>

glm::vec2 BoundedAreaRule::computeForce(const std::vector<BoidView>& neighborhood, const BoidView& boid) {
  glm::vec2 force(0.f);
  glm::vec2 tempForce(0.f);
  float strength = 0.f;

  ImVec2 displaySize = ImGui::GetIO().DisplaySize;
  // desiredDistance is the distance from the borders that the boids should try to maintain. 

  // begin solution
  if (boid.position.x < desiredDistance)
  {
      // Apply force right
      tempForce = glm::vec2(1.f, 0.f);

      float dist = boid.position.x;
      if (dist <= 0.0001f)
          dist = 0.0001f;

      strength = desiredDistance / dist;
      tempForce *= strength;

      force += tempForce;
  }
  if (boid.position.x > displaySize.x - desiredDistance)
  {
      // Apply force left
      tempForce = glm::vec2(-1.f, 0.f);

      float dist = (displaySize.x - boid.position.x);
      if (dist <= 0.0001f)
          dist = 0.0001f;

      strength = desiredDistance / dist;
      tempForce *= strength;

      force += tempForce;
  }
  if (boid.position.y < desiredDistance)
  {
      // Apply force down
      tempForce = glm::vec2(0.f, 1.f);

      float dist = boid.position.y;
      if (dist <= 0.0001f)
          dist = 0.0001f;

      strength = desiredDistance / dist;
      tempForce *= strength;

      force += tempForce;
  }
  if (boid.position.y > displaySize.y - desiredDistance)
  {
      // Apply force up
      tempForce = glm::vec2(0.f, -1.f);

      float dist = (displaySize.y - boid.position.y);
      if (dist <= 0.0001f)
          dist = 0.0001f;

      strength = desiredDistance / dist;
      tempForce *= strength;

      force += tempForce;
  }

  return force;
}

bool BoundedAreaRule::drawImguiRuleExtra() {
  ImVec2 displaySize = ImGui::GetIO().DisplaySize;
  float widthWindows = displaySize.x > 0.f ? displaySize.x : 1280.f;
  float heightWindows = displaySize.y > 0.f ? displaySize.y : 800.f;
  bool valueHasChanged = false;
  int minHeightWidth = static_cast<int>(std::min(widthWindows, heightWindows));

  if (ImGui::SliderInt("Desired Distance From Borders", &desiredDistance, 0, minHeightWidth / 3, "%i")) {
    valueHasChanged = true;
  }
  return valueHasChanged;
}

void BoundedAreaRule::drawWorldOverlay(ImDrawList* dl) const {
  ImVec2 displaySize = ImGui::GetIO().DisplaySize;
  float w = displaySize.x > 0.f ? displaySize.x : 1280.f;
  float h = displaySize.y > 0.f ? displaySize.y : 800.f;
  float d = static_cast<float>(desiredDistance);
  ImU32 col = IM_COL32(128, 128, 128, 200);

  dl->AddLine({d, d}, {w - d, d}, col);
  dl->AddLine({w - d, d}, {w - d, h - d}, col);
  dl->AddLine({w - d, h - d}, {d, h - d}, col);
  dl->AddLine({d, h - d}, {d, d}, col);
}
