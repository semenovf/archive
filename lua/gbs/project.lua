require "pfs.string";

local fs  = require("pfs.sys.fs");

local project = {};

function project:new (settings)
    local o = {_settings = settings}; 
    self.__index = self;
    return setmetatable(o, self);
end

--function project.usage ()
--    print("NAME");
--    print("    gbs { project | pro | prj } - project manipulation");
--    print("");
--    print("SYNOPSYS");
--    print("    (1) gbs project --create");
--    print("            --name=NAME [--type=PROJECT_TYPE]");
--    print("            [--lang=LANG] [--depends=NAME [--depends=NAME ...]]");
--    print("    (2) gbs project --build");
--    print("            [--name=NAME]");
--    print("            [--config={debug | release}]");
--    print("            [--build-tool=BUILD_TOOL]");
--    print("            [--target-platform=TARGET_PLATFORM]");
--    print("");
--    print("DESCRIPTION");
--    print("    (1) - create project with name NAME");
--    print("    (2) - build project or solution en masse");
--    print("");
--    print("OPTIONS");
--    print("    --depends");
--    print("        specify one more sibling project names (inside solution)");
--    print("");
--    print("VALUES");
--    print("    NAME");
--    print("        Valid only alphanumeric characters, underscore ('_') and dash ('-')");
--    print("    PROJECT_TYPE");
--    print("        `console-app' | `gui-app' | `shared-lib' | `static-lib' | `test'");
--    print("        Default is `console-app'");
--    print("    LANG");
--    print("        Language identifier: `C++' | `C'. Default is C++");
--    print("    BUILD_TOOL");
--    print("        `gmake' | `vs2005' | `vs2008' | `vs2010' | `vs2012' | `vs2013' | `vs2015'");
--    print("    TARGET_PLATFORM"); 
--    print("        `unix32' | `unix64' | `mswin32' | `mswin64'");
--end

--function project:run ()
--    local gbs = self:gbs();
--    
--    if gbs:hasOpt("create") then
--        return self:create();
--    elseif gbs:hasOpt("build") then
--        return self:build();
--    elseif gbs:hasOpt("clean") then
--        return self:clean();
--    else
--        lib.print_error("action for project must be specified");
--    end
--    return false;
--end
--
--function project:name ()
--    local gbs = self:gbs();
--    r = gbs:optarg("name");
--    
--    if r == nil then return nil; end
--    if not lib.is_valid_name(r) then return nil; end
--    return r; 
--end

--function project:type ()
--    local gbs = self:gbs();
--    if not gbs:hasOpt("type") then return "console-app"; end
--    local t = gbs:optarg("type");
--    
--    if     t == "console-app"
--        or t == "gui-app"
--        or t == "shared-lib"
--        or t == "static-lib"
--        or t == "test" then
--        return t;
--    end
----    lib.throw("bad project type");
--    return nil; 
--end

---
--- @see https://github.com/premake/premake-core/wiki/kind
---
function project.premakeKind (projectType)
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
function project.premakeLang (projectLang)
    if projectLang == "C++" then
        return "C++";
    elseif projectLang == "C" then
        return "C";
    end
    return nil;
end

function project.registered (solutionFile, projectName)
    for line in io.lines(solutionFile) do
        r = line:match('^project%s"([^%s]-)"');
        if r == name then
            return true; 
        end
    end
    return false;
end

function project:create ()
    local settings         = self._settings;
    
    local projectFileName  = settings:get_or_throw("ProjectFileName");
    local projectName      = settings:get_or_throw("ProjectName");
    local projectType      = settings:get_or_throw("ProjectType");
    local projectLang      = settings:get_or_throw("ProjectLanguage");
    local projectDepends   = settings:get("Dependecies");
    local solutionFileName = settings:get_or_throw("SolutionFileName");
    
    local projectDir   = fs.join(".gbs", projectName);
    local solutionFile = fs.join(".gbs", solutionFileName);
    
    local trn = require("gbs.transaction"):begin();
    
    trn:append(PathExists, {solutionFile}, "Check if creating project is inside of solution directory");
    trn:append(PathNotExists, {projectName}, "Project directory already exists: " .. projectDir); 
    trn:append(function (args) return project.registered(args[1], args[2]); end
        , {solutionFile, projectName}, "Project already registered: " .. projectName);
    trn:append(MakeDir, {projectDir}, "Create project directory: " .. projectDir);
    
    if projectLang == 'C++' or projectLang == 'C' then
        trn:append(Transaction, {require("gbs.plugin.cpp"):new(self):transactionsForCreate()});
        trn:append(AppendLinesToFile, {
              solutionFile
            , {
                  ""
                , "-- BEGIN PROJECT"
                , "project " .. string.quote(projectName)
                , "    include(" .. string.quote(projectName .. "/" .. projectFileName) .. ")"
                , "-- END PROJECT" 
            }}, "Update solution configration file: " .. solutionFile);
    end

    return trn:exec();
