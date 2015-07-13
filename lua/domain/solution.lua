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
    if not is_valid_name(name) then
        print_error("invalid solution name, only alphanumeric characters, underscore ('_') and dash ('-') are valid");
        return false;
    end
    
    local solutions_filepath = join_path(".gbs", gbs_solutions_file);
     
    if not file_exists(solutions_filepath) then
        print_error("can't create solution outside of workspace directory");
        return false;
    end
    
    if not (create_dir(name) and create_dir(join_path(name, ".gbs"))) then
        print_error(name .. ": failed to create solution");
        return false; 
    end

    local fh, errstr = io.open(solutions_filepath, "a+");
    if fh == nil then
        print_error(solutions_filepath .. ": can't open file: " .. errstr); 
        return false;
    end
    
    fh:write(name .. "\n");
    fh:flush();
    fh:close();
    
    return true;
end