cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Phase LANGUAGES CXX)

set(rishi_src_dir "../Phase/src")


add_executable(Phase ../Phase/vendor/stb_image/stb_image.cpp 

${rishi_src_dir}/main.cpp 
${rishi_src_dir}/Maths.cpp 
${rishi_src_dir}/Log.cpp 
${rishi_src_dir}/CommandLineFeatures.cpp 
${rishi_src_dir}/Constants.cpp

${rishi_src_dir}/Application/Application.cpp 
${rishi_src_dir}/Application/EventCallbacks.cpp

${rishi_src_dir}/AssetManagement/Font.cpp 
${rishi_src_dir}/AssetManagement/StandardTexture.cpp 
${rishi_src_dir}/AssetManagement/Texture.cpp

${rishi_src_dir}/BlockSpawnerFunc/DefaultSpawnerFunc.cpp 
${rishi_src_dir}/BlockSpawnerFunc/TutorialSpawnerFunc.cpp

${rishi_src_dir}/Collision/Collision.cpp
${rishi_src_dir}/Core/StringHelper.cpp

${rishi_src_dir}/Events/BackgroundLayer.cpp 
${rishi_src_dir}/Events/BlockSpawnerLayer.cpp 
${rishi_src_dir}/Events/CreditLayer.cpp 
${rishi_src_dir}/Events/FadeoutScreenLayer.cpp 
${rishi_src_dir}/Events/Layer.cpp 
${rishi_src_dir}/Events/UILayer.cpp 
${rishi_src_dir}/Events/MainMenu/MainMenuLayer.cpp 
${rishi_src_dir}/Events/Player/AbstractPlayerLayer.cpp 
${rishi_src_dir}/Events/Player/NormalPlayerLayer.cpp
${rishi_src_dir}/Events/Player/NormalPlayerLayerUI.cpp 
${rishi_src_dir}/Events/Player/ZenPlayerLayer.cpp 
${rishi_src_dir}/Events/Tutorial/TutorialBlockSpawnerLayer.cpp 

${rishi_src_dir}/Events/Tutorial/TutorialPlayerLayer.cpp

${rishi_src_dir}/Renderer/Renderer.cpp 
${rishi_src_dir}/Renderer/RendererShapes.cpp 
${rishi_src_dir}/Renderer/RendererVertex.cpp 

${rishi_src_dir}/UI/Button.cpp 
${rishi_src_dir}/UI/PushButton.cpp 
${rishi_src_dir}/UI/UIObject.cpp
${rishi_src_dir}/UI/UIImage.cpp
)

target_compile_definitions(Phase PRIVATE GLEW_STATIC RM_MAC)

target_include_directories(Phase PRIVATE ./Phase/src ./Phase/vendor/Freetype ./Phase/vendor/GLEW/include ./Phase/vendor/GLFW/include ./Phase/vendor/glm ./Phase/vendor/spdlog/include ./Phase/vendor/stb_image)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message ("rishi darwin")
    set(rishi_lib_path "/Users/rishimenon/Desktop/dev/cpp/Dasher/Phase/Phase/vendor")
	
    TARGET_LINK_LIBRARIES (Phase 
    ${rishi_lib_path}/GLEW/lib/Release/mac/glew.a 
    ${rishi_lib_path}/GLFW/lib-mac/glfw3.a 
    ${rishi_lib_path}/Freetype/lib-mac/freetype.a
    "-framework Cocoa"
    "-framework IOKit"
    "-framework OpenGL"
    /usr/lib/libbz2.1.0.dylib 
    /usr/lib/libz.1.dylib
    )

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    message ("rishi windows")
    set(rishi_lib_path "../Phase/vendor")

    target_link_libraries(Phase 
    ${rishi_lib_path}/GLEW/lib/Release/x64/glew32s
    ${rishi_lib_path}/GLFW/lib-x64/glfw3 
    ${rishi_lib_path}/Freetype/lib-x64/freetype 
    opengl32
    )

endif()