function print_error (m)
    print("ERROR: " .. m);
end

--- @brief Parses command line arguments
---
--- @param argv Array of command line arguments
--- @param argn Number of command line arguments
--- @param argi Index of command line argument to begin scan
--- @return @c Table of parsed arguments if parsing is successfull, @c nil on error.
---
function parse_options (argv, argn, argi)
    local r = {};
    local index = 1;
    
    for i = argi, argn do
        if argv[i]:find('--', 1, true) == 1 then
            local optname, optarg = argv[i]:match("^[-][-]([%a%d_-]-)=(.-)$");
            if optname == nil then
                optname = argv[i]:match("^[-][-]([%a%d_-]-)$");
                if optname == nill then
                    print_error(argv[i] .. ": bad option");
                    return nil;
                end
                r[optname] = true;
            else
                r[optname] = optarg;
            end
        else
            r[index] = argv[i];
            index = index + 1;
        end
    end
    return r;
end

function validate_name (name, what)
    local r = name:match("^([%a%d_-]-)$");
    
    if r == nil then
        print_error("invalid " .. what .. ", only alphanumeric characters, underscore ('_') and dash ('-') are valid");
        os.exit(1);
    end
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