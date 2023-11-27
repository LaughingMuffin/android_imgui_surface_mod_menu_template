//
// Created by Laughing Muffin on 22.09.2023
//
//======================================================================================================================
#pragma once
//======================================================================================================================
#include <Globals.h>
#include <DrawUtils.h>
//======================================================================================================================
using json = nlohmann::json;
//======================================================================================================================
const char *style_list[11] = {
        OBFUSCATE("Classic"),
        OBFUSCATE("Light"),
        OBFUSCATE("Dark Blue"),
        OBFUSCATE("Green & Blue"),
        OBFUSCATE("Dark Red"),
        OBFUSCATE("Deep Dark"),
        OBFUSCATE("Golden Black"),
        OBFUSCATE("Dark Grey"),
        OBFUSCATE("Grey"),
        OBFUSCATE("Soft Dark Red"),
        OBFUSCATE("Steam Half Life")
};
//======================================================================================================================
struct Variables {
    char StylePath[128] = "";
} Vars;
//======================================================================================================================
struct StyleVariables {
    std::string name = OBFUSCATE("style.json");
    int style_selection = 0;
} StyleVars;
// if you extend the number of vars remember to declare them here too
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(StyleVariables, name, style_selection)

//======================================================================================================================
StyleVariables LoadStyles() {

    // try open file
    FILE *file = fopen(Vars.StylePath, OBFUSCATE("r"));
    if (!file) {
        // return default if not there
        return StyleVars;
    }

    json j;

    // try read file
    try {
        std::ifstream f(Vars.StylePath, std::ifstream::binary);
        f >> j;
        return j.get<StyleVariables>();
    } catch (...) {
        // return default if errors
        return StyleVars;
    }
}

//======================================================================================================================
void SaveStyle(StyleVariables preset) {

    //save or overwrite file
    json j = preset;
    std::ofstream o(Vars.StylePath);
    o << std::setw(4) << j << std::endl;
}

//======================================================================================================================
struct MenuVariables {
    float winWidth = 0.0f;
    float winHeight = 0.0f;
    float winPosWidth = 0.0f;
    float winPosHeight = 0.0f;
    ImVec4 textColor = ImVec4(0.990f, 0.0396f, 0.816f,
                              1.00f); // R G B + GAMMA | Using floats, check readme for picker
    ImVec4 color_red = ImVec4(1.0f, 0.0f, 0.0f, 1.00f); // RED - rgb01(1, 0, 0) | #ff0000
    ImVec4 color_purple = ImVec4(1.0f, 0.0f, 1.0f, 1.00f); // PURPLE - rgb01(1, 0, 1) | #ff00ff
    ImVec4 color_green = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // GREEN - rgb01(0, 1, 0) | #00ff00
} MenuVars;
//======================================================================================================================
struct CheatVariables {
    float damageMultiplierF = 1.0f;
    int damageMultiplier = 1;
    int int_radio_A = 0;
    int int_button_math = 0;
    int drag_A = 0;
    int drag_B = 0;
    bool bool_checkbox_A = false;
    bool bool_draw_line = false;
    bool bool_draw_box = false;
} CheatVars;

