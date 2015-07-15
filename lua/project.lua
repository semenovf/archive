local Project = {}

local Lib  = require("lib");
local Path = require("path");
local File = require("file");

function Project:new ()
    local o = {
--          _opts = nil
--        , _solutionName = nil
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

function Project:action ()
    if #self._opts > 1 then
        return self.opts[2];
    end
    Lib.print_error("action must be specified");
    return nil;
end

function Project:name ()
    local r = self._opts[name] or Lib.throw("project name must be specified");
    Lib.assert(Lib.is_valid_name(r), "bad project name");
    return r; 
end

function Project:type () 
    if self._opts[type] == nil then return "console-app"; end
    
    if self._opts[type] == "console-app"
            or self._opts[type] == "gui-app"
            or self._opts[type] == "shared-lib"
            or self._opts[type] == "static-lib"
            or self._opts[type] == "test"
        return self._opts[type];
    end
    Lib.throw("bad project type");
    return nil; 
end

function Project:language () 
    if self._opts[lang] == nil then return "C++"; end
    if self._opts[lang] == "C++"
            or self._opts[lang] == "C"
        return self._opts[lang];
    end
    Lib.throw("bad project language");
    return nil; 
end

function Project:solutionName () 
    return self._solutionName or Lib.throw("unknown solution name"); 
end

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

function Project:premakeLang ()
    local lang = self:language();
    
    if lang == "C++" then
        return "C++";
    elseif lang == "C" then
        return "C";
    end
    return nil;
end

function Project:create ()
    local name = self:name();

    if not Path.exists('src') then
         Lib.assert(Path.mkdir('src'));
    end
    
--    if self:isStaticLib
    local solutionName = self:solutionName();
    local kind     = self:premakeKind();
    local language = self:premakeLang();
    local location = "../../.build/" .. solution_name .. '/' .. name;
        
    Lib.assert(File.appendLines(Path.join(".gbs", "solution.gbs"), {
              ''
            , '-- BEGIN PROJECT'
            , 'project '      .. Lib.quote(name)
            , '    kind     ' .. Lib.quote(kind)
            , '    language ' .. Lib.quote(language)
            , '    location ' .. Lib.quote(location)
            , '-- END PROJECT'
        }));    
    
    return true;
end

return Project;