
workspace "AI"
  language "C++"
  location("./build")
  platforms { "Win64" }
  
  configurations { "Debug", "Release" }
  defines {"SFML_STATIC" , "_CRT_SECURE_NO_WARNINGS"}
  
  includedirs{
    "./include/",
    "./deps/sfml/include",
    "./deps/imgui/include",
    }

  files {
    "./deps/sfml/include/**.h*",
    "./deps/imgui/src/**.c*",
    "./deps/imgui/include/**.h*"
  }
    
  libdirs { "./deps/sfml/lib" }
  links {
	"freetype",
    "winmm",
    "opengl32"
  }

  filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"
    links {	
	  "sfml-graphics-s-d",
	  "sfml-window-s-d",
	  "sfml-system-s-d"
      }
   
  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"
    links {
      "sfml-graphics-s",
	  "sfml-window-s",
	  "sfml-system-s"
      }

project "AI_BOARD"
  kind "ConsoleApp"
  location("./build/AI_BOARD")
  targetdir "./bin"
  files { 
    "./src/**.c*",
    "./include/**.h"
    }