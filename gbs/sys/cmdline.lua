require("gbs.sys.die");

local cmdline = {}

function cmdline:new ()
    local o = {
          _program = arg[0]
    }
    return setmetatable(o, self);
end

function cmdline:setProgram (name)
    if name == nil then
        name = arg[0];
    end
    die("Expected string value"):unless(type(name) == "string");
    self._program = name;
end

function cmdline:setName (text)
    self:setArticle("name", text);
    die("Expected string value"):unless(type(text) == "string");
    self._name = text;
end

function cmdline:setDescription (text)
    self:setArticle("description", text);
end

function _articleKey (title)
    return "_article_" .. title;
end

function cmdline:setArticle (title, text)
    die("Expected string value for title"):unless(type(title) == "string");
    die("Expected string value for text"):unless(type(text) == "string");
    
    self[_articleKey(title)] = text;
end

return cmdline;
