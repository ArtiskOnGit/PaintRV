#include "ImguiWindows.h"

void ImguiWindows::init_ui()
{
    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    io = ImGui::GetIO();

    ImGui::NewFrame();
    ImGui::ShowDemoWindow(); // Show demo window! :)

    if (showNewFileWindow) { NewFileWindow(); }


    ImGui::Begin("Tools!");                          // Create a window called "Hello, world!" and append into it.
    ImGui::Text("This is the tools windows.");
    //tool selector
    ImGui::RadioButton("No tool", &canva->tool, 0); ImGui::SameLine();
    ImGui::RadioButton("Brush", &canva->tool, 1); ImGui::SameLine();
    ImGui::RadioButton("Eraser", &canva->tool, 2); ImGui::SameLine();
    ImGui::RadioButton("Fill", &canva->tool, 3);
    ImGui::RadioButton("Circle", &canva->tool, 4);
    ImGui::SliderInt("Taille", &(canva->size), 1, 100);
    //ImGui::SliderFloat("Taille", &size, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("Couleur pinceau", (float*)&(canva->couleur_pinceau)); // Edit 3 floats representing a color



    ImGui::End();


    //test de menu
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {

            ImGui::MenuItem("(file menu)", NULL, false, false);
            if (ImGui::MenuItem("New")) {showNewFileWindow = true; printf("New file\n"); }
            if (ImGui::MenuItem("Open", "Ctrl+O")) {}
            if (ImGui::BeginMenu("Open Recent"))
            {
                ImGui::MenuItem("fish_hat.jpg");
                ImGui::MenuItem("fish.png");
                ImGui::MenuItem("camarchepas.jpg");
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) { printf("Save\n"); }
            if (ImGui::MenuItem("Save As..")) { printf("Save as\n"); }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void ImguiWindows::NewFileWindow()
{
    
    showNewFileWindow = true;
	bool window_contents_visible = ImGui::Begin("Example: Documents", &showNewFileWindow);
    if (!window_contents_visible)
    {
        printf("Ici\n");
        ImGui::End();
        return;
    }
    ImGui::Text("Entrez la taille du canva desiree : ");
    ImGui::DragInt("X", &(new_canva_width), 1.f);
    ImGui::DragInt("Y", &(new_canva_height), 1.f);
    if (ImGui::Button("Valider")) {
        canva->new_blank_canva(new_canva_width, new_canva_height);
        showNewFileWindow = false;
    }


    ImGui::End();
}
