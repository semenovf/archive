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
    local enableQt        = settings:get("EnableQt") or false;

    local premakeKind = plugin.premakeKind(projectType);
    local premakeLang = plugin.premakeLang(projectLang);
    die(projectType .. ": invalid project type\n"):unless(premakeKind);
    die(projectType .. ": invalid project language\n"):unless(premakeLang);

    local projectDir      = fs.join(".gbs", projectName);
    local projectFile     = fs.join(projectDir, projectFileName);

    local trn = require("gbs.transaction"):begin(verbose);

    local srcDir = sourcesDirName;
    if projectType == "test" then
        srcDir = testsDirName;
    end

    local projectSrcDir = fs.join(srcDir, projectName);
    local templateDir = fs.join(gbsHomeDir, "template");

    trn:MakeDirIfNotExists(srcDir, "Create solution source directory");
    trn:MakeDirIfNotExists(projectSrcDir, "Create project source directory: " .. projectSrcDir);

    if projectType == "shared-lib" or projectType == "static-lib" then
        trn:MakeDirIfNotExists("include", "Create solution include directory");
    end

    if enableQt then
        trn:CopyFileIfNotExists(
              fs.join(templateDir, "qt", "qt_enable.lua")
            , fs.join("..", ".gbs", "qt_enable.lua"));
        trn:Print("ATTENTION: Check and modify qt_enable.lua on demand in workspace directory");
    end

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
            , string.quote("../../" .. testsDirName .. "/" .. projectName .. "/*.c"));

        if projectLang == "C++" then
            table.insert(projectSrcFileList
                , string.quote("../../" .. testsDirName .. "/" .. projectName .. "/*.cpp"));
        end
    else
        table.insert(projectSrcFileList
            , string.quote("../../" .. sourcesDirName .. "/" .. projectName .. "/*.c"));

        if projectLang == "C++" then
            table.insert(projectSrcFileList
                , string.quote("../../" .. sourcesDirName .. "/" .. projectName .. "/*.cpp"));
        end
    end

    table.insert(projectIncludeDirList, string.quote("../../include"))
    
    if projectType == "shared-lib" or projectType == "static-lib" then
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
    local projectLibDirs   = "../../../.build";

    if projectType == "test" then
       projectTargetDir = projectTargetDir .. "/" .. "tests";
--       targetName       = "test-" .. targetName;
    end

    local projectContent = require("pfs.vector"):new();
    projectContent:push_back(utils.fileTitle("--", programName, cmdlineString));


    projectContent:push_back("kind          " .. string.quote(premakeKind));
    projectContent:push_back("language      " .. string.quote(premakeLang));
    projectContent:push_back("targetname    " .. string.quote(targetName));
    projectContent:push_back("defines       {  }");
    projectContent:push_back("includedirs   { " .. string.join(", ", projectIncludeDirList) .. " }");
    projectContent:push_back("libdirs       { " .. string.quote(projectLibDirs) .. " }");
    projectContent:push_back("files         { " .. string.join(", ", projectSrcFileList) .. " }");
    projectContent:push_back("");

    if enableQt then
        projectContent:push_back("-- Special for Qt projects");
        projectContent:push_back("require(\"../../../.gbs/qt_enable\")");
        projectContent:push_back("");
        projectContent:push_back("--Available modules:");
        projectContent:push_back("--    core, gui, multimedia, network, opengl, positioning, printsupport,");
        projectContent:push_back("--    qml, quick, sensors, sql, svg, testlib, websockets, widgets, xml");
        projectContent:push_back("--");
        projectContent:push_back("qtmodules     { \"core\", \"gui\" }");
        projectContent:push_back("");
    end

    projectContent:push_back("configuration " .. string.quote("debug"));
    projectContent:push_back("    flags        { \"FatalWarnings\", \"Symbols\" }");
    projectContent:push_back("    defines      { \"DEBUG\" }");
    projectContent:push_back("    objdir       " .. string.quote(projecObjDir .. "/debug"));

    if enableQt then
        projectContent:push_back("    qtgenerateddir "  .. string.quote(projecObjDir .. "/debug"));
    end

    projectContent:push_back("    targetdir    " .. string.quote(projectTargetDir));
    projectContent:push_back("    targetsuffix " .. string.quote("-d"));
    projectContent:push_back("    links        {  }");
    projectContent:push_back("");
    projectContent:push_back("configuration " .. string.quote("release"));
    projectContent:push_back("    flags        { \"FatalWarnings\" }");
    projectContent:push_back("    defines      { \"NDEBUG\" }");
    projectContent:push_back("    objdir       " .. string.quote(projecObjDir .. "/release"));

    if enableQt then
        projectContent:push_back("    qtgenerateddir "  .. string.quote(projecObjDir .. "/release"));
    end

    projectContent:push_back("    targetdir    " .. string.quote(projectTargetDir));
    projectContent:push_back("    links        {  }");
    projectContent:push_back("");

    if #projectDependencies > 0 then
        local deplist = string.quote(projectDependencies);
        projectContent:push_back("dependson { " .. string.join(", ", deplist) .." }");
        projectContent:push_back("");
    end

    projectContent:push_back("");
    projectContent:push_back([[filter "action:gmake"]]);
    projectContent:push_back("    buildoptions {}");
    projectContent:push_back("    linkoptions  {}");
    projectContent:push_back("");
    projectContent:push_back([[include(os.getenv("GBS_HOME") .. ]] .. [["/premake/filter_action_gmake_install")]]);

    --
    -- -rdynamic option (gcc) usefull for backtrace
    -- 
    -- Unix: (backtrace)[http://www.kernel.org/doc/man-pages/online/pages/man3/backtrace.3.html]
    -- Mac: (backtrace)[http://developer.apple.com/library/mac/#documentation/Darwin/Reference/ManPages/man3/backtrace.3.html]
    -- Windows: (CaptureBackTrace)[http://msdn.microsoft.com/en-us/library/windows/desktop/bb204633%28v=vs.85%29.aspx]
    --
    projectContent:push_back("");
    projectContent:push_back("filter { \"debug\", \"action:gmake\" }");
    projectContent:push_back("    linkoptions  { \"-rdynamic\" }"); 

    if enableQt then
        projectContent:push_back("");
        projectContent:push_back("-- Special for Qt projects");
        projectContent:push_back("package.loaded[\"../../../.gbs/qt_enable\"] = nil");
    end

    projectContent:push_back("");

    trn:AppendLinesToFile(projectFile
        , projectContent:data()
        , "Update project configuration file: " .. projectFile);  

    return trn;
end

return plugin;
