local lib = {};

function __FILE__() return debug.getinfo(2,'S').source end
function __LINE__() return debug.getinfo(2, 'l').currentline end

function lib.print_error (m)
    print("ERROR: " .. m);
end

function lib.throw (m)
    if m ~= nil and m:len() > 0 then
        error("ERROR: " .. m);
    else
        error("ERROR: exception!", 2);
    end
end

function lib.assert (predicate, m)
    if not predicate then
        if m ~= nil then
            if m:len() > 0 then lib.print_error(m); end
            os.exit(1);
        else
            assert(predicate);
        end
    end
end

function lib.is_valid_name (name)
    local r = name:match("^[%a%d_-]-$");
    
    if r == nil then
        -- lib.print_error(name .. ": invalid name");
        return false;
    end
    return true;
end

function lib.runAction (action, actions)
    lib.assert(type(action) == "string");
    lib.assert(type(actions) == "table");
    
    local func = actions[action];
    if type(func) == "function" then
        return func();
    end
    
    lib.print_error(action .. ": bad action");
    return false;
end

return lib;
