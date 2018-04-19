require "pfs.die";
local fs = require("pfs.sys.fs");

local help = {};

function help:new (settings)
    local o = {
        _settings = settings
    }; 
    self.__index = self;
    return setmetatable(o, self);
end

function help:print(name)
    local settings = self._settings;
    local filepath = fs.join(settings:get("GbsHomeDir")
        , "lua"
        , "gbs"
        , "help"
        , name .. ".txt");

    die(filepath .. ": help file not found\n"):unless(fs.exists(filepath)); 

    local content = fs.readAll(filepath);

    print(content);
    print("COPYRIGHT");
    print("    Copyright (C) 2015-2016 Fedor Semenov.");
end

function help:help ()
    local settings = self._settings;
    local domain = settings:get("Domain");-- or throw("Domain expected");

    if not domain then
        self:print("help");
        return true;
    end

    if domain == "workspace" 
            or domain == "ws" then
        self:print("workspace");
    elseif domain == "solution" 
            or domain == "sln" then
        self:print("solution");
    elseif domain == "project" 
            or domain == "prj" 
            or domain == "pro" then
        self:print("project");
    else
        print_warn("bad domain for help");
        return false;
    end 
    
    return true;
end

return help;

