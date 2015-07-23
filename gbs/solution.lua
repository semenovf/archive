local solution = {};

require("gbs.sys.string");

local lib = require("gbs.sys.lib");
local fs  = require("gbs.sys.fs");
local app = require("gbs.sys.app");

function solution:new (gbs)
    local o = {
        gbs = function () return gbs; end
    }; 
    
    self.__index = self;
    return setmetatable(o, self);
end

function solution:name ()
    local gbs = self:gbs();
    
    local r = gbs:optarg("name");
    if type(r) ~= "string" or r:isEmpty() then 
        lib.throw("project name must be specified");
    end

    lib.assert(lib.is_valid_name(r), "bad project name");
    return r; 
end

function solution:run ()
    local gbs = self:gbs();
    local action = gbs:action() or lib.throw("action for solution must be specified");
        
    return lib.runAction(action, {
          create = function () return self:create(); end
--        , build  = function () return self:build(); end
    });
end

function solution:create ()
    local gbs = self:gbs();
    local wsFile = fs.join(".gbs", gbs:workspaceFileName());
    local slnName = self:name();
--    local slnFileName = fs.join(".gbs", gbs:solutionFileName());

    if not fs.exists(wsFile) then
        lib.print_error("can't create solution outside of workspace directory");
        return false;
    end
    
    if not fs.mkdir(slnName) then
        lib.print_error("failed to create solution");
        return false;
    end

    if not fs.mkdir(fs.join(slnName, ".gbs")) then
        lib.print_error("failed to create solution `.gbs' directory");
        return false;
    end
    
    if gbs:hasOpt("git") then
        if not fs.exists(".gitignore") then
            local src = fs.join(gbs:homeDir(), "scripts", "gitignore");
            local dest = ".gitignore";
            lib.assert(fs.copy(src, dest));
        end
    end
    
    lib.assert(fs.appendLines(wsFile, { slnName }));
    lib.assert(fs.appendLines(fs.join(slnName, ".gbs", gbs:solutionFileName()), {
          "--************************************************************"
        , "--* Generated automatically by `" .. app.name() .. "'"
        , "--* Command: `" .. app.cmdline() .. "'"
        , "--* Date:    " .. os.date() 
        , "--************************************************************"
        , 'solution ' .. string.quote(slnName)
        , '    configurations {"debug", "release"}'
        , '    platforms {"unix32", "unix64", "win32", "win64"}'
        , '    filter "platforms:*32"'
        , '        architecture "x32"'
        , '    filter "platforms:*64"'
        , '        architecture "x64"'
    }));  
    
    return true;
end

return solution;