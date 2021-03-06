#!/usr/bin/env lua

-- Check GBS_HOME environment variable is set
local gbs_home = os.getenv("GBS_HOME");

if gbs_home == nil or gbs_home:len() == 0 then
    print_error("GBS_HOME environment variable must be set to GBS directory");
    os.exit(1);
end

package.path = gbs_home .. '/lua/?.lua;' .. package.path;
require("gbs.core");

os.exit(gbs.run(#arg, arg));
