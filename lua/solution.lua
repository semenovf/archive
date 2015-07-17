local Solution = {};

local Lib  = require("lib");
local Path = require("path");
local File = require("file");
local App  = require("app");

function Solution:new (gbs)
    local o = {
        gbs = function () return gbs; end
    }; 
    
    self.__index = self;
    return setmetatable(o, self);
end

function Solution:name ()
    local gbs = self:gbs();
    local r = gbs:optarg("name") or Lib.throw("project name must be specified");
    Lib.assert(Lib.is_valid_name(r), "bad project name");
    return r; 
end

function Solution:run ()
    local gbs = self:gbs();
    local action = gbs:action() or Lib.throw("action for solution must be specified");
        
    if action == "create" then
        return self:create();
    else
        Lib.print_error(action .. ": bad action");
        return false;
    end
end

function Solution:create ()
    local gbs = self:gbs();
    local wsFile = Path.join(".gbs", gbs:workspaceFileName());
    local slnName = self:name();
--    local slnFileName = Path.join(".gbs", gbs:solutionFileName());

    if not Path.exists(wsFile) then
        Lib.print_error("can't create solution outside of workspace directory");
        return false;
    end
    
    if not Path.mkdir(slnName) then
        Lib.print_error("failed to create solution");
        return false;
    end

    if not Path.mkdir(Path.join(slnName, ".gbs")) then
        Lib.print_error("failed to create solution `.gbs' directory");
        return false;
    end
    
    Lib.assert(File.appendLines(wsFile, { slnName }));
    Lib.assert(File.appendLines(Path.join(slnName, ".gbs", gbs:solutionFileName()), {
              "--************************************************************"
            , "--* Generated automatically by `" .. App.name() .. "'"
            , "--* Command: `" .. App.cmdline() .. "'"
            , "--* Date:    " .. os.date() 
            , "--************************************************************"
            , 'solution ' .. Lib.quote(slnName)
            , '    configurations {"debug", "release"}'
        }));    
    
    return true;
end

return Solution;