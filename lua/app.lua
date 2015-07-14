local App = {};

local Path = require('path');

function App.name ()
    return Path.basename(arg[0]);
end

function App.cmdline ()
    local r = App.name();
    for i = 1, #arg do
        r = r .. ' ' .. arg[i]; 
    end
    return r;
end

return App;