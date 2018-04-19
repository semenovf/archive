local p = premake
p.modules.gbs = {}
p.modules.gbs._VERSION = "1.0.0-dev"

local gbs = p.modules.gbs

newaction {
    trigger         = "gbs",
    shortname       = "Griotte Build System",
    description     = "Module for control development workflow including configure, build and packaging",

    valid_kinds     = { "ConsoleApp", "WindowedApp", "StaticLib", "SharedLib" },

    valid_languages = { "C", "C++" },

    valid_tools     = {
        cc     = { "clang", "gcc" }
    },

    onStart = function()
        print("GBS::onStart()")
    end,

    onInitialize = function()
        print("GBS::onInitialize()")

        if _OPTIONS["configure"] then
            --require("gbs_configure")
            --p.modules.gmake2.cpp.initialize()
            os.exit(0)
        end
    end,

    onWorkspace = function(wks)
        print("GBS::onWorkspace()")
        --p.escaper(p.modules.gmake2.esc)
        --p.generate(wks, p.modules.gmake2.getmakefilename(wks, false), p.modules.gmake2.generate_workspace)
    end,

    onProject = function(prj)
        print("GBS::onProject()")
    --        p.escaper(p.modules.gmake2.esc)
    --        local makefile = p.modules.gmake2.getmakefilename(prj, true)
    --        if prj.kind == p.UTILITY then
    --                p.generate(prj, makefile, p.modules.gmake2.utility.generate)
    --        elseif prj.kind == p.MAKEFILE then
    --                p.generate(prj, makefile, p.modules.gmake2.makefile.generate)
    --        else
    --                if project.isdotnet(prj) then
    --                        p.generate(prj, makefile, p.modules.gmake2.cs.generate)
    --                elseif project.isc(prj) or project.iscpp(prj) then
    --                        p.generate(prj, makefile, p.modules.gmake2.cpp.generate)
    --                end
    --        end
    end,

    onCleanWorkspace = function(wks)
        print("GBS::onCleanWorkspace()")
    --        p.clean.file(wks, p.modules.gmake2.getmakefilename(wks, false))
    end,

    onCleanProject = function(prj)
        print("GBS::onCleanProject()")
    --        p.clean.file(prj, p.modules.gmake2.getmakefilename(prj, true))
    end,


    onEnd = function()
        print("GBS::onEnd()")
    end,
    
    execute = function()
        print("GBS::execute")
    end
}

newoption {
   trigger     = "configure",
   description = "Create configuration file"
}

return gbs
