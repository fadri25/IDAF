project "GLEW"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir ("bin/" .. outdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outdir .. "/%{prj.name}")


	files {
		"include/GL/eglew.h",
		"include/GL/glew.h",
		"include/GL/glxew.h",
		"include/GL/wglew.h"
	}

	includedirs {
		"include"
	}

	filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"