#!/usr/bin/lua

function trim(s)
  return s:match("^%s*(.*)"):match("(.-)%s*$")
end

--
-- Check GBS_HOME environment variable is set
--
local gbs_home = os.getenv("GBS_HOME")

if gbs_home == nil or gbs_home:len() == 0 then
    error("GBS_HOME environment variable must be set to GBS directory")
end

--
-- Choose make command
--
local make_cmd = "make"
local env_os = os.getenv("OS") -- Windows 2000/XP/7 return Windows_NT

if env_os == nil or env_os:len() == 0 then -- is not Windows 2000/XP/7
-- Nothing to do
else
    if env_os == "Windows_NT" then
	make_cmd = gbs_home .. "\\make.bat"
    end
end

local action = arg[1]

if action == nil then
    action = "all"
end

local file = assert(io.open("solutions.txt", "r"));

for solution in file:lines() do
    if solution:len() > 0 then -- skip empty lines
	solution = trim(solution)

	if solution:len() > 0 then
	    if not solution:match("^#") then -- skip comment lines
		command = "cd " .. solution .. "/.gbs && qmake && " .. make_cmd .. " " .. action

		if action == "all" then -- if build action then need install(deploy)
		    command = command .. " && " .. make_cmd .. " install"
		end

		if not os.execute(command) then
		    break;
		end
	    end
	end
    end
end

file:close();

