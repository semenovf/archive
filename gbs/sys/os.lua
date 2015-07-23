os.info = function ()
local popen_status, popen_result = pcall(io.popen, "")
if popen_status then
    popen_result:close();
    osPlatform = io.popen('uname -i', 'r'):read('*l');
    
    if osPlatform:match("^i")
else
--    -- Windows
--    local env_OS = os.getenv('OS')
--    local env_ARCH = os.getenv('PROCESSOR_ARCHITECTURE')
--    if env_OS and env_ARCH then
--        raw_os_name, raw_arch_name = env_OS, env_ARCH
--    end
end
end
