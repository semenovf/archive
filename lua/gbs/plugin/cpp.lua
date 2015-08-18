require "pfs.string";
require "gbs.utils";

local fs  = require("pfs.sys.fs");

local plugin = {};

function plugin:new (settings)
    local o = {
        _settings = settings
    }; 
    self.__index = self;
    return setmetatable(o, self);
end

---
--- @see https://github.com/premake/premake-core/wiki/kind
---
function plugin.premakeKind (projectType)
    if projectType == "console-app" then
        return "ConsoleApp";
    elseif projectType == "gui-app" then
        return "WindowedApp";
    elseif projectType == "shared-lib" then
        return "SharedLib";
    elseif projectType == "static-lib" then
        return "StaticLib";
    elseif projectType == "test" then
        return "ConsoleApp";
    end
    return nil;
end


---
--- @see https://github.com/premake/premake-core/wiki/language
--- 
function plugin.premakeLang (projectLang)
    if projectLang == "C++" then
        return "C++";
    elseif projectLang == "C" then
        return "C";
    end
    return nil;
end


function plugin:transaction ()
    local settings = self._settings;
    
    local verbose = settings:get("Verbose") or false;
    local cmdlineString   = settings:get_or_throw("CommandLineString");
    local programName     = settings:get_or_throw("ProgramName");
    local gbsHomeDir      = settings:get_or_throw("GbsHomeDir");
    local solutionName    = settings:get_or_throw("SolutionName");
    local projectName     = settings:get_or_throw("ProjectName");
    local projectFileName = settings:get_or_throw("ProjectFileName");
    local projectLang     = settings:get_or_throw("ProjectLanguage");
    local sourcesDirName  = settings:get_or_throw("SourcesDirName");
    local testsDirName    = settings:get_or_throw("TestsDirName");
    local projectType     = settings:get_or_throw("ProjectType");
    local projectDependencies = settings:get_or_throw("ProjectDependencies");

    local projectDir      = fs.join(".gbs", projectName);
    local projectFile     = fs.join(projectDir, projectFileName);
        
    local trn = require("gbs.transaction"):begin(verbose);
    
    local srcDir = sourcesDirName;
    if projectType == "test" then
        srcDir = testsDirName;
    end

    local projectSrcDir = fs.join(srcDir, projectName);
    
    trn:MakeDirIfNotExists(srcDir, "Create solution source directory");
    trn:MakeDirIfNotExists(projectSrcDir, "Create project source directory: " .. projectSrcDir);
    
    if projectType == "shared-lib" or projectType == "static-lib" then
        trn:MakeDirIfNotExists("include", "Create solution include directory");
    end
   
    local templateDir = fs.join(gbsHomeDir, "template");
    
    if projectType == "test" then
        if projectLang == "C++" then
            trn:CopyFileIfNotExists(
                      fs.join(templateDir, "cpp", projectType .. ".cpp")
                    , fs.join(projectSrcDir, "test.cpp"));
        elseif projectLang == "C" then
            trn:CopyFileIfNotExists(
                      fs.join(templateDir, "c", projectType .. ".c")
                    , fs.join(projectSrcDir, "test.c"));
        end
    else
        if projectLang == "C++" then
            trn:CopyFileIfNotExists(
                      fs.join(templateDir, "cpp", projectType .. ".cpp")
                    , fs.join(projectSrcDir, "main.cpp"));
        elseif projectLang == "C" then
            trn:CopyFileIfNotExists(
                      fs.join(templateDir, "c", projectType .. ".c")
                    , fs.join(projectSrcDir, "main.c"));
        end
    end

    local projectSrcFileList = {};
    local projectIncludeDirList = {};

    if projectType == "test" then
        table.insert(projectSrcFileList
            , string.quote("../../" .. testsDirName .. "/" .. projectName .. "/**.c"));
        
        if projectLang == "C++" then
            table.insert(projectSrcFileList
                , string.quote("../../" .. testsDirName .. "/" .. projectName .. "/**.cpp"));
        end
    else
        table.insert(projectSrcFileList
            , string.quote("../../" .. sourcesDirName .. "/" .. projectName .. "/**.c"));
            
        if projectLang == "C++" then
            table.insert(projectSrcFileList
                , string.quote("../../" .. sourcesDirName .. "/" .. projectName .. "/**.cpp"));
        end
    end
    
    if projectType == "shared-lib" or projectType == "static-lib" then
        table.insert(projectIncludeDirList, string.quote("../../include"))
        
        if projectLang == "C++" then
            table.insert(projectSrcFileList, string.quote("../../include/**.hpp"));
            table.insert(projectSrcFileList, string.quote("../../include/**.h"));
        elseif projectLang == "C" then
            table.insert(projectSrcFileList, string.quote("../../include/**.h"));
        end
    end

    local projecObjDir     = string.join("/", "../../../.build", solutionName, projectName);
    local projectTargetDir = string.join("/", "../../../.build");
    local targetName       = projectName;
    
    if projectType == "test" then
       projectTargetDir = projectTargetDir .. "/" .. "tests";
--       targetName       = "test-" .. targetName;
    end
    
    trn:AppendLinesToFile(projectFile
        , {
               utils.fileTitle("--", programName, cmdlineString) 
            , "kind          " .. string.quote(plugin.premakeKind(projectType))
            , "language      " .. string.quote(plugin.premakeLang(projectLang))
            , "targetname    " .. string.quote(targetName)
            , "includedirs   { " .. string.join(", ", projectIncludeDirList) .. " }"
            , "files { " .. string.join(", ", projectSrcFileList) .. " }"
            , ""
            , "configuration " .. string.quote("debug")
            , "    objdir       " .. string.quote(projecObjDir .. "/debug")
            , "    targetdir    " .. string.quote(projectTargetDir)
            , "    targetsuffix " .. string.quote("-d")
            , ""
            , "configuration " .. string.quote("release")
            , "    objdir       " .. string.quote(projecObjDir .. "/release")
            , "    targetdir    " .. string.quote(projectTargetDir)
        }, "Update project configuration file: " .. projectFile);  
    

    if #projectDependencies > 0 then
        local deplist = string.quote(projectDependencies);
        trn:AppendLinesToFile(projectFile, {"dependson { " .. string.join(", ", deplist) .." }"});
    end
    
    local inc1 = [[include(os.getenv("GBS_HOME") .. ]] 
        .. [["/premake/filter_action_gmake_install")]];

    trn:AppendLinesToFile(projectFile
        , {
              ""
            , [[filter "action:gmake"]]
            , "    " .. inc1
        });
   
    return trn;
end

return plugin;
