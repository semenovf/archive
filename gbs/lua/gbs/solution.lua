require "pfs.string";
require "gbs.utils";

local fs = require("pfs.sys.fs");

local solution = {};

function solution:new (settings)
    local o = {_settings = settings};
    self.__index = self;
    return setmetatable(o, self);
end

--function solution.usage ()
--    print("NAME");
--    print("    gbs { solution | sln } - solution manipulation");
--    print("");
--    print("SYNOPSYS");
--    print("    (1) gbs solution --create");
--    print("            --name=NAME [GIT_OPTIONS]");
--    print("");
--    print("DESCRIPTION");
--    print("    (1) - create solution with name NAME");
--    print("");
--    print("VALUES");
--    print("    NAME");
--    print("        Valid only alphanumeric characters, underscore ('_') and dash ('-')");
--    print("");
--    print("GIT OPTIONS");
--    print("    --git");
--    print("        initialize only git local repo");
----    print("    --git=github");
----    print("        initialize git local repo and synchronize it with remote repository"); 
----    print("        on GitHub (must be created previously)");
----    print("    --git=bitbucket");
----    print("        initialize git local repo and synchronize it with remote repository");
----    print("        on Bitbucket (must be created previously)");
----    print("    --git-login=LOGIN");
----    print("        login name to access remote git repository");
--end

function solution:create ()
    local settings          = self._settings;
    
    local verbose           = settings:get("Verbose") or false;
    local gbsHomeDir        = settings:get_or_throw("GbsHomeDir");
    local programName       = settings:get_or_throw("ProgramName");
    local solutionName      = settings:get_or_throw("SolutionName");
    local solutionFileName  = settings:get_or_throw("SolutionFileName");
    local workspaceFileName = settings:get_or_throw("WorkspaceFileName");
    local cmdlineString     = settings:get_or_throw("CommandLineString");
    local enableGitRepo     = settings:get_or_throw("EnableGitRepo");
        
    local workspaceFile = fs.join(".gbs", workspaceFileName);
    local solutionFile  = fs.join(solutionName, ".gbs", solutionFileName);
    
    local trn = require("gbs.transaction"):begin(verbose);

    trn:Function(function () return utils.isValidName(solutionName); end, "Validate solution name");
    trn:PathExists(workspaceFile, "Check if creating solution is inside of workspace directory");
    trn:MakeDir(solutionName, "Create solution directory");
    trn:MakeDir(fs.join(solutionName, ".gbs"), "Create solution system subdirectory");
    
    if enableGitRepo then
        local srcFile  = fs.join(gbsHomeDir, "template", "git", "gitignore");
        local destFile = fs.join(solutionName, ".gitignore");
        
        trn:CopyFileIfNotExists(srcFile, destFile, "Deploy '.gitignore'");
    end
    
    trn:AppendLinesToFile(solutionFile
        , {
               utils.fileTitle("--", programName, cmdlineString) 
            , 'solution ' .. string.quote(solutionName)
            , '    configurations {"debug", "release"}'
            , '    platforms {"unix32", "unix64", "mswin32", "mswin64"}'
            , '    filter "platforms:*32"'
            , '        architecture "x32"'
            , '    filter "platforms:*64"'
            , '        architecture "x64"'
        }, "Update solution configuration file: " .. solutionFile);  
    
    return trn:exec();
end

return solution;
