local Lib = {};

function Lib.print_error (m)
    print("ERROR: " .. m);
end

function Lib.throw (m)
    if m ~= nil and m:len() > 0 then
        Lib.print_error(m);
    end
    os.exit(1);
end

function Lib.assert (predicate, m)
    if not predicate then
        if m ~= nil and m:len() > 0 then
            Lib.print_error(m);
        end
        os.exit(1);
    end
end

return Lib;