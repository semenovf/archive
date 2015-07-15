local File = {};

local Lib = require("lib");

function File.appendLines (filepath, lines)

    local fh, errstr = io.open(filepath, "a+");
    if fh == nil then
        Lib.print_error(filepath .. ": can't open file: " .. errstr); 
        return false;
    end
    
    for i = 1, #lines do
        fh:write(lines[i] .. "\n");
    end

    fh:flush();
    fh:close();
    
    return true;
end

return File;