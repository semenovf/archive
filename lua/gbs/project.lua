require "pfs.string";
require "pfs.die";

local fs  = require("pfs.sys.fs");

local project = {};

function project:new (settings)
    local o = {
        _settings = settings
    }; 
    self.__index = self;
    return setmetatable(o, self);
end

function project:createPlugin ()
    local settings = self._settings;
    local projectLang = settings:get_or_throw("ProjectLanguage");
    
    if projectLang == 'C++' then
        return require("gbs.plugin.cpp"):new(settings);
    elseif self.projectLang == 'C' then
        return require("gbs.plugin.cpp"):new(settings);
    end
    return require("gbs.plugin.dummy"):new(settings);
end

function project:buildPlugin ()
    local settings = self._settings;
    local buildTool = settings:get_or_throw("BuildTool");

    if buildTool == "gmake" then
        return require("gbs.plugin.gmake"):new("build", settings);
    elseif buildTool:match("^vs*") then
        return require("gbs.plugin.msbuild"):new("build", settings);
    end
    return require("gbs.plugin.dummy"):new(settings);
end

function project:cleanPlugin ()
    local settings = self._settings;
    local buildTool = settings:get_or_throw("BuildTool");

    if buildTool == "gmake" then
        return require("gbs.plugin.gmake"):new("clean", settings);
    elseif buildTool:match("^vs*") then
        return require("gbs.plugin.msbuild"):new("clean", settings);
    end
    return require("gbs.plugin.dummy"):new(settings);
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
    local settings = self._settings;
    
    local verbose          = settings:get("Verbose") or false;
    local projectFileName  = settings:get_or_throw("ProjectFileName");
    local projectName      = settings:get_or_throw("ProjectName");
    local projectType      = settings:get_or_throw("ProjectType");
    local projectLang      = settings:get_or_throw("ProjectLanguage");
    local solutionFileName = settings:get_or_throw("SolutionFileName");

    local projectDir   = fs.join(".gbs", projectName);
    local solutionFile = fs.join(".gbs", solutionFileName);
    
    local trn = require("gbs.transaction"):begin(verbose);
    
    trn:PathExists(solutionFile, "Check if creating project is inside of solution directory");
    trn:PathNotExists(projectName, "Project directory already exists: " .. projectDir); 
    trn:Function(function () return project.registered(solutionFile, projectName); end
        , "Project already registered: " .. projectName);
    trn:MakeDir(projectDir, "Create project directory: " .. projectDir);
    
    trn:Transaction(self:createPlugin():transaction());
    trn:AppendLinesToFile(solutionFile
        , {
              ""
            , "-- BEGIN PROJECT"
            , "project " .. string.quote(projectName)
            , "    include(" .. string.quote(projectName .. "/" .. projectFileName) .. ")"
            , "-- END PROJECT"}
        , "Update solution configration file: " .. solutionFile);

    return trn:exec();
end

function project:build ()
    local settings = self._settings;

    local verbose          = settings:get("Verbose") or false;
    local solutionFileName = settings:get_or_throw("SolutionFileName");

    local solutionFile = fs.join(".gbs", solutionFileName);

    local trn = require("gbs.transaction"):begin(verbose);

    trn:PathExists(solutionFile, "Building is inside of solution directory");
    trn:Transaction(self:buildPlugin():transaction());

    return trn:exec();
end

function project:clean ()
    local settings = self._settings;

    local verbose          = settings:get("Verbose") or false;
    local solutionFileName = settings:get_or_throw("SolutionFileName");

    local solutionFile = fs.join(".gbs", solutionFileName);

    local trn = require("gbs.transaction"):begin(verbose);

    trn:PathExists(solutionFile, "Cleaning is inside of solution directory");
    trn:Transaction(self:cleanPlugin():transaction());

    return trn:exec();
end

return project;
