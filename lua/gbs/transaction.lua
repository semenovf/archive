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


function PathExists (args)
    local path = args[1] or throw_expected_arg(1);
    return fs.exists(path);
end

function PathNotExists (args)
    local path = args[1] or throw_expected_arg(1);
    return not fs.exists(path);
end

function MakeDir (args)
    local dir = args[1] or throw_expected_arg(1);
    return fs.mkdir(dir);
end

function MakeDirIfNotExists (args)
    local dir = args[1] or throw_expected_arg(1);
    if not fs.exists(dir) then return fs.mkdir(dir); end
    return true;
end

function CopyFile (args)
    local src = args[1] or throw_expected_arg(1);
    local dst = args[2] or throw_expected_arg(2);
    return fs.copy(src, dst);
end

function CopyFileIfNotExists (args)
    local src = args[1] or throw_expected_arg(1);
    local dst = args[2] or throw_expected_arg(2);
    if not fs.exists(dst) then return fs.copy(src, dst); end
    return true;
end

function AppendLinesToFile (args)
    local path = args[1] or throw_expected_arg(1);
    local lines = args[2] or throw_expected_arg(2);
    return fs.appendLines(path, lines);
end

function Transaction (args)
    local tr = args[1] or throw_expected_arg(1);
    return tr:exec();
end

function transaction:append (command, args, description)
    die():unless(type(command) == "function");
--    die():unless(type(args) == "table");
    die():unless(type(description) == "string");
    
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