end

--
-- solutionName not used.
-- Valid tragets: all, clean, <project-name>
-- Valid config: debug, release
-- targetPlatform
--
function project:_gmake (solutionName, config, targetPlatform, gmakeTarget)
--    local gbs = self:gbs();
    local projectName = self:name() or "";
--    local solutionFile = fs.join(".gbs", gbs:solutionFileName());
--    
--    if not fs.exists(solutionFile) then
--        lib.die("can't build project outside of solution directory");
--    end
        
--    local targetPlatform = gbs:optarg("target-platform") or lib.die("unspecified target platform");
--    local config = gbs:optarg("config") or "debug";
    
    config = config .. "_" .. targetPlatform;
--    fs.executePremake("--file=" .. solutionFile, "gmake");
    
    if string.isEmpty(gmakeTarget) then
        gmakeTarget = "all"
    end
        
    if string.isEmpty(projectName) then
        fs.execute("make", "-C", ".gbs", "config=" .. config, gmakeTarget);
    else
        fs.execute("make", "-f", projectName .. ".make", "-C", ".gbs", "config=" .. config, gmakeTarget);
    end
end

-- FIXME
function project:_msbuild (solutionName, config, targetPlatform, msbuildTarget)
    local projectName = self:name() or "";

    if string.isEmpty(projectName) then
        fs.execute("msbuild"
            , "/t:" .. msbuildTarget
            , "/p:Configuration=" .. config
            , "/p:Platform=" .. targetPlatform
--            , "/v:quiet"
            , fs.join(".gbs", solutionName .. ".sln"));
    else
--        fs.execute("make", "-f", projectName .. ".make", "-C", ".gbs", "config=" .. config, gmakeTarget);
    end

end


function project:build ()
    local gbs = self:gbs();
    local projectName = self:name() or "";
    local buildTool = gbs:optarg("build-tool") or lib.die("unspecified build tool");
    local solutionFile = fs.join(".gbs", gbs:solutionFileName());
    local targetPlatform = gbs:optarg("target-platform") or lib.die("unspecified target platform");
    local config = gbs:optarg("config") or "debug";
    
    if not fs.exists(solutionFile) then
        lib.die("can't build project outside of solution directory");
    end
    
    local solutionName = gbs:solutionName();

    if buildTool == "gmake" then
        fs.executePremake("--file=" .. solutionFile, buildTool);
        self:_gmake(solutionName, config, targetPlatform, "all")
    elseif buildTool:match("^vs*") then
        fs.executePremake("--file=" .. solutionFile, buildTool);
        self:_msbuild(solutionName, config, targetPlatform, "Build");
    else
        lib.die(buildTool .. ": unsupported build tool")
    end
    
    return true;
end

function project:clean ()
    local gbs = self:gbs();
    local projectName = self:name() or "";
    local solutionFile = fs.join(".gbs", gbs:solutionFileName());
    
    if not fs.exists(solutionFile) then
        lib.die("can't build project outside of solution directory");
    end

    local buildTool = gbs:optarg("build-tool") or lib.die("unspecified build tool");    

    if buildTool == "gmake" then
        -- self:_gmake("clean") --FIXME
    elseif buildTool:match("^vs*") then
        --self:_msbuild(solutionName, config, targetPlatform, "Clean");
        self:_msbuild(); -- FIXME
    else
        lib.die(buildTool .. ": unsupported build tool")
    end
    --fs.executePremake("clean", projectName);
end

return project;
