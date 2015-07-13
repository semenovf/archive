local separator = package.config[1];

function workspace (start_arg_index)
    local action = arg[start_arg_index];
    
    if action == nil then
        print_error("action for workspace must be specified");
        return 1;
    elseif action == "create" then
        return workspace_create(arg[start_arg_index + 1]);
    else
        print_error(action .. ": bad action");
    end
    
    return 1;
end

function workspace_create (path)
    if path == nil then
        print_error("workspace path must be specified");
        return false;
    end
    
    if file_exists(path) then
        print_error(path .. ": can't create workspace: directory already exists");
        return false;
    end
    
    local solutions_filepath = join_path(path, ".gbs", gbs_solutions_file);
    
    if not (create_dir(path)
            and create_dir(join_path(path, ".gbs"))
            and touch_file(solutions_filepath)) then
        print_error(path .. ": failed to create workspace");
        return false; 
    end
    
    local fh, errstr = io.open(solutions_filepath, "a+");
    if fh == nil then
        print_error(solutions_filepath .. ": can't open file: " .. errstr); 
        return false;
    end
    
    local appname = basename(arg[0]);
    local cmdline = appname;
    for i = 1, #arg do
        cmdline = cmdline .. ' ' .. arg[i]; 
    end
    
    fh:write("#************************************************************\n");
    fh:write("#* Generated automatically by " .. appname .. "\n");
    fh:write("#* Command: " .. cmdline .. "\n");
--    echo "#* Author:  $USERNAME"                                         >> solution.pro
    fh:write("#* Date:    " .. os.date() .. "\n");
    fh:write("#************************************************************\n");
        
    fh:flush();
    fh:close();
    
    return true;
end
