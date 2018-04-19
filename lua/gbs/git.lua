local git = {};
require("gbs.sys.string");
local fs = require("gbs.sys.fs");
local lib = require("gbs.sys.lib");

---
--- @param solutionName Solution name.
--- @param gitHosting   Name of remote repository hosting {github, bitbucket}.
--- @param login        Login name on remote repository.
---
function git.init (solutionName, gitHosting, login)
    
    lib.assert(not string.isEmpty(solutionName));
    
    if not fs.exists(".git") then
        lib.assert(fs.execute("git", "init"));
        lib.assert(fs.execute("git", "add", "-A" , "."));
        lib.assert(fs.execute("git", "commit", "-m" , [["Initial commit"]]));
    end

    if string.isNull(gitHosting) then return true end;

    if string.isEmpty(login) then
        lib.print_error("login for '" .. gitHosting .. "' repository must be specified");
        return false;
    end

    if gitHosting == "github" then
        lib.assert(fs.execute("git", "remote", "add", "origin", "git@github.com:" .. login .. "/" .. solutionName .. ".git"));
        lib.assert(fs.execute("git", "push", "-u", "origin", "master"));
    elseif gitHosting == "bitbucket" then
        lib.assert(fs.execute("git", "remote", "add", "origin", "git@bitbucket.org:" .. login .. "/" .. solutionName .. ".git"));
        lib.assert(fs.execute("git", "push", "-u", "origin", "master"));
    end
end

return git;