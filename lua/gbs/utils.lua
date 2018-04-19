require "pfs.sys.os";

utils = {};

function utils.fileTitle (inlineCommentSeq, programName, cmdlineString)
    return inlineCommentSeq .. "************************************************************\n"
        .. inlineCommentSeq .. "* Generated automatically by `" .. programName .. "'\n"
        .. inlineCommentSeq .. "* Command: `" .. programName .. " " .. cmdlineString .. "'\n"
        .. inlineCommentSeq .. "* Date:    " .. os.date() .. "\n"
        .. inlineCommentSeq .. "************************************************************\n";
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
