require "pfs.string";
require "gbs.utils";

local fs  = require("pfs.sys.fs");

local plugin = {};

function plugin:new (project)
    local o = {_project = project}; 
    self.__index = self;
    return setmetatable(o, self);
end

function plugin:transactionsForCreate ()
    local project = self._project or throw_undefined();
    local settings = project._settings or throw_undefined();

    local cmdlineString   = settings:get_or_throw("CommandLineString");
    local programNam      = settings:get_or_throw("ProgramName");
    local gbsHomeDir      = settings:get_or_throw("GbsHomeDir");
    local projectName     = settings:get_or_throw("ProjectName");
    local projectFileName = settings:get_or_throw("ProjectFileName");
    local projectLang     = settings:get_or_throw("ProjectLanguage");
    local projectDir      = fs.join(".gbs", projectName);
    local projectFile     = fs.join(projectDir, projectFileName);
    local sourcesDirName  = settings:get_or_throw("SourcesDirName");
    local testsDirName    = settings:get_or_throw("TestsDirName");
    local projectType     = settings:get_or_throw("ProjectType");
    
    local trn = require("gbs.transaction"):begin();
    
    local srcDir = sourcesDirName;
    if projectType == "test" then
        srcDir = testsDirName;
    end

    local projectSrcDir = fs.join(srcDir, projectName);
    
    trn:append(MakeDirIfNotExists, {srcDir}, "Create solution source directory");
    trn:append(MakeDirIfNotExists, {projectSrcDir}, "Create project source directory: " .. projectSrcDir);
    
    if projectType == "shared-lib" or projectType == "static-lib" then
        trn:append(MakeDirIfNotExists, {"include"}, "Create solution include directory");
    end
   
    local templateDir = fs.join(gbsHomeDir, "template");
    
    if projectType == "test" then
        if projectLang == "C++" then
            trn:append(CopyFileIfNotExists, {
                      fs.join(templateDir, "cpp", projectType .. ".cpp")
                    , fs.join(projectSrcDir, "test.cpp")}, "");
        elseif projectLang == "C" then
            trn:append(CopyFileIfNotExists, {
                      fs.join(templateDir, "c", projectType .. ".c")
                    , fs.join(projectSrcDir, "test.c")}, "");
        end
    else
        if projectLang == "C++" then
            trn:append(CopyFileIfNotExists, {
                      fs.join(templateDir, "cpp", projectType .. ".cpp")
                    , fs.join(projectSrcDir, "main.cpp")}, "");
        elseif projectLang == "C" then
            trn:append(CopyFileIfNotExists, {
                      fs.join(templateDir, "c", projectType .. ".c")
                    , fs.join(projectSrcDir, "main.c")}, "");
        end
    end


--    local projectDependencies = nil;
--    if gbs:hasOpt("depends") then
--        local t = string.quote(gbs:optarg("depends"));
--        projectDependencies = string.join(", ", t);
--    end
--
--    local solutionName = gbs:solutionName();
--    local projecObjDir    = string.join("/", "../../../.build", solutionName, projectName);
--    
--    local projectTargetDir   = string.join("/", "../../../.build");
--    if projectType == "test" then
--        projectTargetDir = string.join("/", "../../../.build", "tests");
--    end
    

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
    
    trn:append(AppendLinesToFile, {    
          projectFile
        , {
               utils.fileTitle(programName, cmdlineString) 
            , "kind          " .. string.quote(project:premakeKind())
            , "language      " .. string.quote(project:premakeLang())
            , "targetname    " .. string.quote(projectName)
            , "includedirs { " .. string.join(", ", projectIncludeDirList) .. " }"
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
        }}, "Update project configuration file: " .. projectFile);  
    
    
    if projectDependson then
        trn:append(AppendLinesToFile, {
            projectFile, {"dependson { " .. projectDependson .." }"}});
    end

    local inc1 = [[include(os.getenv("GBS_HOME") .. ]] 
        .. [["/premake/filter_action_gmake_install")]];

    trn:append(AppendLinesToFile, {
        projectFile, {
              ""
            , [[filter "action:gmake"]]
            , "    " .. inc1}});
   
    return trn;
end

return plugin;
