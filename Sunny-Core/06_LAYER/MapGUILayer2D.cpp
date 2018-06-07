#include "MapGUILayer2D.h"

#include "../06_LAYER/MapGUILayer3D.h"

MapGUILayer2D::MapGUILayer2D() 
: Layer2D(maths::mat4::Orthographic(0.0f, Application::GetApplication().GetWindowWidth(), 0.0f, Application::GetApplication().GetWindowHeight(), -1.0f, 1.0f))
{

}

MapGUILayer2D::~MapGUILayer2D()
{

}

void MapGUILayer2D::OnInit(Renderer2D& renderer)
{
	Application::GetApplication().PushLayer3D(new MapGUILayer3D(*this));
	ImGui::GetStyle().WindowRounding = 3.0f;
}

void MapGUILayer2D::OnTick()
{

}

void MapGUILayer2D::OnUpdate(const utils::Timestep& ts)
{

}

void MapGUILayer2D::OnRender(Renderer2D& renderer)
{
	ImGui_ImplDX11_NewFrame();

	static bool show_demo_window = true;
	static bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui::SetWindowSize(ImVec2((float)Window::GetWindowClass()->GetWidth(), (float)Window::GetWindowClass()->GetHeight()));

	{
		static float f = 0.0f;
		static int counter = 0;
		ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
	if (show_demo_window)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
		ImGui::ShowDemoWindow(&show_demo_window);
	}
	float str0;


	/* Map Editor */
	ImGui::Begin("Map Editor", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	{
		ImGui::SetWindowSize(ImVec2(300, 350));

		if (ImGui::CollapsingHeader("Transform Component", &open_transform, ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Position");
			ImGui::SameLine(80);
			ImGui::PushItemWidth(-1);
			ImGui::InputFloat3("#model_position", &model_position[0], 2);
			ImGui::PopItemWidth();

			ImGui::Text("Rotation");
			ImGui::SameLine(80);
			ImGui::PushItemWidth(-1);
			ImGui::InputFloat3("#model_rotation", &model_rotation[0], 2);
			ImGui::PopItemWidth();

			ImGui::Text("Scale");
			ImGui::SameLine(80);
			ImGui::PushItemWidth(-1);
			ImGui::InputFloat3("#model_scale", &model_scale[0], 2);
			ImGui::PopItemWidth();
		}

		if (ImGui::CollapsingHeader("Camera Component", &open_transform, ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Position");
			ImGui::SameLine(80);
			ImGui::PushItemWidth(-1);
			ImGui::InputFloat3("#camera_position", &camera_position[0], 2);
			ImGui::PopItemWidth();

			ImGui::Text("FocalPoint");
			ImGui::SameLine(80);
			ImGui::PushItemWidth(-1);
			ImGui::InputFloat3("#camera_focalPoint", &camera_focalPoint[0], 2);
			ImGui::PopItemWidth();

			ImGui::Text("Scale");
			ImGui::SameLine(80);
			ImGui::PushItemWidth(-1);
			ImGui::InputFloat3("#model_scale", &model_scale[0], 2);
			ImGui::PopItemWidth();
		}
	}
	ImGui::End();

	/* Asset */
	ImGui::Begin("Asset", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	{
		ImGui::SetWindowPos(ImVec2(5, 700));
		ImGui::SetWindowSize(ImVec2(900, 195));
	}
	ImGui::End();


	// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}