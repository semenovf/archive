function print_error (m)
    print("ERROR: " .. m);
end

function parse_option (opt)
    local optname, optarg = opt:match("^[-][-]([%a%d_-]-)=(.-)$");
    
    if optname == nil then
        optname = opt:match("^[-][-]([%a%d_-]-)$");
    end
    
    if optarg == nil or optarg:len() == 0 then
        optarg = nil;
    end

    return optname, optarg;
end

function is_valid_name (name)
    local r = name:match("^([%a%d_-]-)$");
    
    if r == nil then
        return false;
    end

    return true;
end

function file_exists (path)
    --if type(path)~="string" then return false end
    return os.rename(path,path) and true or false;
end

function touch_file (path)
    local fh, errstr = io.open(path, "w");
    if fh == nil then
        print_error(path .. ": can't touch file: " .. errstr); 
        return false;
    end
    io.close(fh);
    return true;
end

function create_dir (path)
    if not os.execute("mkdir " .. path) then
        return false;
    end
    return true;
end

function join_path (a, b, ...)
    local path_separator = package.config:sub(1,1);
    local r = a .. path_separator .. b;
    
    for i = 1, select('#',...) do
        r = r .. path_separator .. select(i, ...);
    end
    return r;
end

function basename (path)
    local path_separator = package.config:sub(1,1);

    local r = path:match("([^" .. path_separator .. "]-)$");
    
    if r == nil then
        return path;
    end
    
    return r;
end