workspace "Flies_Core"
    architecture "x86_64"

    startproject "sandbox"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

--     filter "system:Windows"
--     system "windows"

--  filter "system:Unix"
--     system "linux"

--  filter "system:Mac"
--     system "macosx"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


group "Deps"
    include "vendor"
group ""



include "core"
include "sandbox"




