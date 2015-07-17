local Lib = {};

function __FILE__() return debug.getinfo(2,'S').source end
function __LINE__() return debug.getinfo(2, 'l').currentline end

function Lib.print_error (m)
    print("ERROR: " .. m);
end

function Lib.throw (m)
    if m ~= nil and m:len() > 0 then
        Lib.print_error(m);
        os.exit(1);
    end
    error("ERROR: exception!", 2);
    os.exit(1);
end

function Lib.assert (predicate, m)
    if not predicate then
        if m ~= nil then
            if m:len() > 0 then Lib.print_error(m); end
            os.exit(1);
        else
            assert(predicate);
        end
    end
end

function Lib.is_valid_name (name)
    local r = name:match("^[%a%d_-]-$");
    
    if r == nil then
        --Lib.print_error(name .. ": invalid name");
        return false;
    end
    return true;
end

function Lib.quote (x)
    if type(x) == "table" then
        local r = {};
        for i = 1, #x do
            table.insert(r, '"' .. x[i] .. '"');
        end
        return r;
    end
    return '"' .. x .. '"';
end 

function Lib.join (separator, a, ...)
    local r = '';

    if type(a) == "table" then
        if #a > 0 then
            r = a[1];
        end
        for i = 2, #a do
            r = r .. separator .. a[i];
        end
    else
        r = a;
    end
    
    for i = 1, select('#', ...) do
        local b = select(i, ...);
        if type(b) == "table" then
            for j = 1, #b do
                r = r .. separator .. b[j];
            end
        else
            r = r .. separator .. b;
        end
    end
    return r;
end


return Lib;