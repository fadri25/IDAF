Setup:

assimps .dll file is located in (should be in solution folder already) : vendor\assimp-master\build\bin\Debug

Include paths:
In Projekteigenschaften (Visual Studio): C/C++/General/Additional Include Directories
vendor/assimp-master/source/include
vendor/glew/include
vendor/glfw-3.3.6.bin.WIN64/include
vendor/glm
vendor/opencv/build/include
vendor/imgui

Library paths:
In Projekteigenschaften (Visual Studio): Linker/General/Additional Library Directories
vendor/glew/lib/Release/x64
vendor/assimp-master/build/lib/Debug
vendor/glfw-3.3.6.bin.WIN64/lib-vc2022 	//hängt von der Visual Studio version ab
vendor/opencv/build\x64/vc15/lib
vendor/imgui/bin/Debug-windows-x867ImGui

Libraries:
In Projekteigenschaften (Visual Studio): Linker/Input/Additional Deoendencies
opengl32.lib
assimp-vc143-mtd.lib
glew32s.lib
glfw3.lib
opencv_world455.lib
ImGui.lib

Preprocessor defines:
In Projekteigenschaften (Visual Studio): C/C++/Preprocessor/Preprocessor Definitions
_DEBUG
_CONSOLE
GLEW_STATIC
_CRT_SECURE_NO_WARNINGS
GLM_FORCE_RADIANS
_USE_MATH_DEFINES


Application Controls:
wasd - model rotation
arrow keys 	- model translation
i & k keys 	- model translation z-axis
t & g keys 	- scale model
F1 key	- reset model
p & n 	- change view models(filters)
Esc key 	- close application


Neue Models hinzufügen:
in Ordner res/models/models.txt den Namen ohne Dateiendung des models
.obj und .mtl Datei sollten in dem Ordner res/models in einen eigenen Ordner
mit dem selben Namen wie die .obj Datei verschoben werden
Material- und Texturedateien müssen im selben Ornder wie das Model sein

Falls Model nicht geladen werden kann (in Konsole: "dobe_....."):
	in Material Datei alle Zeilen die mit "adobe" beginnen löschen
Falls Model nicht geladen werden kann (in Konsole: cannot load image: ...):
	Datei Pfad von texturen in Material Datei anpassen zu : 
	[Dateiname].png (ohne Pfadelemente vor dem Dateinamen)
