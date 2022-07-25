workspace "Flies_Core"
    architecture "x86_64"

    startproject "sandbox"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


group "Deps"
    include "vendor"
group ""



include "core"
include "sandbox"




