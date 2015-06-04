#!/usr/bin/lua

function trim(s)
  return s:match("^%s*(.*)"):match("(.-)%s*$")
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
		command = "cd " .. solution .. "/.gbs && qmake && make " .. action

		if action == "all" then -- if build action then need install(deploy)
		    command = command .. " && make install"
		end

		if not os.execute(command) then
		    break;
		end
	    end
	end
    end
end

file:close();

