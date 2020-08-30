cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(MyProject LANGUAGES CXX)


INCLUDE_DIRECTORIES(../Phase/src ../Phase/vendor/Freetype ../Phase/vendor/GLEW/include ../Phase/vendor/GLFW/include ../Phase/vendor/glm ../Phase/vendor/spdlog/include ../Phase/vendor/stb_image)

LINK_DIRECTORIES(../Phase/vendor/Freetype/lib-x64 ../Phase/vendor/GLEW/lib/Release/x64 ../Phase/vendor/GLFW/lib-x64)

add_executable(phase ../Phase/vendor/stb_image/stb_image.cpp 
../Phase/src/main.cpp ../Phase/src/Maths.cpp ../Phase/src/Log.cpp ../Phase/src/CommandLineFeatures
../Phase/src/Application/Application.cpp ../Phase/src/Application/EventCallbacks.cpp
../Phase/src/AssetManagement/Font.cpp ../Phase/src/AssetManagement/StandardTexture.cpp ../Phase/src/AssetManagement/Texture.cpp
../Phase/src/BlockSpawnerFunc/DefaultSpawnerFunc.cpp ../Phase/src/BlockSpawnerFunc/TutorialSpawnerFunc.cpp
../Phase/src/Collision/Collision.cpp
../Phase/src/Events/BackgroundLayer.cpp ../Phase/src/Events/BlockSpawnerLayer.cpp ../Phase/src/Events/CreditLayer.cpp ../Phase/src/Events/FadeoutScreenLayer.cpp ../Phase/src/Events/Layer.cpp ../Phase/src/Events/UILayer.cpp 
../Phase/src/Events/MainMenu/MainMenuLayer.cpp ../Phase/src/Events/Player/AbstractPlayerLayer.cpp ../Phase/src/Events/Player/NormalPlayerLayer.cpp ../Phase/src/Events/Player/ZenPlayerLayer.cpp ../Phase/src/Events/Tutorial/TutorialBlockSpawnerLayer.cpp ../Phase/src/Events/Tutorial/TutorialPlayerLayer.cpp
../Phase/src/Renderer/Renderer.cpp ../Phase/src/Renderer/RendererShapes.cpp ../Phase/src/Renderer/RendererVertex.cpp ../Phase/src/UI/Button.cpp ../Phase/src/UI/PushButton.cpp ../Phase/src/UI/UIObject.cpp)

add_compile_definitions (GLEW_STATIC RM_WIN)

TARGET_LINK_LIBRARIES(phase glew32s glfw3 freetype opengl32)

