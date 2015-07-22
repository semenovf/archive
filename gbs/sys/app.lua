local app = {};

local fs = require("gbs.sys.fs");

function app.name ()
    return fs.basename(arg[0]);
end

function app.cmdline ()
    local r = app.name();
    for i = 1, #arg do
        r = r .. ' ' .. arg[i]; 
    end
    return r;
end

return app;