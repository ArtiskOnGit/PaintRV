#include "ImguiWindows.h"


bool ColorPicker4U32(const char* label, ImU32* color, ImGuiColorEditFlags flags = 0) {
    float col[4];
    col[0] = (float)((*color >> 0) & 0xFF) / 255.0f;
    col[1] = (float)((*color >> 8) & 0xFF) / 255.0f;
    col[2] = (float)((*color >> 16) & 0xFF) / 255.0f;
    col[3] = (float)((*color >> 24) & 0xFF) / 255.0f;

    bool result = ImGui::ColorPicker4(label, col, flags);

    *color = ((ImU32)(col[0] * 255.0f)) |
        ((ImU32)(col[1] * 255.0f) << 8) |
        ((ImU32)(col[2] * 255.0f) << 16)|
        ((ImU32)(col[3] * 255.0f) << 24);

    return result;
}



void ImguiWindows::show_ui()
{
    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    io = ImGui::GetIO();

    ImGui::NewFrame();
    //ImGui::ShowDemoWindow(); // Show demo window! :)

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
    ImGui::Checkbox("Gomme", &canva->eraser);
    //ImGui::RadioButton("Gomme", &canva->tool, 2); 
    ImGui::RadioButton("Remplir", &canva->tool, 3);
    ImGui::RadioButton("Pipette", &canva->tool, 4);
    ImGui::RadioButton("Cercle", &canva->tool, 5);
    ImGui::SliderInt("Taille", &(canva->size), 1, 200);
    //ImGui::SliderFloat("Taille", &size, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ColorPicker4U32("Couleur pinceau", &canva->u32_color_brush); // Edit 3 floats representing a color

    ImGui::Separator();
    ImGui::Text("Calques :"); ImGui::SameLine();
    if (ImGui::Button("Nouveau calque")) {
        canva->new_layer();
    }
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
    if (ImGui::BeginChild("ResizableChild", ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 8), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY))
        for (int n = 0; n < canva->nr_layer; n++)
        {
            
            //ImGui::Text("Calque %04d", n); ImGui::SameLine();
            ImTextureID my_tex_id = canva->calques[n]->texture;
            
            float height_text = ImGui::GetTextLineHeightWithSpacing() * 2;
            float width_text = height_text * canva->calques[n]->width/canva->calques[n]->height;
            ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
            ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right

            ImGui::PushID(n);
            ImGui::Image(my_tex_id, ImVec2(width_text, height_text), uv_min, uv_max); ImGui::SameLine();
            ImGui::RadioButton("##radioButton", &canva->selected_layer, n); ImGui::SameLine();
            ImGui::InputText("##nom", (canva->calques[n]->nom), 128); ImGui::SameLine();
            ImGui::Checkbox("Montrer", &canva->calques[n]->activated);
            ImGui::PopID();
        }
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
        //printf("Ici\n");
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
        //printf("Ici\n");
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
        //printf("Ici\n");
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
