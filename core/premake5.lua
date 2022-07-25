project "core"
    kind "SharedLib"

    language "C++"
    files { "src/**.h", "src/**.cpp" }
    targetdir ("%{wks.location}/build/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/build/obj/" .. outputdir .. "/%{prj.name}")
