local Path = {};

function Path.basename (path)
    local path_separator = package.config:sub(1,1);

    local r = path:match("([^" .. path_separator .. "]-)$");
    
    if r == nil then
        return path;
    end
    
    return r;
end

function Path.separator ()
    return package.config:sub(1,1);
end

function Path.join (a, ...)
    local separator = Path.separator();
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

function Path.exists (path)
    return os.rename(path,path) and true or false;
end

function Path.touch (path)
    local fh, errstr = io.open(path, "w");
    if fh == nil then
        print_error(path .. ": can't touch file: " .. errstr); 
        return false;
    end
    io.close(fh);
    return true;
end

--
-- DEPRECATED. Use gbs.fs instead
--
function Path.execute (program, argv)
    local exe = program;
    
    if type(argv) == "table" then
        for i = 1, #argv do
            exe = exe .. ' ' .. argv[i];
        end
    else
        exe = exe .. ' ' .. argv;
    end
    
    if not os.execute(exe) then return false end;
    return true;
end

function Path.mkdir (path)
    return Path.execute("mkdir", path);
end

function Path.copy (src, dest)
    return Path.execute("cp", {src, dest});
end

return Path;