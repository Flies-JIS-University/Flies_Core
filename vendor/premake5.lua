project "asio"
    kind "StaticLib"
    cppdialect "C++20"
    language "C++"
    files { "asio/asio/src/asio.cpp", "asio/asio/src/asio_ssl.cpp" }
    targetdir ("%{wks.location}/build/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/build/obj/" .. outputdir .. "/%{prj.name}")

    defines {
        "ASIO_STANDALONE",
        "ASIO_SEPARATE_COMPILATION"
    }

    includedirs {
        "%{wks.location}/vendor/asio/asio/include"
    }

    filter "system:Windows"
        includedirs { "C:\\Program Files\\OpenSSL-Win64\\include" }
    filter {}