//======================================================================================================================
void HelpMarker(const char *desc) {
    ImGui::TextDisabled(OBFUSCATE("(?)"));
    if (ImGui::BeginItemTooltip()) {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 30.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

//======================================================================================================================
void DrawLeftColumn() {
    // set current column width
    ImGui::SetColumnWidth(-1, 400);

    // very shitty separator, it applies to the entire window, be it horizontal or vertical, see better example below
    // you will notice the long line on top of the window, that's this
    ImGui::Separator();

    // add your stuff here

    // this is a text separator used with an "empty" string, use ## and write some crap there :)
    // it will stay bound to column
    ImGui::SeparatorText(OBFUSCATE("##LEFT_COLUMN_TOP_SEPARATOR_1"));

    // this prints a simple text, not suggested for long phrases, see nicer example below
    ImGui::Text(OBFUSCATE("This is a normal text!"));
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Text(OBFUSCATE("This is a normal text but it's a little ..."));

    // this separator will show a text in the middle of it
    ImGui::SeparatorText(OBFUSCATE("Separator"));

    // this is a formatted text, it will go to next line wen reaching "EOL"
    ImGui::TextWrapped(OBFUSCATE(
                               "Longer text that will go to next line when reaching the end of the column!"));

    ImGui::SeparatorText(OBFUSCATE("##LEFT_COLUMN_TOP_SEPARATOR_2"));

    // colored text, have fun making it rainbow
    ImGui::TextColored(MenuVars.textColor, OBFUSCATE("I'm colored!"));

    ImGui::SeparatorText(OBFUSCATE("##LEFT_COLUMN_TOP_SEPARATOR_3"));
    ImGui::Spacing();
    ImGui::BulletText(OBFUSCATE("I'm a bullet text!"));
    ImGui::Spacing();
    ImGui::SeparatorText(OBFUSCATE("##LEFT_COLUMN_TOP_SEPARATOR_4"));

    // disabled looking text
    ImGui::TextDisabled(OBFUSCATE("Disabled text!"));

    // put next element on the same line of the previous element
    ImGui::SameLine();

    // I'm something like a tool tip
    HelpMarker(OBFUSCATE("Short Description!"));

    ImGui::SeparatorText(OBFUSCATE("##LEFT_COLUMN_TOP_SEPARATOR_5"));
}

//======================================================================================================================
void DrawCheats() {

    // gives some space btw previous element and next one
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Text(OBFUSCATE("Cheats Tab!"));
    ImGui::SeparatorText(OBFUSCATE("##SEP_MID_1"));

    // make your list of items
    const char *items_combo[] = {"AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH",
                                 "IIIIIII", "JJJJ", "KKKKKKK"};
    static int item_combo_current = 0;
    ImGui::Text("Combo: ");
    ImGui::SameLine();
    ImGui::Text("%s", items_combo[item_combo_current]);

    // pass the list, size and current selection pointer reference
    ImGui::Combo("##_my_combo", &item_combo_current, items_combo, IM_ARRAYSIZE(items_combo));
    ImGui::SeparatorText("##SEP_MID_1");

    // same concept here just different widget :)
    const char *items_list[] = {"Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple",
                                "Strawberry", "Watermelon"};
    static int item_list_current = 1;
    ImGui::Text("List: ");
    ImGui::SameLine();
    ImGui::Text("%s", items_list[item_list_current]);
    ImGui::ListBox("##_my_list", &item_list_current, items_list, IM_ARRAYSIZE(items_list), 5);
    ImGui::SeparatorText("##SEP_MID_1");

    ImGui::Text("Drag INT, not capped");
    ImGui::DragInt("##drag int", &CheatVars.drag_A, 1);
    ImGui::SeparatorText("##SEP_MID_1");

    // always END the item or you crash!!!
    ImGui::EndTabItem();
}

//======================================================================================================================
void DrawMore() {
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Text(OBFUSCATE("More Tab!"));
    ImGui::SeparatorText(OBFUSCATE("##SEP_MID_1"));

    ImGui::TextColored(MenuVars.textColor, "Slider with INTEGERS - 1 to 100");
    ImGui::SliderInt("##_dmg", &CheatVars.damageMultiplier, 1, 100);
    ImGui::SameLine();
    HelpMarker("Slider from 1 to 100");
    ImGui::SameLine();
    ImGui::Text("§ %d", CheatVars.damageMultiplier);
    ImGui::SeparatorText(OBFUSCATE("##SEP_MID_1"));

    ImGui::TextColored(MenuVars.textColor, "Slider with FLOATS - 1.0 to 10.0");
    ImGui::SliderFloat("##_dmg_f", &CheatVars.damageMultiplierF, 1.0f, 10.0f, "%.1f");
    ImGui::SameLine();
    HelpMarker("Slider from 1.0 to 10.0");
    ImGui::SameLine();
    ImGui::Text("§ %f", CheatVars.damageMultiplierF);
    ImGui::SeparatorText(OBFUSCATE("##SEP_MID_1"));

    ImGui::Text("Drag INT, capped 0 - 100");
    ImGui::DragInt("##drag int 0..100_", &CheatVars.drag_B, 1, 0, 100, "%d%",
                   ImGuiSliderFlags_AlwaysClamp);
    ImGui::SeparatorText("##SEP_MID_1");

    // always END the item or you crash!!!
    ImGui::EndTabItem();
}

//======================================================================================================================
void DrawEvenMore() {
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Text(OBFUSCATE("Even More Tab!"));
    ImGui::SeparatorText(OBFUSCATE("##SEP_MID_1"));

    // simple checkbox
    ImGui::Checkbox("Check me nii-san!", &CheatVars.bool_checkbox_A);
    ImGui::SameLine();
    HelpMarker("Onii-san!");
    ImGui::SeparatorText(OBFUSCATE("##SEP_MID_1"));

    // simple radio buttons
    ImGui::RadioButton("Offline!", &CheatVars.int_radio_A, 0);
    ImGui::SameLine();
    ImGui::RadioButton("On-AIR!", &CheatVars.int_radio_A, 1);
    ImGui::SameLine();
    ImGui::SameLine();
    HelpMarker("Kimochi!");
    ImGui::SeparatorText(OBFUSCATE("##SEP_MID_1"));

    // buttons
    ImGui::Button("I do nothing!");
    ImGui::SameLine();
    if (ImGui::Button("I do math")) {
        CheatVars.int_button_math++;
    }
    ImGui::SameLine();
    HelpMarker("Tap to ++!");
    ImGui::SameLine();
    ImGui::Text("Counter is: %d", CheatVars.int_button_math);
    ImGui::SeparatorText(OBFUSCATE("##SEP_MID_1"));

    // ESP Control via simple checkboxes
    ImGui::Checkbox("Draw Line", &CheatVars.bool_draw_line);
    ImGui::SameLine();
    ImGui::Checkbox("Draw Box", &CheatVars.bool_draw_box);
    ImGui::SeparatorText(OBFUSCATE("##SEP_MID_1"));

    // always END the item or you crash!!!
    ImGui::EndTabItem();
}

//======================================================================================================================
void DrawStyleEditor() {
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Text(OBFUSCATE("Style Editor Tab!"));
    ImGui::SeparatorText(OBFUSCATE("##SEP_MID_1"));

    ImGui::Spacing();
    ImGui::Text(OBFUSCATE("Default Style Picker"));
    ImGui::Combo(OBFUSCATE("##Default Style Picker"), &StyleVars.style_selection, style_list,
                 IM_ARRAYSIZE(style_list), 15);
    ImGui::Spacing();
    if (ImGui::Button(OBFUSCATE("Save"), ImVec2(170, 50))) {
        // save config
        SaveStyle(StyleVars);
    }

    // always END the item or you crash!!!
    ImGui::EndTabItem();
}

//======================================================================================================================
void DrawOtherStuff() {

    ImGui::SeparatorText(OBFUSCATE("##SEP_MID_0"));

    // let's setup an item bar
    if (ImGui::BeginTabBar("_tab_bar", ImGuiTabBarFlags_FittingPolicyScroll)) {

        // first element on the bar
        if (ImGui::BeginTabItem("Cheats", NULL, ImGuiTabItemFlags_Leading)) {
            DrawCheats();
        }

        // second element of the tab
        if (ImGui::BeginTabItem("More")) {
            DrawMore();
        }

        // third element of the tab
        if (ImGui::BeginTabItem("Even More")) {
            DrawEvenMore();
        }

        // fourth and last element of the tab
        if (ImGui::BeginTabItem("Style Editor")) {
            DrawStyleEditor();
        }

        // always END the bar or you crash!!!
        ImGui::EndTabBar();
    }
}

//======================================================================================================================
void DrawEsp() {

    // for making things simpler I only draw one thing per element
    // but usually you would have to loop and draw for each entity

    if (CheatVars.bool_draw_line) {
        /*
        ** Start coords as ImVec2
        ** End coords as ImVec2
        ** Color as ImVec4
        */
        DrawLine({0, 0}, {250, 550}, MenuVars.color_red);

        /*
        ** Coords as ImVec2
        ** Color as ImVec4
        ** Text as const char *
        ** Font size as float
        */
        DrawText({252, 500}, MenuVars.color_purple, "[X] Line from 0:0 to 250:550", 30.0f);
    }

    if (CheatVars.bool_draw_box) {
        DrawBox({250, 550}, {350, 650}, MenuVars.color_green);
        DrawText({350, 650}, MenuVars.color_red, "[X] Box from 250, 550 to 350, 650", 30.0f);
    }
}

//======================================================================================================================
void DrawColumnsDrivenMenu() {

    // setup how many columns you want
    ImGui::Columns(2);

    // draw left section
    DrawLeftColumn();

    // go to next column, here I don't care about the size, let's use it all
    ImGui::NextColumn();

    // draw the rest
    DrawOtherStuff();

    // draw esp stuff
    DrawEsp();
}
//======================================================================================================================