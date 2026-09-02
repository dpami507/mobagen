#include "MouseInfluenceRule.h"
#include "imgui.h"

glm::vec2 MouseInfluenceRule::computeForce(const std::vector<BoidView>& neighborhood, const BoidView& boid) {
  glm::vec2 force(0.f);

  // ImGui::IsMouseDown(ImGuiMouseButton_Left) returns true if the left mouse button is currently pressed.
  // ImGui::GetIO().MousePos returns the current mouse position as an ImVec2.
  // glm::length(vec) returns the length of a vector

  // begin solution
  if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
  {
      ImVec2 imMousePos = ImGui::GetIO().MousePos;
      glm::vec2 mousePos = glm::vec2(imMousePos.x, imMousePos.y);

      float dist = 0;
      glm::vec2 newForce = glm::vec2(0);
      glm::vec2 offset = glm::vec2(0);

      newForce = glm::vec2(0);
      offset = boid.position - mousePos;

      if (isRepulsive)
          newForce += glm::normalize(offset); // dir (subtract to go the other way)
      else
          newForce -= glm::normalize(offset); // dir (subtract to go the other way)

      dist = glm::length(offset);             // dist
      newForce *= (1.f / dist) * this->weight; // Weight multiplication because its weak

      force += newForce;
  }
  // end solution

  return force;
}

bool MouseInfluenceRule::drawImguiRuleExtra() {
  bool valueHasChanged = false;

  if (ImGui::RadioButton("Attractive", !isRepulsive)) {
    isRepulsive = false;
    valueHasChanged = true;
  }
  ImGui::SameLine();
  if (ImGui::RadioButton("Repulsive", isRepulsive)) {
    isRepulsive = true;
    valueHasChanged = true;
  }

  return valueHasChanged;
}
