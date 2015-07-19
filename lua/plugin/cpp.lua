local CppPlugin = {};

local Lib  = require("../lib");
local Path = require("../path");
local File = require("../file");
local App  = require("../app");

function CppPlugin:new (prj)
    local o = {
        project = function () return prj; end
    }; 
    
    self.__index = self;
    return setmetatable(o, self);
end

function CppPlugin:create ()
    local pro = self:project();
    local gbs = pro:gbs(); 
    local gbsHomeDir = gbs:homeDir();
    
    local proName = pro:name();
    local proDir  = Path.join(".gbs", proName);
    local proFile = Path.join(proDir, gbs:projectFileName());
    local proLanguage = pro:language();
    local proType = pro:type();
    local proSrcFileList = {};
    local proIncludeDirList = {};
    
    -- Create source directory
    local srcDir = gbs:sourcesDirName();
    if proType == "test" then
        srcDir = gbs:testsDirName();
    end
    
    if not Path.exists(srcDir) then
        Lib.assert(Path.mkdir(srcDir));
    end
    
    local proSrcDir = Path.join(srcDir, proName);
    if not Path.exists(proSrcDir) then
         Lib.assert(Path.mkdir(proSrcDir));
    end
    
    --
    -- Create `include' directory if project is a library
    --
    if proType == "shared-lib" or proType == "static-lib" then
        if not Path.exists("include") then
            Lib.assert(Path.mkdir("include"));
        end
        table.insert(proIncludeDirList, Lib.quote("../../include"))
        if proLanguage == "C++" then
            table.insert(proSrcFileList, Lib.quote("../../include/**.hpp"));
            table.insert(proSrcFileList, Lib.quote("../../include/**.h"));
        elseif proLanguage == "C" then
            table.insert(proSrcFileList, Lib.quote("../../include/**.h"));
        end
    end
    
    local templateDir = nil;
    local mainSrc = nil;
    local mainDest = nil;
    
    if proType == "test" then
        if proLanguage == "C++" then
            templateDir = Path.join(gbsHomeDir, "template", "cpp");
            mainSrc = Path.join(templateDir, proType .. ".cpp");
            mainDest = Path.join(proSrcDir, "test.cpp");
            table.insert(proSrcFileList, Lib.quote("../../" .. gbs:testsDirName() .. "/" .. proName .. "/**.cpp"));
        elseif proLanguage == "C" then
            templateDir = Path.join(gbsHomeDir, "template", "c");
            mainSrc = Path.join(templateDir, proType .. ".c");
            mainDest = Path.join(proSrcDir, "test.c");
            table.insert(proSrcFileList, Lib.quote("../../" .. gbs:testsDirName() .. "/" .. proName .. "/**.c"));
        end
    else
        if proLanguage == "C++" then
            templateDir = Path.join(gbsHomeDir, "template", "cpp");
            mainSrc = Path.join(templateDir, proType .. ".cpp");
            mainDest = Path.join(proSrcDir, "main.cpp");
            table.insert(proSrcFileList, Lib.quote("../../" .. gbs:sourcesDirName() .. "/" .. proName .. "/**.cpp"));
    --        table.insert(proSrcFileList, "src/" .. proName .. "/**.hpp");
    --        table.insert(proSrcFileList, "src/" .. proName .. "/**.h");
        elseif proLanguage == "C" then
            templateDir = Path.join(gbsHomeDir, "template", "c");
            mainSrc = Path.join(templateDir, proType .. ".c");
            mainDest = Path.join(proSrcDir, "main.c");
            table.insert(proSrcFileList, Lib.quote("../../" .. gbs:sourcesDirName() .. "/" .. proName .. "/**.c"));
    --        table.insert(proSrcFileList, "src/" .. proName .. "/**.h");
        end
    end
    
    Lib.assert(mainSrc);
    Lib.assert(mainDest);

    if Path.exists(mainSrc) then
        Lib.assert(Path.copy(mainSrc, mainDest));
    end    

    local proDependson = nil;
    if gbs:hasOpt("depend") then
        local t = Lib.quote(gbs:optarg("depend"));
        proDependson = Lib.join(", ", t);
    end

    local slnName        = gbs:solutionName();
    local proKind        = pro:premakeKind();
    local proLang        = pro:premakeLang();
    local proObjDir      = Lib.join("/", "../../../.build", slnName, proName);
    
    local proTargetDir   = Lib.join("/", "../../../.build");
    if proType == "test" then
        proTargetDir = Lib.join("/", "../../../.build", "tests");
    end
    
    local proSrcFiles    = Lib.join(", ", proSrcFileList);
    local proIncludeDirs = Lib.join(", ", proIncludeDirList);
    
    Lib.assert(File.appendLines(proFile, {
          "--************************************************************"
        , "--* Generated automatically by `" .. App.name() .. "'"
        , "--* Command: `" .. App.cmdline() .. "'"
        , "--* Date:    " .. os.date() 
        , "--************************************************************"
    }));    
    
    Lib.assert(File.appendLines(proFile, {
          "kind          " .. Lib.quote(proKind)
        , "language      " .. Lib.quote(proLang)
        , "targetname    " .. Lib.quote(proName)
        , "includedirs { " .. proIncludeDirs .. " }"
        , "files { " .. proSrcFiles .. " }"
        , ""
        , "configuration " .. Lib.quote("debug")
        , "    objdir       " .. Lib.quote(proObjDir .. "/debug")
        , "    targetdir    " .. Lib.quote(proTargetDir)
        , "    targetsuffix " .. Lib.quote("-d")
        , ""
        , "configuration " .. Lib.quote("release")
        , "    objdir       " .. Lib.quote(proObjDir .. "/release")
        , "    targetdir    " .. Lib.quote(proTargetDir)
    }));
    
    if proDependson then
        Lib.assert(File.appendLines(proFile, "dependson { " .. proDependson .." }")); 
    end

    Lib.assert(File.appendLines(proFile, {
          ""
        , 'filter "action:gmake"'
        , "    makesettings [["
        , ""
        , "GBS_INSTALL_FILE    = install -D -m 644 -p"
        , "GBS_INSTALL_BIN     = install -D -m 755 -p"
        , "GBS_INSTALL_DIR     = cp -f -R"
        , "GBS_DEL_FILE        = rm -f"
        , "GBS_SYMLINK         = ln -f -s"
        , "GBS_DEL_DIR         = rmdir"
        , "GBS_BASENAME        = basename"
        , ""
        , "GBS_MAJOR_VER       = 1"
        , "GBS_MINOR_VER       = 0"
        , "GBS_RELEASE         = 0"
        , "GBS_TARGETNAME      = $(shell $(GBS_BASENAME) $(TARGET))"
        , "GBS_TARGET0         = ${HOME}/lib/$(GBS_TARGETNAME)"
        , "GBS_TARGET1         = ${HOME}/lib/$(GBS_TARGETNAME).$(GBS_MAJOR_VER)"
        , "GBS_TARGET2         = ${HOME}/lib/$(GBS_TARGETNAME).$(GBS_MAJOR_VER).$(GBS_MINOR_VER)"
        , "GBS_TARGET3         = ${HOME}/lib/$(GBS_TARGETNAME).$(GBS_MAJOR_VER).$(GBS_MINOR_VER).$(GBS_RELEASE)"
        , ""
        , "install: all"
        , "\t$(GBS_INSTALL_BIN)  $(TARGET) $(GBS_TARGET3)"
        , "\t$(GBS_SYMLINK) $(GBS_TARGET3) $(GBS_TARGET2)"
        , "\t$(GBS_SYMLINK) $(GBS_TARGET3) $(GBS_TARGET1)"
        , "\t$(GBS_SYMLINK) $(GBS_TARGET3) $(GBS_TARGET0)"
        , "\t$(GBS_INSTALL_DIR) ../include ${HOME}/include"
        , ""
        , "uninstall:"
        , "\t$(GBS_DEL_FILE) $(GBS_TARGET0)"
        , "\t$(GBS_DEL_FILE) $(GBS_TARGET1)"
        , "\t$(GBS_DEL_FILE) $(GBS_TARGET2)"
        , "\t$(GBS_DEL_FILE) $(GBS_TARGET3)"
        , "]]"
    }));
    
    return true;
end

return CppPlugin;
