local Project = {};

local Lib  = require("lib");
local Path = require("path");
local File = require("file");

function Project:new (gbs)
    local solutionName = nil;
    local solutionFilepath = Path.join(".gbs", gbs:solutionFileName());
    
    if Path.exists(solutionFilepath) then
        solutionName = gbs:getSolutionNameFromFile(solutionFilepath)
            or Lib.throw(solutionFilepath .. ": unable to take solution name, may be solution file is absent or corrupted");
    end
    
    local o = {
          name = function ()
                local r = gbs:optarg("name") or Lib.throw("project name must be specified");
                Lib.assert(Lib.is_valid_name(r), "bad project name");
                return r; 
          end
        , homeDir           = function () return gbs:homeDir(); end
        , action = function () return gbs:action() or Lib.throw("action must be specified"); end
        , solutionName = function () return solutionName; end
        , type = function ()
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
          
        , language = function () 
            if not gbs:hasOpt("lang") then return "C++"; end
            local l = gbs:optarg("lang");
            if     l == "C++"
                or l == "C" then
                return l;
            end
            Lib.throw("bad project language");
            return nil; 
          end
    }; 
    
    self.__index = self;
    return setmetatable(o, self);
end

function Project:run ()
    local action = self:action() or Lib.throw();
        
    if action == "create" then
        return self:create();
    else
        Lib.print_error(action .. ": bad action");
        return false;
    end
    
    return true;
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

function Project:isExists(name)
    local solutionFile = Path.join(".gbs", "solution.gbs");
    for line in io.lines(solutionFile) do
        r = line:match('^project%s"([^%s]-)"');
        if r == name then
            return true; 
        end
    end
    return false;
end

function Project:create ()
    local proName = self:name();
    local proLanguage = self:language();
    local solutionFile = Path.join(".gbs", "solution.gbs");
    
    if self:isExists(proName) then
        Lib.print_error(proName .. ': project already exists');
        return false;
    end
    
    Lib.assert(File.appendLines(solutionFile, {
          ''
        , '-- BEGIN PROJECT'
        , 'project ' .. Lib.quote(proName)
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
