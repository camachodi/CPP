
workspace "Workspace"
  language "C++"
  location("./build")
  platforms { "Win64" }

  configurations { "Debug", "Release" }

  flags{ "MultiProcessorCompile" }

  includedirs{ 
    "./include/",
    "./deps",
    "./deps/glfw/include/GLFW", 
    "./deps/soloud20200207/include/", 
    "./deps/loguru/include", 
    "./deps/opengl-4.6/include/glad",
    "./deps/imgui/include", 
    "./deps/glm",
    "./deps/stb",
    "./deps/lua"
    }

  defines {"_CRT_SECURE_NO_WARNINGS", "_GLFW_WIN32", "WITH_WINMM", "_CRT_SECURE_NO_DEPRECATE", "MTR_ENABLED"}

  --links { "opengl32" }
  
  filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"
   
  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"

project "NGine"
  kind "StaticLib"
  location("./build/NGine")
  targetdir "./bin"

  files {

    --glfw

    "./deps/glfw/GLFW/context.c",
    "./deps/glfw/GLFW/egl_context.c",
    "./deps/glfw/GLFW/init.c",
    "./deps/glfw/GLFW/input.c",
    "./deps/glfw/GLFW/monitor.c",
    "./deps/glfw/GLFW/osmesa_context.c",
    "./deps/glfw/GLFW/vulkan.c",
    "./deps/glfw/GLFW/wgl_context.c",
    "./deps/glfw/GLFW/window.c", 
    "./deps/glfw/GLFW/win32_*.c",

    --opengl

    "./deps/opengl-4.6/src/*.c",

    --stb

    "./deps/stb/*.h",

    "./deps/lua/*.c",

    --logger

    "./deps/loguru/src/**.c*",

    --multi_threading_library

    "./deps/px_library/include/**.h*",

    --glm

    "./deps/glm/**.h*",

    --soLoud

    "./deps/soloud20200207/include/**.h*",
    "./deps/soloud20200207/src/core/**.c*",
    "./deps/soloud20200207/src/filter/**.c*",
    "./deps/soloud20200207/src/audiosource/monotone/**.c*",
    "./deps/soloud20200207/src/audiosource/noise/**.c*",
    "./deps/soloud20200207/src/audiosource/openmpt/**.c*",
    "./deps/soloud20200207/src/audiosource/sfxr/**.c*",
    "./deps/soloud20200207/src/audiosource/speech/**.c*",
    "./deps/soloud20200207/src/audiosource/speech/**.h*",
    "./deps/soloud20200207/src/audiosource/tedsid/**.c*",
    "./deps/soloud20200207/src/audiosource/tedsid/**.h*",
    "./deps/soloud20200207/src/audiosource/vic/**.c*",
    "./deps/soloud20200207/src/audiosource/vizsn/**.c*",
    "./deps/soloud20200207/src/audiosource/wav/**.c*",
    "./deps/soloud20200207/src/audiosource/wav/**.h*",
    "./deps/soloud20200207/src/backend/winmm/**.c*",

    --ImGui

    "./deps/imgui/src/**.c*",
    "./deps/imgui/include/**.h*",

    --our classes

    "./src/**.c*",
    "./include/**.h"
    }

    removefiles {
    "./deps/tinyobjloader/examples/viewer/viewer.cc",
    "./deps/tinyobjloader/experimental/viewer.cc",
    "./deps/tinyobjloader/python/bindings.cc",
    }
    
project "NGine_App"
  location("./examples/ngine_App")
  kind "ConsoleApp"
  targetdir "./bin/NGine_App/"
  links { "NGine"}
  
  files { 
    "./examples/ngine_App/**.c*"
    }