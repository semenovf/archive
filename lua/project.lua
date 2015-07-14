function project (arg_index)
    local action = GbsPrefs.opts[arg_index];
    
    if action == nil then
        print_error("action for project must be specified");
        return 1;
    else
        if action == "create" then
            return project_create(arg[arg_index + 1]);
        end
    end
end

function project_create (name)
    validate_name(name, "project name");
    
    local solutions_filepath = join_path(".gbs", prefs.solutions_file);
     
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