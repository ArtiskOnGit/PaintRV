#include "ImguiWindows.h"

void ImguiWindows::init_ui()
{
    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    io = ImGui::GetIO();

    ImGui::NewFrame();
    ImGui::ShowDemoWindow(); // Show demo window! :)

    //fenetres auxillaires
    if (showNewFileWindow) { NewFileWindow(); }
    if (showSaveAsWindow) { SaveAsWindow(); }
    if (showOpenFileWindow) { OpenFileWindow(); }

    //shortcuts
    //if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_O && !showOpenFileWindow)) { printf("Open File shortcut"); showOpenFileWindow = true; }


    ImGui::Begin("Tools!");                          // Create a window called "Hello, world!" and append into it.
    ImGui::Text("This is the tools windows.");
    //tool selector
    ImGui::RadioButton("Pas d'outil", &canva->tool, -1);
    ImGui::RadioButton("Brosse carrée", &canva->tool, 0); ImGui::SameLine();
    ImGui::RadioButton("Brosse circulaire", &canva->tool, 1); ImGui::SameLine();
    ImGui::Checkbox("Gomme", &canva->gomme);
    //ImGui::RadioButton("Gomme", &canva->tool, 2); 
    ImGui::RadioButton("Remplir", &canva->tool, 3);
    ImGui::RadioButton("Pipette", &canva->tool, 4);
    ImGui::RadioButton("Cercle", &canva->tool, 5);
    ImGui::SliderInt("Taille", &(canva->size), 1, 200);
    //ImGui::SliderFloat("Taille", &size, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("Couleur pinceau", (float*)&(canva->couleur_pinceau)); // Edit 3 floats representing a color

    ImGui::Separator();
    ImGui::Text("Calques :");
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
    if (ImGui::BeginChild("ResizableChild", ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 8), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY))
        for (int n = 0; n < 10; n++)
            ImGui::Text("Calque %04d", n);
    ImGui::PopStyleColor();
    ImGui::EndChild();

    ImGui::End();


    //test de menu
    if (ImGui::BeginMainMenuBar()) {
        
        if (ImGui::BeginMenu("File")) {

            ImGui::MenuItem("(file menu)", NULL, false, false);
            if (ImGui::MenuItem("New")) {showNewFileWindow = true; printf("New file\n"); }
            if (ImGui::MenuItem("Open", "Ctrl+O")) { showOpenFileWindow = true; printf("Open file\n");  }
            if (ImGui::BeginMenu("Open Recent"))
            {
                if (ImGui::MenuItem("fish_hat.jpg")) { canva->load_image("container.jpg"); }
                ImGui::MenuItem("fish.png");
                ImGui::MenuItem("camarchepas.jpg");
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) { printf("Save\n"); }
            if (ImGui::MenuItem("Save As..")) { showSaveAsWindow = true; printf("Save as\n"); }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void ImguiWindows::NewFileWindow()
{
    
    
	bool window_contents_visible = ImGui::Begin("New file", &showNewFileWindow);
    if (!window_contents_visible)
    {
        printf("Ici\n");
        ImGui::End();
        return;
    }
    ImGui::Text("Entrez la taille du canva desiree : ");
    ImGui::DragInt("X", &(new_canva_width), 1.f);
    ImGui::DragInt("Y", &(new_canva_height), 1.f);
    ImGui::Checkbox("Channel Apha ?", &(new_canva_has_alpha));
    if (ImGui::Button("Valider")) {
        canva->new_blank_canva(new_canva_width, new_canva_height, new_canva_has_alpha);
        showNewFileWindow = false;
    }


    ImGui::End();
}


void ImguiWindows::SaveAsWindow()
{

    
    bool window_contents_visible = ImGui::Begin("Example: Documents", &showSaveAsWindow);
    if (!window_contents_visible)
    {
        printf("Ici\n");
        ImGui::End();
        return;
    }
    ImGui::Text("Sauvegarde le fichier vers: ");
    static char path[512] = "C:\\\\Users\\\Alexandre\\Documents\\cours\\tries\\P1RV\\PaintRV\\test glfw\\saving_test\\test";
    ImGui::InputText("Chemin de sauvegarde du fichier ", path, IM_ARRAYSIZE(path));
    static int format = 0;
    ImGui::Text("format du fichier: ");
    ImGui::RadioButton("png", &format, 0); ImGui::SameLine();
    ImGui::RadioButton("jpg", &format, 1);

    if (ImGui::Button("Valider")) {
        if (canva->save_image(path, format)) {
            showSaveAsWindow = false;
        }
        else { ImGui::Text("Erreur dans la sauvegarde du fichier "); }
    }


    ImGui::End();
}

void ImguiWindows::OpenFileWindow()
{


    bool window_contents_visible = ImGui::Begin("Example: Documents", &showOpenFileWindow);
    if (!window_contents_visible)
    {
        printf("Ici\n");
        ImGui::End();
        return;
    }
    ImGui::Text("Ouvrir un fichier : ");
    static char path[512] = "C:\\\\Users\\\Alexandre\\Documents\\cours\\tries\\P1RV\\PaintRV\\test glfw\\saving_test\\test";
    ImGui::InputText("Chemin du fichier ", path, IM_ARRAYSIZE(path));
   

    if (ImGui::Button("Valider")) {
        if (canva->load_image(path)) {
            showOpenFileWindow = false;
        }
        else { ImGui::Text("Erreur dans l'ouverture du fichier "); }
    }


    ImGui::End();
}
