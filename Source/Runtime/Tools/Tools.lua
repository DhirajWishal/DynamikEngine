-- Copyright 2020 Dhiraj Wishal
-- SPDX-License-Identifier: Apache-2.0

---------- Tools project description ----------

project "Tools"
	kind "StaticLib"
	language "C++"
	systemversion "latest"
	cppdialect "C++17"
	staticruntime "On"

	targetdir "$(SolutionDir)Builds/Engine/Binaries/$(Configuration)-$(Platform)/$(ProjectName)"
	objdir "$(SolutionDir)Builds/Engine/Intermediate/$(Configuration)-$(Platform)/$(ProjectName)"

	pchheader "dmkafx.h"
	pchsource "../Core/PCH/dmkafx.cpp"

	files {
		"**.txt",
		"**.cpp",
		"**.h",
		"**.lua",
		"**.txt",
		"**.md",
	}

	includedirs {
		"$(SolutionDir)Dependencies/Libraries/Local",
		"$(SolutionDir)Source/Runtime/",
		"$(SolutionDir)Source/Runtime/Core/PCH/",
		"$(SolutionDir)Dependencies/ThirdParty/imgui",
		"$(SolutionDir)Dependencies/ThirdParty/SPIRV-Cross",
		"%{IncludeDir.Vulkan}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.gli}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.zlib}",
		"%{IncludeDir.SPIRVTools}",
		"%{IncludeDir.glslang}",
	}

	libdirs {
		"%{IncludeLib.Vulkan}",
		"%{IncludeLib.GLFW}",
		"%{IncludeLib.GLEW}",
		"%{IncludeLib.SPIRVTools}",
	}

	links { 
		"glew32s",
		"opengl32",
		"glfw3dll",
		"vulkan-1",
		"SPIRV-Tools",
		"SPIRV-Cross",
	}

	filter "system:windows"
		defines {
			"DMK_PLATFORM_WINDOWS",
		}

	filter "system:linux"
		defines {
			"DMK_PLATFORM_LINUX",
		}

	filter "system:macosx"
		defines {
			"DMK_PLATFORM_MAC",
		}