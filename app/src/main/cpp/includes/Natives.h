//
// Created by Laughing Muffin on 18.09.2023
//
//======================================================================================================================
#pragma once
//======================================================================================================================
#include <Globals.h>
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

    g_Initialized = true;

}
//======================================================================================================================
void native_SurfaceChanged (JNIEnv *env, jclass clazz, jobject gl, jint width, jint height) {

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

    ImGui::SetNextWindowPos(ImVec2(glWidth * 0.05f, glHeight * 0.10f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize({glWidth * 0.95f, 0.0f});
    ImGui::SetNextWindowCollapsed(true, ImGuiCond_::ImGuiCond_Once);

    ImGui::Begin(OBFUSCATE("@androidrepublic.org - ImGUI Surface Template"));

    ImGui::Text(OBFUSCATE("Hello :)"));

    ImGui::StyleColorsDark();

    ImGui::End();

    ImGui::Render();
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
//======================================================================================================================
void native_Shutdown (JNIEnv *env, jclass clazz) {

    if (!g_Initialized)
        return;

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