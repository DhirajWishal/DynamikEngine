-- Copyright 2020 Dhiraj Wishal
-- SPDX-License-Identifier: Apache-2.0

---------- Core project description ----------

project "Core"
	kind "StaticLib"
	language "C++"
	systemversion "latest"
	cppdialect "C++17"
	staticruntime "On"

	defines {
		"DMK_INTERNAL"
	}

	targetdir "$(SolutionDir)Builds/Framework/Binaries/$(Configuration)-$(Platform)"
	objdir "$(SolutionDir)Builds/Framework/Intermediate/$(Configuration)-$(Platform)/$(ProjectName)"

	files {
		"**.txt",
		"**.cpp",
		"**.h",
		"**.lua",
		"**.txt",
		"**.md",
	}

	includedirs {
		"$(SolutionDir)Framework/",
		"%{IncludeDir.xxhash}",
	}

	libdirs {
		"%{IncludeLib.xxhash}",
	}

	links { 
		"xxhash"
	}