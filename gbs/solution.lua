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

function solution.usage ()
    print("NAME");
    print("    gbs { solution | sln } - solution manipulation");
    print("");
    print("SYNOPSYS");
    print("    (1) gbs solution --create");
    print("        --name=NAME [GIT_OPTIONS]");
    print("");
    print("DESCRIPTION");
    print("    (1) - create solution with name NAME");
    print("");
    print("VALUES");
    print("    NAME");
    print("        Valid only alphanumeric characters, underscore ('_') and dash ('-')");
    print("");
    print("GIT OPTIONS");
    print("    --git");
    print("        initialize only git local repo");
--    print("    --git=github");
--    print("        initialize git local repo and synchronize it with remote repository"); 
--    print("        on GitHub (must be created previously)");
--    print("    --git=bitbucket");
--    print("        initialize git local repo and synchronize it with remote repository");
--    print("        on Bitbucket (must be created previously)");
--    print("    --git-login=LOGIN");
--    print("        login name to access remote git repository");
end

function solution:run ()
    local gbs = self:gbs();
    
    if gbs:hasOpt("create") then
        return self:create();
    else
        lib.print_error("action for solution must be specified");
    end
    return false;
end

function solution:create ()
    local gbs = self:gbs();
    
    local solutionName  = gbs:optarg("name") or lib.die("`name' must be specified");
    local workspaceFile = fs.join(".gbs", gbs:workspaceFileName());
    local solutionFile  = fs.join(solutionName, ".gbs", gbs:solutionFileName());

    if not fs.exists(workspaceFile) then
        lib.die("can't create solution outside of workspace directory");
    end
    
    lib.assert(fs.mkdir(solutionName));
    lib.assert(fs.mkdir(fs.join(solutionName, ".gbs")));
    
    if gbs:hasOpt("git") then
        local srcFile  = fs.join(gbs:homeDir(), "template", "git", "gitignore");
        local destFile = fs.join(solutionName, ".gitignore");
        
        if not fs.exists(destFile) then
            lib.assert(fs.copy(srcFile, destFile));
        end
    end
    
    lib.assert(fs.appendLines(solutionFile, 
          "--************************************************************"
        , "--* Generated automatically by `" .. app.name() .. "'"
        , "--* Command: `" .. app.cmdline() .. "'"
        , "--* Date:    " .. os.date() 
        , "--************************************************************"
        , 'solution ' .. string.quote(solutionName)
        , '    configurations {"debug", "release"}'
        , '    platforms {"unix32", "unix64", "mswin32", "mswin64"}'
        , '    filter "platforms:*32"'
        , '        architecture "x32"'
        , '    filter "platforms:*64"'
        , '        architecture "x64"'
    ));  
    
    return true;
end

return solution;