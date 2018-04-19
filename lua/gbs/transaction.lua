require "pfs.die";

local fs = require("pfs.sys.fs");

local transaction = {};

function transaction:begin (verbose)
    local o = {
          _sequence = require("pfs.array"):new()
        , _verbose = verbose or false
    }
    self.__index = self;
    return setmetatable(o, self);
end

function transaction:rollback ()
-- TODO need to implement
end

function transaction:commit ()
end

function transaction:Function (func, description)
    die("Expected function"):unless(type(func) == "function");
    self:append(func, {}, description);
    return self;
end

function transaction:PathExists (path, description)
    self:append(function () return fs.exists(path); end, {}, description);
    return self;
end

function transaction:PathNotExists (path, description)
    self:append(function () return not fs.exists(path); end, {}, description);
    return self;
end

function transaction:MakeDir (dir, description)
    self:append(function () return fs.mkdir(dir); end, {}, description);
    return self;
end

function transaction:MakeDirIfNotExists (dir, description)
    self:append(
        function () 
            if not fs.exists(dir) then return fs.mkdir(dir); end
            return true;
        end
        , {}, description);
    return self;
end

function transaction:AppendLinesToFile (path, lines, description)
    die("Expected table"):unless(type(lines) == "table");
    self:append(function () return fs.appendLines(path, lines); end, {}, description);
    return self;
end

function transaction:CopyFile (src, dst, description)
    self:append(function () return fs.copy(src, dst); end, {}, description);
    return self;
end

function transaction:CopyFileIfNotExists (src, dst, description)
    self:append(function ()
            if not fs.exists(dst) then
                return fs.copy(src, dst); 
            end
            return true;
        end, {}, description);
    return self;
end

function transaction:Print (text)
    if text ~= nil then
        print(text);
    end
    return self;
end

function transaction:Transaction (trn, description)
    if trn then
        self:append(function () return trn:exec() end, {}, description);
    end
    return self;
end

function transaction:append (command, args, description)
    die():unless(type(command) == "function");
    self._sequence:append({command, args, description});
end

function transaction:exec ()
    for i = 0, self._sequence:size() - 1 do
        local item        = self._sequence:at(i);
        local command     = item[1];
        local args        = item[2];
        local description = item[3];
        
        die():unless(type(item)    == "table");
        die():unless(type(command) == "function");
        die():unless(type(args)    == "table");
        
        if not command(args) then
            if description then
                print_error(description .. " ... FAILED" );
            end 
            self:rollback();
            return false;
        end
        
        if self._verbose then
            if description then
                print(description .. " ... OK" );
            end
        end
    end
    
    self:commit();
    return true;
end

return transaction;
