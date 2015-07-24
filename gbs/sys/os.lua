
--- 
--- @brief  Returns hardware and OS specific information.
---   
--- @return Table with information about OS type, bits, name and cpu architecture:
---         table.bits: 32 | 64
---         table.type: "unix" | "mswin"
---         table.os:   "linux" | "bsd" | "macos" | "mswin" | "mingw" | "cygwin" | "solaris" | "hp-ux"
---         table.cpu:  "x86" | "x86_64" | "arm" ("powerpc", "mips" - unsupported yet) 
---
os.info = function ()
    local r = {
          bits = 0
        , type = "unknown"
        , os   = "unknown"
        , cpu  = "unknown"
    };
    local path_separator = package.config:sub(1,1);
    
    if path_separator == "/" then
        r.type = "unix";
        
        local popen_status, popen_result = pcall(io.popen, "");
        if popen_status then
            popen_result:close()
            
            local fh1 = io.popen("uname -s", "r");
            local fh2 = io.popen("uname -m", "r");
            
            assert(fh1 ~= nil);
            assert(fh2 ~= nil);
            
            local os_name = fh1:read("*l");
            local cpu     = fh2:read("*l");
            
            os_name = os_name:lower();
            cpu = cpu:lower();
            
            local os_patterns = {
                  ["linux"]   = "linux"
                , ["mac"]     = "macos"
                , ["darwin"]  = "macos"
                , ["^mingw"]  = "mingw"
                , ["^cygwin"] = "cygwin"
                , ["bsd$"]    = "bsd"
                , ["sunos"]   = "solaris"
                , ["hp-ux"]   = "hp-ux"
            };
    
            for pattern, name in pairs(os_patterns) do
                if os_name:match(pattern) then
                    r.os = name;
                    break;
                end
            end
            
            local cpu_patterns = {
                  ['^x86$']   = {'x86', 32}
                , ['i[%d]86'] = {'x86', 32}
                , ['amd64']   = {'x86_64', 64}
                , ['x86_64']  = {'x86_64', 64}
                , ['^armv7']  = {'arm', 32}
                , ['^armv8']  = {'arm', 64}
--                ['power macintosh'] = {'powerpc',                
--                ['^mips']   = 'mips',
            };
            
            for pattern, item in pairs(cpu_patterns) do
                if cpu:match(pattern) then
                    r.cpu = item[1];
                    r.bits = item[2];
                    break;
                end
            end
        else
            error("unsupported popen() call");
        end
            
    elseif path_separator == "\\" then
        r.type = "mswin";
        r.os   = "mswin";
        --
        -- http://blogs.msdn.com/b/david.wang/archive/2006/03/26/howto-detect-process-bitness.aspx
        -- =======================================================================================
        -- Environment Variable / Program Bitness |  32bit Native  |  64bit Native  |  WOW64
        -- =======================================================================================
        -- PROCESSOR_ARCHITECTURE                 |       x86      |      AMD64     |   x86
        -- PROCESSOR_ARCHITEW6432                 |    undefined   |    undefined   |  AMD64
        -- =======================================================================================
        -- WOW64 = 32bit Program on 64bit OS
        -- Replace AMD64 with IA64 for Itaniums
        --
        local pro_arch     = os.getenv('PROCESSOR_ARCHITECTURE');
        local pro_arch6432 = os.getenv('PROCESSOR_ARCHITEW6432');
        
        if pro_arch == nil then
            error("expected PROCESSOR_ARCHITECTURE environment variable set on Windows");
        end
        
        if pro_arch == "x86" then
            -- 32bit Program on 64bit OS  
            if pro_arch6432 == "AMD64" or pro_arch6432 == "IA64" then
                r.bits = 64;
            else
                r.bits = 32;
            end
        elseif pro_arch == "AMD64" or pro_arch == "IA64" then
            r.bits = 64;
        else
            error("'" ..  pro_arch .. "': unexpected PROCESSOR_ARCHITECTURE environment variable value");
        end
    else
        error("'" .. path_separator .. "': unsupported path separator");
    end
    
    return r;
end
