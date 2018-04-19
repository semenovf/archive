string.isNull = function (s)
    return s == nil;
end

string.isEmpty = function (s)
    return s == nil or s:len() == 0;
end

string.trim = function (s)
  return s:match("^%s*(.*)"):match("(.-)%s*$")
end

string.quote = function (x)
    if type(x) == "table" then
        local r = {};
        for i = 1, #x do
            table.insert(r, '"' .. x[i] .. '"');
        end
        return r;
    end
    return '"' .. x .. '"';
end 

string.join = function (separator, a, ...)
    local r = '';

    if type(a) == "table" then
        if #a > 0 then
            r = a[1];
        end
        for i = 2, #a do
            r = r .. separator .. a[i];
        end
    else
        r = a;
    end
    
    for i = 1, select('#', ...) do
        local b = select(i, ...);
        if type(b) == "table" then
            for j = 1, #b do
                r = r .. separator .. b[j];
            end
        else
            r = r .. separator .. b;
        end
    end
    return r;
end