local plugin = {};

require("gbs.sys.string");

local lib = require("gbs.sys.lib");
local fs  = require("gbs.sys.fs");
local app = require("gbs.sys.app");

function plugin:new (prj)
    local o = {
        project = function () return prj; end
    }; 
    
    self.__index = self;
    return setmetatable(o, self);
end

function plugin:create ()
    local pro = self:project();
    local gbs = pro:gbs(); 
    local gbsHomeDir = gbs:homeDir();
    
    local proName = pro:name();
    local proDir  = fs.join(".gbs", proName);
    local proFile = fs.join(proDir, gbs:projectFileName());
    local proLanguage = pro:language();
    local proType = pro:type();
    local proSrcFileList = {};
    local proIncludeDirList = {};
    
    -- Create source directory
    local srcDir = gbs:sourcesDirName();
    if proType == "test" then
        srcDir = gbs:testsDirName();
    end
    
    if not fs.exists(srcDir) then
        lib.assert(fs.mkdir(srcDir));
    end
    
    local proSrcDir = fs.join(srcDir, proName);
    if not fs.exists(proSrcDir) then
         lib.assert(fs.mkdir(proSrcDir));
    end
    
    --
    -- Create `include' directory if project is a library
    --
    if proType == "shared-lib" or proType == "static-lib" then
        if not fs.exists("include") then
            lib.assert(fs.mkdir("include"));
        end
        table.insert(proIncludeDirList, string.quote("../../include"))
        if proLanguage == "C++" then
            table.insert(proSrcFileList, string.quote("../../include/**.hpp"));
            table.insert(proSrcFileList, string.quote("../../include/**.h"));
        elseif proLanguage == "C" then
            table.insert(proSrcFileList, string.quote("../../include/**.h"));
        end
    end
    
    local templateDir = nil;
    local mainSrc = nil;
    local mainDest = nil;
    
    if proType == "test" then
        if proLanguage == "C++" then
            templateDir = fs.join(gbsHomeDir, "template", "cpp");
            mainSrc = fs.join(templateDir, proType .. ".cpp");
            mainDest = fs.join(proSrcDir, "test.cpp");
            table.insert(proSrcFileList, string.quote("../../" .. gbs:testsDirName() .. "/" .. proName .. "/**.cpp"));
        elseif proLanguage == "C" then
            templateDir = fs.join(gbsHomeDir, "template", "c");
            mainSrc = fs.join(templateDir, proType .. ".c");
            mainDest = fs.join(proSrcDir, "test.c");
            table.insert(proSrcFileList, string.quote("../../" .. gbs:testsDirName() .. "/" .. proName .. "/**.c"));
        end
    else
        if proLanguage == "C++" then
            templateDir = fs.join(gbsHomeDir, "template", "cpp");
            mainSrc = fs.join(templateDir, proType .. ".cpp");
            mainDest = fs.join(proSrcDir, "main.cpp");
            table.insert(proSrcFileList, string.quote("../../" .. gbs:sourcesDirName() .. "/" .. proName .. "/**.cpp"));
    --        table.insert(proSrcFileList, "src/" .. proName .. "/**.hpp");
    --        table.insert(proSrcFileList, "src/" .. proName .. "/**.h");
        elseif proLanguage == "C" then
            templateDir = fs.join(gbsHomeDir, "template", "c");
            mainSrc = fs.join(templateDir, proType .. ".c");
            mainDest = fs.join(proSrcDir, "main.c");
            table.insert(proSrcFileList, string.quote("../../" .. gbs:sourcesDirName() .. "/" .. proName .. "/**.c"));
    --        table.insert(proSrcFileList, "src/" .. proName .. "/**.h");
        end
    end
    
    lib.assert(mainSrc);
    lib.assert(mainDest);

    if fs.exists(mainSrc) then
        lib.assert(fs.copy(mainSrc, mainDest));
    end    

    local proDependson = nil;
    if gbs:hasOpt("depends") then
        local t = string.quote(gbs:optarg("depends"));
        proDependson = string.join(", ", t);
    end

    local slnName        = gbs:solutionName();
    local proKind        = pro:premakeKind();
    local proLang        = pro:premakeLang();
    local proObjDir      = string.join("/", "../../../.build", slnName, proName);
    
    local proTargetDir   = string.join("/", "../../../.build");
    if proType == "test" then
        proTargetDir = string.join("/", "../../../.build", "tests");
    end
    
    local proSrcFiles    = string.join(", ", proSrcFileList);
    local proIncludeDirs = string.join(", ", proIncludeDirList);
    
    lib.assert(fs.appendLines(proFile, {
          "--************************************************************"
        , "--* Generated automatically by `" .. app.name() .. "'"
        , "--* Command: `" .. app.cmdline() .. "'"
        , "--* Date:    " .. os.date() 
        , "--************************************************************"
    }));    
    
    lib.assert(fs.appendLines(proFile, {
          "kind          " .. string.quote(proKind)
        , "language      " .. string.quote(proLang)
        , "targetname    " .. string.quote(proName)
        , "includedirs { " .. proIncludeDirs .. " }"
        , "files { " .. proSrcFiles .. " }"
        , ""
        , "configuration " .. string.quote("debug")
        , "    objdir       " .. string.quote(proObjDir .. "/debug")
        , "    targetdir    " .. string.quote(proTargetDir)
        , "    targetsuffix " .. string.quote("-d")
        , ""
        , "configuration " .. string.quote("release")
        , "    objdir       " .. string.quote(proObjDir .. "/release")
        , "    targetdir    " .. string.quote(proTargetDir)
    }));
    
    if proDependson then
        lib.assert(fs.appendLines(proFile, "dependson { " .. proDependson .." }")); 
    end

    local inc1 = [[include(os.getenv("GBS_HOME") .. "]] 
        .. fs.separator() 
        .. fs.join("premake", "filter_action_gmake_install") 
        .. [[")]];
    
    lib.assert(fs.appendLines(proFile
        , ""
        , inc1
    ));
   
    return true;
end

return plugin;
