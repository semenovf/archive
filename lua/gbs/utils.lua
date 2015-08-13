require "pfs.sys.os";

utils = {};

function utils.fileTitle (programName, cmdlineString)
    return "#************************************************************\n"
        .. "#* Generated automatically by `" .. programName .. "'\n"
        .. "#* Command: `" .. programName .. " " .. cmdlineString .. "'\n"
        .. "#* Date:    " .. os.date() .. "\n"
        .. "#************************************************************\n";
end

function utils.isValidName (name)
    if type(name) ~= "string" then return false; end
    local r = name:match("^[%a%d_-]-$");
    
    if r == nil then
        -- lib.print_error(name .. ": invalid name");
        return false;
    end
    return true;
end
