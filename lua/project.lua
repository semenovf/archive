local Project = {};

local Lib  = require("lib");
local Path = require("path");
local File = require("file");

function Project:new (gbs)
    local o = {
        gbs = function () return gbs; end
    }; 
    
    self.__index = self;
    return setmetatable(o, self);
end

function Project:run ()
    local gbs = self:gbs();
    local action = gbs:action() or Lib.throw("action for project must be specified");
        
    if action == "create" then
        return self:create();
    else
        Lib.print_error(action .. ": bad action");
        return false;
    end
    
    return true;
end

function Project:name ()
    local gbs = self:gbs();
    local r = gbs:optarg("name") or Lib.throw("project name must be specified");
    Lib.assert(Lib.is_valid_name(r), "bad project name");
    return r; 
end

function Project:type ()
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
    Lib.throw("bad project type");
    return nil; 
end

          
function Project:language () 
    local gbs = self:gbs();
    
    if not gbs:hasOpt("lang") then return "C++"; end
    
    local l = gbs:optarg("lang");
    if     l == "C++"
        or l == "C" then
        return l;
    end
    Lib.throw("bad project language");
    return nil; 
end

---
--- @see https://github.com/premake/premake-core/wiki/kind
---
function Project:premakeKind ()
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
function Project:premakeLang ()
    local lang = self:language();
    
    if lang == "C++" then
        return "C++";
    elseif lang == "C" then
        return "C";
    end
    return nil;
end

function Project:exists (name)
    local gbs = self:gbs();
    local solutionFile = Path.join(".gbs", gbs:solutionFileName());
    
    for line in io.lines(solutionFile) do
        r = line:match('^project%s"([^%s]-)"');
        if r == name then
            return true; 
        end
    end
    return false;
end

function Project:create ()
    local gbs = self:gbs();
    local proName = self:name();
    local proDir = Path.join(".gbs", proName);
    local proLanguage = self:language();
    local solutionFile = Path.join(".gbs", gbs:solutionFileName());
    
    if not Path.exists(solutionFile) then
        Lib.print_error("can't create project outside of solution directory");
        return false;
    end
    
    
    if self:exists(proName) then
        Lib.print_error(proName .. ': project already registered');
        return false;
    end
    
    if Path.exists(proDir) then
        Lib.print_error(proDir .. ': project directory already exists');
        return false;
    end
    
    if not Path.mkdir(proDir) then
        Lib.print_error(proDir .. ': failed to create project directory');
        return false;
    end
    
    
    Lib.assert(File.appendLines(solutionFile, {
          ''
        , '-- BEGIN PROJECT'
        , 'project ' .. Lib.quote(proName)
        , '    include(' .. Lib.quote(proName .. '/' .. gbs:projectFileName()) ..')' 
    }));
    
    if proLanguage == 'C++' or proLanguage == 'C' then
        local Plugin = require('plugin/cpp');
        local p = Plugin:new(self);
        p:create();
    end

    Lib.assert(File.appendLines(solutionFile, {
        '-- END PROJECT'
    }));    
    return true;
end

return Project;
