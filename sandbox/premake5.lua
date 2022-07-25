project "sanbox"
    language "C++"
    files { "src/**.h", "src/**.cpp"}
    kind "ConsoleApp"
    cppdialect "C++20"
    targetdir ("%{wks.location}/build/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/build/obj/" .. outputdir .. "/%{prj.name}")

    includedirs {
        "%{wks.location}/core/src",
        "%{wks.location}/vendor/asio/asio/include"
    }

    links { "core", "asio", "ssl", "crypto" }