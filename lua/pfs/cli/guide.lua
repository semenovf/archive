require("pfs.die");

local guide = {};

function guide:new ()
    local o = {
          _program = arg[0]
    };
    self.__index = self;
    return setmetatable(o, self);
end

function guide:setProgram (name)
    if name == nil then
        name = arg[0];
    end
    die("Expected string value"):unless(type(name) == "string");
    self._program = name;
end

function guide:setName (text)
    self:setArticle("name", text);
    die("Expected string value"):unless(type(text) == "string");
    self._name = text;
end

function guide:setDescription (text)
    self:setArticle("description", text);
end

function _articleKey (title)
    return "_article_" .. title;
end

function guide:setArticle (title, text)
    die("Expected string value for title"):unless(type(title) == "string");
    die("Expected string value for text"):unless(type(text) == "string");
    
    self[_articleKey(title)] = text;
end

function guide:usage ()
    print("Usage:");
    print("    " .. "gbs" .. " [OPTIONS]");
    print("");
    print("COMMANDS");
    print("");
    print("OPTIONS");
end

--
-- NAME
--     %APP_NAME% - %SHORT_DESCRIPTION%
-- 
-- DESCRIPTION
--     %DESCRIPTION%
-- 
-- COMMANDS
--     <Output if router accepted action(s)>
--
-- OPTIONS
--     <Output if router accepted option(s)>
--
-- <ARTICLE_0>
--     ...
--
-- <ARTICLE_1>
--     ...
--
-- ...
--
-- AUTHORS
--     %AUTHORS%
--
-- SEE ALSO
--     %SEE_ALSO%
--
-- NOTES
--     %NOTES%
--
-- COPYRIGHT
--     %COPYRIGHT%
--
function guide:man ()
    local app = require("gbs.sys.app"):instance();
    
    print("NAME");
    print("    " .. app:name() .. " [OPTIONS]");
end

return guide;
