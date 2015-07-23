local fs = {}

local lib = require("gbs.sys.lib");

function fs.execute (program, ...)
    local cmd = program;

    for i = 1, select('#', ...) do
        local a = select(i, ...);
        if type(a) == "table" then
            for j = 1, #a do
                cmd = cmd .. ' ' .. a[i];
            end
        else
            cmd = cmd .. ' ' .. a;
        end
    end
   
    if not os.execute(cmd) then return false end;
    return true;
end

function fs.executePremake (...)
    return fs.execute("premake5", ...);
end

function fs.separator ()
    return package.config:sub(1,1);
end

function fs.exists (path)
    local f = io.open(path, "r");
    if f ~= nil then io.close(f) return true else return false end
--    return os.rename(path,path) and true or false;
end

function fs.mkdir (dir)
    return fs.execute("mkdir", dir);
end

function fs.chdir (dir)
    -- need to support Windows specific cd '/d' 
    return fs.execute("cd", dir);
end

function fs.copy (src, dest)
    return fs.execute("cp", src, dest);
end

function fs.join (a, ...)
    local separator = fs.separator();
    local r = '';

    local ta = a;
    
    if type(a) ~= "table" then
        ta = { a };
    end
    
    if #ta > 0 then
        r = ta[1];
    end
    for i = 2, #ta do
        if ta[i]:find(separator, 1, true) == 1 then -- Absolute path
            r = ta[i];
        else
            r = r .. separator .. ta[i];
        end
    end
    
    for i = 1, select('#', ...) do
        local b = select(i, ...);
        local tb = b;
        
        if type(b) ~= "table" then
            tb = { b };
        end
        
        for j = 1, #tb do
            if tb[j]:find(separator, 1, true) == 1 then -- Absolute path
                r = tb[j];
            else
                r = r .. separator .. tb[j];
            end
        end
    end
    return r;
end

function fs.basename (path)
    local path_separator = package.config:sub(1,1);

    local r = path:match("([^" .. path_separator .. "]-)$");
    
    if r == nil then
        return path;
    end
    
    return r;
end

function fs.touch (path)
    local fh, errstr = io.open(path, "w");
    if fh == nil then
        print_error(path .. ": can't touch file: " .. errstr); 
        return false;
    end
    io.close(fh);
    return true;
end

function fs.appendLines (filepath, ...)

    local fh, errstr = io.open(filepath, "a+");
    if fh == nil then
        lib.print_error(filepath .. ": can't open file: " .. errstr); 
        return false;
    end
    
    for i = 1, select('#', ...) do
        local line = select(i, ...);
        local lines;
        
        if type(line) == "table" then
            lines = line;
        else
            lines = { line };
        end
        
        for j = 1, #lines do
            fh:write(lines[j] .. "\n");
        end
    end
    
--    if type(lines) == "table" then
--        for i = 1, #lines do
--            fh:write(lines[i] .. "\n");
--        end
--    else
--        fh:write(lines .. "\n");
--    end

    fh:flush();
    fh:close();
    
    return true;
end

return fs;