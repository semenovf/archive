function solution (start_arg_index)
    local action = arg[start_arg_index];
    
    if action == nil then
        print_error("action for solution must be specified");
        return 1;
    elseif action == "create" then
        return solution_create(arg[start_arg_index + 1]);
    end
end

function solution_create (name)
    is_valid_name(name) or error("bad solution name");
    validate_name(name, "solution name");
    
    local workspace_filepath = join_path(".gbs", GbsPrefs.workspace_file);
     
    if not file_exists(workspace_filepath) then
        print_error("can't create solution outside of workspace directory");
        return false;
    end
    
    if not (create_dir(name) and create_dir(join_path(name, ".gbs"))) then
        print_error(name .. ": failed to create solution");
        return false; 
    end

    local fh, errstr = io.open(workspace_filepath, "a+");
    if fh == nil then
        print_error(workspace_filepath .. ": can't open file: " .. errstr); 
        return false;
    end
    
    fh:write(name .. "\n");
    fh:flush();
    fh:close();
    
    return true;
end