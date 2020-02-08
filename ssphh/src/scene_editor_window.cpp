#include "pch.hpp"
#include <ssphhapp.hpp>
#include <scene_editor_window.hpp>

SceneEditorWindow::SceneEditorWindow(const std::string& name)
	: Vf::Window(name) {}

SceneEditorWindow::~SceneEditorWindow() {}

void SceneEditorWindow::OnUpdate(double timestamp) {
	if (!ssphh_widget_ptr) {
		ssg = nullptr;
		return;
	}
	else if (ssg != &ssphh_widget_ptr->ssg) {
		ssg = &ssphh_widget_ptr->ssg;
	}
	if (!ssg) return;
	Vf::Window::OnUpdate(timestamp);
}

void SceneEditorWindow::OnRenderDearImGui() {
	// Note: Scene Editor Window is mapped to F4
	if (!ssg) return;
	if (!beginWindow()) return;
	Vf::Window::OnRenderDearImGui();

	Fluxions::BaseEnvironment* e = (Fluxions::BaseEnvironment*) & ssg->environment;

	ImGui::TextColored(Colors::Azure, "Environment");
	Vf::ImGuiValue4f("tonemap ", e->toneMap);
	Vf::ImGuiValue4f("fog     ", e->fogSH);
	Vf::ImGuiValue4f("ground  ", e->ground);
	Vf::ImGuiValue4f("location", e->location);
	Vf::ImGuiValue4f("date    ", e->date);
	Vf::ImGuiValue4f("time    ", e->time);
	ImGui::DragFloat("yyyy", e->date.ptr() + 0, 1.0f, 2010.0f, 2030.0f, "%04f");
	ImGui::DragFloat("mmm", e->date.ptr() + 1, 1.0f, 1.0f, 12.0f, "%03f");
	ImGui::DragFloat("ddd", e->date.ptr() + 2, 1.0f, 1.0f, 30.0f, "%02f");
	ImGui::DragFloat("hh", e->time.ptr() + 0, 1.0f, 0.0f, 23.0f, "%2f");
	ImGui::DragFloat("mm", e->time.ptr() + 1, 1.0f, 0.0f, 59.0f, "%2f");
	ImGui::DragFloat("ss", e->time.ptr() + 2, 1.0f, 0.0f, 59.0f, "%2f");
	ImGui::DragFloat("lat", e->location.ptr() + 0, 1.0f, -90.0f, 90.0f, "% 3.2f");
	ImGui::DragFloat("lon", e->location.ptr() + 1, 1.0f, -180.0f, 180.0f, "% 3.2f");
	ImGui::DragFloat("agl", e->location.ptr() + 2, 1.0f, 0.0f, 100.0f, "% 3.2f");
	ImGui::DragFloat("alt", e->location.ptr() + 3, 1.0f, 0.0f, 100.0f, "% 3.2f");

	if (ssg->dirToLights.count("Sun")) {
		ImGui::PushID("Sun");
		auto L = ssg->dirToLights["Sun"];
		ImGui::TextColored(Colors::Azure, "Sun");
		ImGui::ColorEdit3("sunE0", e->sun.ptr());
		ImGui::DragFloat3("sunDirTo", L.dirTo.ptr());
		if (ImGui::Button("unit")) L.dirTo.normalize();
		ImGui::PopID();
	}

	if (ssg->dirToLights.count("Moon")) {
		ImGui::PushID("Moon");
		auto L = ssg->dirToLights["Moon"];
		ImGui::TextColored(Colors::Azure, "Moon");
		ImGui::ColorEdit3("moonE0", e->moon.ptr());
		ImGui::DragFloat3("moonDirTo", L.dirTo.ptr());
		if (ImGui::Button("unit")) L.dirTo.normalize();
		ImGui::PopID();
	}

	endWindow();
}
