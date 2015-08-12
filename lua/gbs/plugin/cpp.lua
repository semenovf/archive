require "pfs.string";

local fs  = require("pfs.sys.fs");

local plugin = {};

function plugin:new (prj)
    local o = {
        project = function () return prj; end
    }; 
    
    self.__index = self;
    return setmetatable(o, self);
end

function plugin:create ()
    local project = self:project();
    local gbs = project:gbs(); 
    local gbsHomeDir = gbs:homeDir();
    
    local projectName = project:name();
    local projectDir  = fs.join(".gbs", projectName);
    local projectFile = fs.join(projectDir, gbs:projectFileName());
    local projectLanguage = project:language() or lib.die("invalid project language");
    local projectType = project:type() or lib.die("invalid project type");
    local projectSrcFileList = {};
    local projectIncludeDirList = {};
    
    -- Create source directory
    local srcDir = gbs:sourcesDirName();
    if projectType == "test" then
        srcDir = gbs:testsDirName();
    end
    
    if not fs.exists(srcDir) then
        lib.assert(fs.mkdir(srcDir));
    end
    
    local projectSrcDir = fs.join(srcDir, projectName);
    if not fs.exists(projectSrcDir) then
         lib.assert(fs.mkdir(projectSrcDir));
    end
    
    --
    -- Create `include' directory if project is a library
    --
    if projectType == "shared-lib" or projectType == "static-lib" then
        if not fs.exists("include") then
            lib.assert(fs.mkdir("include"));
        end
        table.insert(projectIncludeDirList, string.quote("../../include"))
        if projectLanguage == "C++" then
            table.insert(projectSrcFileList, string.quote("../../include/**.hpp"));
            table.insert(projectSrcFileList, string.quote("../../include/**.h"));
        elseif projectLanguage == "C" then
            table.insert(projectSrcFileList, string.quote("../../include/**.h"));
        end
    end
    
    local templateDir = nil;
    local mainSrc = nil;
    local mainDest = nil;
    
    if projectType == "test" then
        if projectLanguage == "C++" then
            templateDir = fs.join(gbsHomeDir, "template", "cpp");
            mainSrc = fs.join(templateDir, projectType .. ".cpp");
            mainDest = fs.join(projectSrcDir, "test.cpp");
            table.insert(projectSrcFileList, string.quote("../../" .. gbs:testsDirName() .. "/" .. projectName .. "/**.cpp"));
        elseif projectLanguage == "C" then
            templateDir = fs.join(gbsHomeDir, "template", "c");
            mainSrc = fs.join(templateDir, projectType .. ".c");
            mainDest = fs.join(projectSrcDir, "test.c");
            table.insert(projectSrcFileList, string.quote("../../" .. gbs:testsDirName() .. "/" .. projectName .. "/**.c"));
        end
    else
        if projectLanguage == "C++" then
            templateDir = fs.join(gbsHomeDir, "template", "cpp");
            mainSrc = fs.join(templateDir, projectType .. ".cpp");
            mainDest = fs.join(projectSrcDir, "main.cpp");
            table.insert(projectSrcFileList, string.quote("../../" .. gbs:sourcesDirName() .. "/" .. projectName .. "/**.cpp"));
    --        table.insert(projectSrcFileList, "src/" .. projectName .. "/**.hpp");
    --        table.insert(projectSrcFileList, "src/" .. projectName .. "/**.h");
        elseif projectLanguage == "C" then
            templateDir = fs.join(gbsHomeDir, "template", "c");
            mainSrc = fs.join(templateDir, projectType .. ".c");
            mainDest = fs.join(projectSrcDir, "main.c");
            table.insert(projectSrcFileList, string.quote("../../" .. gbs:sourcesDirName() .. "/" .. projectName .. "/**.c"));
    --        table.insert(projectSrcFileList, "src/" .. projectName .. "/**.h");
        end
    end
    
    lib.assert(mainSrc);
    lib.assert(mainDest);

    if fs.exists(mainSrc) then
        lib.assert(fs.copy(mainSrc, mainDest));
    end    

    local projectDependson = nil;
    if gbs:hasOpt("depends") then
        local t = string.quote(gbs:optarg("depends"));
        projectDependson = string.join(", ", t);
    end

    local solutionName = gbs:solutionName();
    local projecObjDir    = string.join("/", "../../../.build", solutionName, projectName);
    
    local projectTargetDir   = string.join("/", "../../../.build");
    if projectType == "test" then
        projectTargetDir = string.join("/", "../../../.build", "tests");
    end
    
    local proSrcFiles    = string.join(", ", projectSrcFileList);
    local proIncludeDirs = string.join(", ", projectIncludeDirList);
    
    lib.assert(fs.appendLines(projectFile, {
          "--************************************************************"
        , "--* Generated automatically by `" .. app.name() .. "'"
        , "--* Command: `" .. app.cmdline() .. "'"
        , "--* Date:    " .. os.date() 
        , "--************************************************************"
    }));    
    
    lib.assert(fs.appendLines(projectFile, {
          "kind          " .. string.quote(project:premakeKind())
        , "language      " .. string.quote(project:premakeLang())
        , "targetname    " .. string.quote(projectName)
        , "includedirs { " .. proIncludeDirs .. " }"
        , "files { " .. proSrcFiles .. " }"
        , ""
        , "configuration " .. string.quote("debug")
        , "    objdir       " .. string.quote(projecObjDir .. "/debug")
        , "    targetdir    " .. string.quote(projectTargetDir)
        , "    targetsuffix " .. string.quote("-d")
        , ""
        , "configuration " .. string.quote("release")
        , "    objdir       " .. string.quote(projecObjDir .. "/release")
        , "    targetdir    " .. string.quote(projectTargetDir)
    }));
    
    if projectDependson then
        lib.assert(fs.appendLines(projectFile, "dependson { " .. projectDependson .." }")); 
    end

    local inc1 = [[include(os.getenv("GBS_HOME") .. ]] 
        .. [["/premake/filter_action_gmake_install")]];
    
    lib.assert(fs.appendLines(projectFile
        , ""
        , [[filter "action:gmake"]]
        , "    " .. inc1
    ));
   
    return true;
end

return plugin;
