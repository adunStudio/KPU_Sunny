#include "MapGUILayer2D.h"
#include "MapGUILayer3D.h"
#include "../06_LAYER/MapGUILayer3D.h"

template <typename T> string tostr(const T& t) {
	ostringstream os;
	os << t;
	return os.str();
}

MapGUILayer2D::MapGUILayer2D(MapGUILayer3D* layer)
: Layer2D(maths::mat4::Orthographic(0.0f, Application::GetApplication().GetWindowWidth(), 0.0f, Application::GetApplication().GetWindowHeight(), -1.0f, 1.0f))
, m_layer3D(layer)
{

}

MapGUILayer2D::~MapGUILayer2D()
{

}

void MapGUILayer2D::OnInit(Renderer2D& renderer)
{
	ImGui::GetStyle().WindowRounding = 3.0f;
}

void MapGUILayer2D::OnTick()
{

}

void MapGUILayer2D::OnUpdate(const utils::Timestep& ts)
{
	if (m_pickedModel)
	{
		auto component = m_pickedModel->GetTransformComponent();
		component->SetPosition(model_position);
		component->SetRotation(model_rotation);
		component->SetScale(model_scale);
	}
}

void MapGUILayer2D::OnRender(Renderer2D& renderer)
{
	ImGui_ImplDX11_NewFrame();

	static bool show_demo_window = true;
	static bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui::SetWindowSize(ImVec2((float)Window::GetWindowClass()->GetWidth(), (float)Window::GetWindowClass()->GetHeight()));
	/*
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
	}*/

	// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
	/*
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
	*/

	// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
	/*if (show_demo_window)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
		ImGui::ShowDemoWindow(&show_demo_window);
	}
	*/
	ImGui::ShowDemoWindow(&show_demo_window);
	/* Setting */
	ImGui::Begin("Option", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	{
		ImGui::SetWindowPos(ImVec2(5, 5));
		ImGui::SetWindowSize(ImVec2(175, 65));

		if (ImGui::Button("Save"))
		{
			Save();
		}
		ImGui::SameLine();

		char* label = directx::Context::s_solidFrame ? "Solid" : "WireFrame";

		if (ImGui::Button(label))
		{
			directx::Context::s_solidFrame = !directx::Context::s_solidFrame;
			directx::Context::GetContext()->SetSolid();
		}
	}
	ImGui::End();

	/* Object */
	ImGui::Begin("Objects", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	{
		ImGui::SetWindowPos(ImVec2(5, 75));
		ImGui::SetWindowSize(ImVec2(240, 620));

		vector<Model3D*>& mapObjects = m_layer3D->m_mapObjects;

		int number = 1;
		string name = "";
		string del_name = "";

		bool push = false;
		for (auto object : mapObjects)
		{
			name = tostr(number) + "." + object->name;
			del_name = "X       " + tostr(number); //
			
			if (object->picked) push = true;
			else push = false;

			if(push)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(150, 140, 61, 255)));
				ImGui::SetScrollHere();
			}

			if (ImGui::Button(name.c_str()))
			{
				if (m_pickedModel)
					m_pickedModel->picked = false;

				m_layer3D->m_pickedModel = object;
				SetPickedModel(object);

				object->picked = true;

			}
			ImGui::SameLine(200);
			
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(153, 61, 61, 255)));
			if (ImGui::Button(del_name.c_str()))
			{
				if (m_pickedModel == object)
				{
					m_pickedModel = nullptr;
					m_layer3D->m_pickedModel = nullptr;
				}
				
				m_layer3D->Remove(object);

				delete object;

				mapObjects.erase(std::remove(mapObjects.begin(), mapObjects.end(), object), mapObjects.end());
			}
			ImGui::PopStyleColor();

			if(push)
				ImGui::PopStyleColor();

			number++;
		}
	}
	ImGui::End();

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

		if (ImGui::CollapsingHeader("Terrain Component", &open_transform, ImGuiTreeNodeFlags_DefaultOpen))
		{
			
	
		}

		if (ImGui::CollapsingHeader("Camera Component", &open_transform, ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Position");
			ImGui::SameLine(80);
			ImGui::PushItemWidth(-1);
			ImGui::InputFloat3("#camera_position",   &camera_position[0], 2);
			ImGui::PopItemWidth();

			ImGui::Text("FocalPoint");
			ImGui::SameLine(80);
			ImGui::PushItemWidth(-1);
			ImGui::InputFloat3("#camera_focalPoint", &camera_focalPoint[0], 2);
			ImGui::PopItemWidth();
		}
	}
	ImGui::End();

	/* Asset */
	/*ImGui::Begin("Asset", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	{
		ImGui::SetWindowPos(ImVec2(5, 700));
		ImGui::SetWindowSize(ImVec2(900, 195));
	}
	ImGui::End();*/


	// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void MapGUILayer2D::SetPickedModel(Model3D* model)
{
	m_pickedModel = nullptr;
	

	auto component = model->GetTransformComponent();

	model_position = vec3(component->GetPosition());
	model_rotation = vec3(component->GetRotation());
	model_scale    = vec3(component->GetScale());

	m_first = true;
	
	m_pickedModel = model;
}

void MapGUILayer2D::Save()
{
	Json::Value root(Json::arrayValue);

	//root.append

	string name;
	TransformComponent* transformComponent = nullptr;
	vec3 position, rotation, scale;

	for (auto& object : m_layer3D->m_mapObjects)
	{
		Json::Value val(Json::objectValue);

		name = object->name;
		transformComponent = object->GetTransformComponent();
		position = transformComponent->GetPosition();
		rotation = transformComponent->GetRotation();
		scale = transformComponent->GetScale();

		val["name"] = name;
		
		val["position"]["x"] = position.x;
		val["position"]["y"] = position.y;
		val["position"]["z"] = position.z;
		
		val["rotation"]["x"] = rotation.x;
		val["rotation"]["y"] = rotation.y;
		val["rotation"]["z"] = rotation.z;
		
		val["scale"]["x"] = scale.x;
		val["scale"]["y"] = scale.y;
		val["scale"]["z"] = scale.z;

		root.append(val);
	}

	
	//system::FileSystem::ReadTextFile("/JSON/MAP/map1.json");
	system::FileSystem::WriteTextFile("03_JSON/MAP/map3.json", root.toStyledString(), true);
}