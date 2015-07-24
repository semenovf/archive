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

function project:run ()
    local gbs = self:gbs();
    local action = gbs:action() or lib.throw("action for project must be specified");
    
    return lib.runAction(action, {
          create = function () return self:create(); end
        , build  = function () return self:build(); end
        , clean  = function () return self:clean(); end
    });
end

function project:name ()
    local gbs = self:gbs();
    local r = gbs:optarg("name") or lib.throw("project name must be specified");
    lib.assert(lib.is_valid_name(r), "bad project name");
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
    lib.throw("bad project type");
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
    lib.throw("bad project language");
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

function project:buildOptions ()
    local gbs = self:gbs();
    local os_info   = nil;
    local config   = gbs:optarg("config") or "debug";
    local platform = gbs:optarg("platform") or nil;
    local target   = gbs:optarg("target") or nil;

    if platform == nil then
        os_info = os_info or os.info();
        platform = os_info.type .. os_info.bits;
    end

    if target == nil then
        os_info = os_info or os.info();
        if os_info.os == "linux" then
            target = "gmake";
        end
    end

    if platform ~= "unix32" 
            and platform ~= "unix64" 
            and platform ~= "mswin32"
            and platform ~= "mswin64" then
        platform = nil;
    end
    
    if config ~= "debug" and config ~= "release" then
        config = nil;
    end
    
    return config, platform, target;
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

function project:exists (name)
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
    local proName = self:name();
    local proDir = fs.join(".gbs", proName);
    local proLanguage = self:language();
    local solutionFile = fs.join(".gbs", gbs:solutionFileName());
    
    if not fs.exists(solutionFile) then
        lib.print_error("can't create project outside of solution directory");
        return false;
    end
    
    if self:exists(proName) then
        lib.print_error(proName .. ': project already registered');
        return false;
    end
    
    if fs.exists(proDir) then
        lib.print_error(proDir .. ': project directory already exists');
        return false;
    end
    
    if not fs.mkdir(proDir) then
        lib.print_error(proDir .. ': failed to create project directory');
        return false;
    end
    
    lib.assert(fs.appendLines(solutionFile, {
          ''
        , '-- BEGIN PROJECT'
        , 'project ' .. string.quote(proName)
        , '    include(' .. string.quote(proName .. '/' .. gbs:projectFileName()) ..')' 
    }));
    
    if proLanguage == 'C++' or proLanguage == 'C' then
        local p = require('gbs.plugin.cpp'):new(self);
        p:create();
    end

    lib.assert(fs.appendLines(solutionFile, {
        '-- END PROJECT'
    }));    
    return true;
end

function project:build ()
    local gbs = self:gbs();
    local proName = self:name();
    local config, platform, target = self:buildOptions();
    
    if config == nil then
        lib.throw("bad or unknown config");
    end

    if platform == nil then
        lib.throw("bad or unknown platform, supported 'unix{32|64}' and 'mswin{32|64}' only");
    end

    if target == nil then
        lib.throw("bad or unknown target, target value must be equals to one of premake's actions");
    end

    if target == "gmake" then
        config = config .. "_" .. "unix64";
        fs.executePremake("--file=.gbs/solution.gbs", target);
        fs.execute("make", "-C", ".gbs", "config=" .. config, proName);
    end
end

function project:clean ()
    local gbs = self:gbs();
    local proName = self:name();

    if fs.exists(fs.join(".gbs", proName .. ".make")) then
        fs.execute("make", "-C", ".gbs", "-f", proName .. ".make", "clean");--, "config=debug_unix32", proName);
--        fs.execute("make", "-C", ".gbs", "clean", "config=debug_unix64", proName);
--        fs.execute("make", "-C", ".gbs", "clean", "config=debug_mswin32", proName);
--        fs.execute("make", "-C", ".gbs", "clean", "config=debug_mswin64", proName);
    end
    fs.executePremake("clean", proName);

--    if target == "gmake" then
--        config = config .. "_" .. "unix64";
--        fs.executePremake("--file=.gbs/solution.gbs", target);
--        fs.execute("make", "-C", ".gbs", "config=" .. config, proName);
--    end
end

return project;
