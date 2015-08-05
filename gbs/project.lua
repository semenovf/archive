local project = {};

require("gbs.sys.string");
require("gbs.sys.os");

local lib = require("gbs.sys.lib");
local fs  = require("gbs.sys.fs");

function project:new (gbs)
    local o = {
        gbs = function () return gbs; end
    }; 
    
    self.__index = self;
    return setmetatable(o, self);
end

function project.usage ()
    print("NAME");
    print("    gbs { project | pro | prj } - project manipulation");
    print("");
    print("SYNOPSYS");
    print("    (1) gbs project --create");
    print("        --name=NAME [--type=PROJECT_TYPE]");
    print("        [--lang=LANG] [--depends=NAME [--depends=NAME ...]]");
    print("    (2) gbs project --build");
    print("        [--name=NAME]");
    print("        [--config={debug | release}]");
    print("        [--build-tool=BUILD_TOOL]");
    print("        [--target-platform=TARGET_PLATFORM]");
    print("");
    print("DESCRIPTION");
    print("    (1) - create project with name NAME");
    print("    (2) - build project with NAME");
    print("");
    print("OPTIONS");
    print("    --depends");
    print("        specify one more sibling project names (inside solution)");
    print("");
    print("VALUES");
    print("    NAME");
    print("        Valid only alphanumeric characters, underscore ('_') and dash ('-')");
    print("    PROJECT_TYPE");
    print("        `console-app' | `gui-app' | `shared-lib' | `static-lib' | `test'");
    print("        Default is `console-app'");
    print("    LANG");
    print("        Language identifier: `C++' | `C'. Default is C++");
    print("    BUILD_TOOL");
    print("        `gmake' | `vs2005' | `vs2008' | `vs2010' | `vs2012' | `vs2013' | `vs2015'");
    print("    TARGET_PLATFORM"); 
    print("        `unix32' | `unix64' | `mswin32' | `mswin64'");
end

function project:run ()
    local gbs = self:gbs();
    
    if gbs:hasOpt("create") then
        return self:create();
    elseif gbs:hasOpt("build") then
        return self:build();
    elseif gbs:hasOpt("clean") then
        return self:clean();
    else
        lib.print_error("action for project must be specified");
    end
    return false;
end

function project:name ()
    local gbs = self:gbs();
    r = gbs:optarg("name");
    
    if r == nil then return nil; end
    if not lib.is_valid_name(r) then return nil; end
    return r; 
end

function project:type ()
    local gbs = self:gbs();
    if not gbs:hasOpt("type") then return "console-app"; end
    local t = gbs:optarg("type");
    
    if     t == "console-app"
        or t == "gui-app"
        or t == "shared-lib"
        or t == "static-lib"
        or t == "test" then
        return t;
    end
--    lib.throw("bad project type");
    return nil; 
end

          
function project:language () 
    local gbs = self:gbs();
    
    if not gbs:hasOpt("lang") then return "C++"; end
    
    local l = gbs:optarg("lang");
    if     l == "C++"
        or l == "C" then
        return l;
    end
    return nil; 
end

---
--- @see https://github.com/premake/premake-core/wiki/kind
---
function project:premakeKind ()
    local kind = self:type();
    
    if kind == "console-app" then
        return "ConsoleApp";
    elseif kind == "gui-app" then
        return "WindowedApp";
    elseif kind == "shared-lib" then
        return "SharedLib";
    elseif kind == "static-lib" then
        return "StaticLib";
    elseif kind == "test" then
        return "ConsoleApp";
    end
    return nil;
end

---
--- @see https://github.com/premake/premake-core/wiki/language
--- 
function project:premakeLang ()
    local lang = self:language();
    
    if lang == "C++" then
        return "C++";
    elseif lang == "C" then
        return "C";
    end
    return nil;
end

function project:registered (name)
    local gbs = self:gbs();
    local solutionFile = fs.join(".gbs", gbs:solutionFileName());
    
    for line in io.lines(solutionFile) do
        r = line:match('^project%s"([^%s]-)"');
        if r == name then
            return true; 
        end
    end
    return false;
end

function project:create ()
    local gbs = self:gbs();
    local projectName = self:name() or lib.die("`name' is bad or must be specified");
    local projectLanguage = self:language() or lib.die("`language' is bad or must be specified");
    local projectDir  = fs.join(".gbs", projectName);
    local solutionFile = fs.join(".gbs", gbs:solutionFileName());
    
    if not fs.exists(solutionFile) then
        lib.die("can't create project outside of solution directory");
    end
    
    if self:registered(projectName) then
        lib.die(projectName .. ': project already registered');
    end
    
    if fs.exists(projectDir) then
        lib.die(projectDir .. ': project directory already exists');
    end
    
    if not fs.mkdir(projectDir) then
        lib.die(projectDir .. ': failed to create project directory');
    end
    
    lib.assert(fs.appendLines(solutionFile
        , ''
        , '-- BEGIN PROJECT'
        , 'project ' .. string.quote(projectName)
        , '    include(' .. string.quote(projectName .. '/' .. gbs:projectFileName()) ..')' 
    ));
    
    if projectLanguage == 'C++' or projectLanguage == 'C' then
        local p = require('gbs.plugin.cpp'):new(self);
        p:create();
    end

    lib.assert(fs.appendLines(solutionFile, '-- END PROJECT'));    
    return true;
end

--
-- Valid tragets: all, clean, <project-name>
--
function project:_gmake (gmakeTarget)
    local gbs = self:gbs();
    local projectName = self:name() or "";
    local solutionFile = fs.join(".gbs", gbs:solutionFileName());
    
    if not fs.exists(solutionFile) then
        lib.die("can't build project outside of solution directory");
    end
        
    local targetPlatform = gbs:optarg("target-platform") or lib.die("unspecified target platform");
    local config = gbs:optarg("config") or "debug";
    
    config = config .. "_" .. targetPlatform;
    fs.executePremake("--file=" .. solutionFile, "gmake");
    
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
function project:_msbuild ()
end


function project:build ()
    local gbs = self:gbs();
    local projectName = self:name() or "";
    local buildTool = gbs:optarg("build-tool") or lib.die("unspecified build tool");

    if buildTool == "gmake" then
        self:_gmake("all")
    elseif buildTool:match("^vs*") then
        self:_msbuild(); -- FIXME
--        fs.executePremake("--file=" .. solutionFile, buildTool);
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
        self:_gmake("clean")
    elseif buildTool:match("^vs*") then
        self:_msbuild(); -- FIXME
    else
        lib.die(buildTool .. ": unsupported build tool")
    end
    --fs.executePremake("clean", projectName);
end

return project;
