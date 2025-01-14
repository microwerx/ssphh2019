#include "pch.hpp"
#include <hatchetfish.hpp>
#include <renderer_window.hpp>
#include <ssphhapp.hpp>

RendererWindow::RendererWindow(const std::string& name) : Vf::Window(name) {}

RendererWindow::~RendererWindow() {}

void RendererWindow::OnUpdate(double timeStamp) {
	if (!ssphh_widget_ptr) {
		rendererContext = nullptr;
		return;
	}

	Vf::Window::OnUpdate(timeStamp);

	if (rendererContext != ssphh_widget_ptr->rendererContext) rendererContext = ssphh_widget_ptr->rendererContext;
}

void RendererWindow::OnRenderDearImGui() {
	if (!rendererContext || !beginWindow()) return;
	Vf::Window::OnRenderDearImGui();

	if (ImGui::Button("Reset")) {
		rendererContext->reinit();
		endWindow();
		return;
	}
	ImGui::SameLine();
	if (ImGui::Button("Defaults")) rendererContext->set_default_parameters();

	ImGui::Value("Render Mode", ssphh_widget_ptr->renderMode);
	ImGui::SameLine();
	ImGui::Value("Render Time", (float)ssphh_widget_ptr->my_hud_info.totalRenderTime);
	ImGui::SameLine();
	ImGui::Text("Renderable Pct %s", rendererContext->status());

	if (ImGui::Button("Load Configs"))
		HFCLOCKSf(lastConfigsLoadTime, rendererContext->loadConfig(SSPHH::default_renderconfig_path)) ImGui::SameLine();
	ImGui::SameLine();
	ImGui::Value("configs load", lastConfigsLoadTime);

	if (ImGui::Button("Load Shaders")) { HFCLOCKSf(lastShadersLoadTime, ssphh_widget_ptr->LoadShaders()); }
	ImGui::SameLine();
	ImGui::Value("shaders load", lastShadersLoadTime);

	if (ImGui::Button("Load Textures")) HFCLOCKSf(lastTextureLoadTime, rendererContext->loadTextures());
	ImGui::SameLine();
	ImGui::Value("texture load", lastTextureLoadTime);

	static float lastFramebufferTime{ 0 };
	if (ImGui::Button("Make Framebuffers")) HFCLOCKSf(lastFramebufferTime, rendererContext->makeFramebuffers());
	ImGui::SameLine();
	ImGui::Value("framebuffer time", lastFramebufferTime);

	ImGui::Separator();

	ImGui::TextColored(Colors::Azure, "Passes");
	ImGui::SameLine();
	ImGui::Checkbox("Sky Box", &ssphh_widget_ptr->Interface->drawSkyBox);
	ImGui::SameLine();
	ImGui::Checkbox("PBR", &ssphh_widget_ptr->Interface->drawPBR);
	ImGui::SameLine();
	ImGui::Checkbox("VIZ", &ssphh_widget_ptr->Interface->drawVIZ);
	ImGui::SameLine();
	ImGui::Checkbox("POST", &ssphh_widget_ptr->Interface->drawPOST);

	ImGui::Separator();

	if (ImGui::TreeNode("Renderer Objects")) {
		for (auto& [k, v] : rendererContext->rendererObjects) {
			ImGui::TextColored(
				Colors::ForestGreen,
				"%d | %s | %s (%d) | %s",
				v->instance(),
				v->name(),
				v->type(),
				v->secondaryClassType(),
				v->status());
		}
		ImGui::TreePop();
	}

	ImGui::Separator();

	if (ImGui::TreeNode("programs")) {
		for (auto& [k, ro] : rendererContext->programs) {
			ImGui::Text("%s [%s, %s]", ro->name(), ro->status(), (ro->usable() ? "usable" : "not usable"));
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("renderers")) {
		for (auto& [k, ro] : rendererContext->renderers) {
			ImGui::Text("%s [%s, %s]", ro->name(), ro->status(), (ro->usable() ? "usable" : "not usable"));
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("rendererconfigs")) {
		for (auto& [k, ro] : rendererContext->rendererConfigs) {
			ImGui::Text("%s [%s, %s]", ro->name(), ro->status(), (ro->usable() ? "usable" : "not usable"));
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("fbos")) {
		for (auto& [k, ro] : rendererContext->fbos) {
			ImGui::Text("%s [%s, %s]", ro->name(), ro->status(), (ro->usable() ? "usable" : "not usable"));
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("texture2Ds")) {
		for (auto& [k, ro] : rendererContext->texture2Ds) {
			ImGui::Text("%s [%s, %s]", ro->name(), ro->status(), (ro->usable() ? "usable" : "not usable"));
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("textureCubes")) {
		for (auto& [k, ro] : rendererContext->textureCubes) {
			ImGui::Text("%s [%s, %s]", ro->name(), ro->status(), (ro->usable() ? "usable" : "not usable"));
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("samplers")) {
		for (auto& [k, ro] : rendererContext->samplers) {
			ImGui::Text("%s [%s, %s]", ro->name(), ro->status(), (ro->usable() ? "usable" : "not usable"));
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("vars")) {
		for (auto& [k, v] : rendererContext->vars.variables) {
			if (v.IsInteger()) ImGui::Value(k.c_str(), v.ival);
			if (v.IsDouble()) ImGui::Value(k.c_str(), (float)v.dval);
			if (v.IsStringOrIdentifier()) ImGui::Value(k.c_str(), v.sval.c_str());
		}
		ImGui::TreePop();
	}

	static int hflogCurrentItem{ 0 };
	int hflogSize = HFLOG_HISTORYSIZE();
	if (hflogSize) ImGui::ListBox("hflog", &hflogCurrentItem, Hf::Log.getHistoryItems(), hflogSize);

	endWindow();
}
