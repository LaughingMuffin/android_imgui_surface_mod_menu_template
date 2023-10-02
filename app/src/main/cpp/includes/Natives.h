//
// Created by Laughing Muffin on 18.09.2023
//
//======================================================================================================================
#pragma once
//======================================================================================================================
#include <Globals.h>
#include <Menu.h>
//======================================================================================================================
static jobject getGlobalContext() {
    jclass activityThread = global_env->FindClass(OBFUSCATE("android/app/ActivityThread"));
    jmethodID currentActivityThread = global_env->GetStaticMethodID(activityThread, OBFUSCATE("currentActivityThread"), OBFUSCATE("()Landroid/app/ActivityThread;"));
    jobject at = global_env->CallStaticObjectMethod(activityThread, currentActivityThread);

    jmethodID getApplication = global_env->GetMethodID(activityThread, OBFUSCATE("getApplication"), OBFUSCATE("()Landroid/app/Application;"));
    jobject context = global_env->CallObjectMethod(at, getApplication);
    return context;
}
//======================================================================================================================
void setupConfigFile() {

    jobject context = getGlobalContext();
    if(!context) {
        Error_Log("I'm stupid and failed to get context apparently!");
        exit(999);
    }

    // get context object class once
    auto contextClass = global_env->FindClass(OBFUSCATE("android/content/Context"));

    // get files dir absolute path
    auto filesDirPtr = global_env->GetMethodID(contextClass, OBFUSCATE("getFilesDir"), OBFUSCATE("()Ljava/io/File;"));
    auto filesDirObject = global_env->CallObjectMethod(context, filesDirPtr);
    auto filesDirClass = global_env->GetObjectClass(filesDirObject);

    auto getFilesDirAbsolutePathMethod = global_env->GetMethodID(filesDirClass, OBFUSCATE("getAbsolutePath"), OBFUSCATE("()Ljava/lang/String;"));

    const char *filesDir = global_env->GetStringUTFChars((jstring) global_env->CallObjectMethod(filesDirObject, getFilesDirAbsolutePathMethod), 0);

    strcat(Vars.StylePath, filesDir);
    strcat(Vars.StylePath, OBFUSCATE("/"));
    char *name_cstr = new char[StyleVars.name.length() + 1];
    strcpy(name_cstr, StyleVars.name.c_str());
    strcat(Vars.StylePath, name_cstr);
}
//======================================================================================================================
void native_Init (JNIEnv *env, jclass clazz, jobject surface) {

    if (g_Initialized)
        return;

    g_NativeWindow = ANativeWindow_fromSurface(env, surface);

    ImGui::CreateContext();
    ImGuiStyle *style                        = &ImGui::GetStyle();
    style->WindowTitleAlign     = ImVec2(0, 0.50);
    style->FrameBorderSize      = 1;
    style->WindowRounding       = 5.3f;
    style->ScrollbarRounding    = 0;
    style->FramePadding         = ImVec2(8, 6);
    style->ScaleAllSizes(2.0f);
    style->ScrollbarSize /= 1;
    style->WindowMinSize = ImVec2(400, 180);

    ImGuiIO *io = &ImGui::GetIO();

    ImGui_ImplAndroid_Init(g_NativeWindow);
    ImGui_ImplOpenGL3_Init(OBFUSCATE("#version 100"));

    ImFontConfig font_cfg;
    io->Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(myFont), sizeof(myFont), 28);

    font_cfg.SizePixels = 28;
    io->Fonts->AddFontDefault(&font_cfg);

    // load config once
    StyleVars = LoadStyles();

    g_Initialized = true;

}
//======================================================================================================================
void native_SurfaceChanged (JNIEnv *env, jclass clazz, jobject gl, jint width, jint height) {

    // get some info to setup menu
    glWidth = width;
    glHeight = height;
    Debug_Log("W - %d | H - %d", width, height);
    glViewport(0, 0, width, height);

    // update imgui display size
    ImGuiIO *io = &ImGui::GetIO();
    io->DisplaySize = ImVec2((float)width, (float)height);

}
//======================================================================================================================
void native_Tick(JNIEnv *env, jclass clazz, jobject thiz) {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(glWidth, glHeight);
    ImGui::NewFrame();

    // setup win initial pos coords and size
    MenuVars.winWidth = glWidth * 0.95f;
    MenuVars.winHeight = 0.0f;
    MenuVars.winPosWidth = glWidth * 0.05f;
    MenuVars.winPosHeight = glHeight * 0.10f;

    // init the win
    ImGui::SetNextWindowPos(ImVec2(MenuVars.winPosWidth, MenuVars.winPosHeight), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize({MenuVars.winWidth, MenuVars.winHeight});

    // sets win collapsed, once - second param is kinda useless but it's a good example, you could make use of that kind
    // of params for other kind of behaviours :)
    ImGui::SetNextWindowCollapsed(true, ImGuiCond_::ImGuiCond_Once);

    // this is the top bar of the menu, collapsed or not
    ImGui::Begin(OBFUSCATE("@androidrepublic.org - ImGUI Surface Template"));

    // draw a menu column based
    DrawColumnsDrivenMenu();

    // setup style
    switch (StyleVars.style_selection) {
        case 0:
            ImGui::StyleColorsClassic();
            break;
        case 1:
            ImGui::StyleColorsLight();
            break;
        case 2:
            ImGui::StyleColorsDark();
            break;
        case 3:
            ImGui::StyleColorsGreenBlue();
            break;
        case 4:
            ImGui::StyleColorsRedDark();
            break;
        case 5:
            ImGui::StyleColorsDeepDark();
            break;
        case 6:
            ImGui::StyleColorsGoldenDark();
            break;
        case 7:
            ImGui::StyleColorsDarkGray();
            break;
        case 8:
            ImGui::StyleColorsGray();
            break;
        case 9:
            ImGui::StyleColorsSoftDarkRed();
            break;
        case 10:
            ImGui::StyleColorsSteamHalfLife();
            break;
    }

    // that's it, now close some stuff and then render it :)
    ImGui::End();

    ImGui::Render();
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
//======================================================================================================================
void native_Shutdown (JNIEnv *env, jclass clazz) {

    if (!g_Initialized)
        return;

    // shut down everything
    g_Initialized = false;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();
    ANativeWindow_release(g_NativeWindow);

}
//======================================================================================================================
jboolean native_Initialized (JNIEnv *env, jclass clazz) { return g_Initialized; }
//======================================================================================================================
jstring native_stringFromJNI (JNIEnv *env, jclass clazz) {
    std::string jniString = "🥴";
    return env->NewStringUTF(jniString.c_str());
}
//======================================================================================================================
int registerNativeFunctions (JNIEnv *env) {

    // declare the array of native methods you want to register
    // JAVA NAME | JAVA SIGNATURE | POINTER TO NATIVE IMPLEMENTATION
    JNINativeMethod methods[] = {
        {OBFUSCATE("Init"), OBFUSCATE("(Landroid/view/Surface;)V"), reinterpret_cast<void *>(native_Init)},
        {OBFUSCATE("SurfaceChanged"), OBFUSCATE("(Ljavax/microedition/khronos/opengles/GL10;II)V"), reinterpret_cast<void *>(native_SurfaceChanged)},
        {OBFUSCATE("Tick"), OBFUSCATE("(Lorg/muffin/imgui/muffin/MuffinSurface;)V"), reinterpret_cast<void *>(native_Tick)},
        {OBFUSCATE("Shutdown"), OBFUSCATE("()V"), reinterpret_cast<void *>(native_Shutdown)},
        {OBFUSCATE("Initialized"), OBFUSCATE("()Z"), reinterpret_cast<void *>(native_Initialized)}
    };

    // let's find class and ensure it's there
    jclass clazz = env->FindClass(OBFUSCATE("org/muffin/imgui/muffin/MuffinSurface"));
    if (!clazz)
        return -1;

    // pass everything to register method under JNIEnv
    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) != 0)
        return -1;

    // declare a new array of native methods you want to register, you can do it more than once :)
    JNINativeMethod moreMethods[] = {
        {OBFUSCATE("stringFromJNI"), OBFUSCATE("()Ljava/lang/String;"), reinterpret_cast<void *>(native_stringFromJNI)}
    };

    // find class and ensure it's there
    jclass anotherClazz = env->FindClass(OBFUSCATE("org/muffin/imgui/MainActivity"));
    if (!anotherClazz)
        return -1;

    // register them :=)
    if (env->RegisterNatives(anotherClazz, moreMethods, sizeof(moreMethods) / sizeof(moreMethods[0])) != 0)
        return -1;

    return 0;
}
//======================================================================================================================