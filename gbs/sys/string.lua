string.isNull = function (s)
    return s == nil;
end

string.isEmpty = function (s)
    return s == nil or s:len() == 0;
end

string.trim = function (s)
  return s:match("^%s*(.*)"):match("(.-)%s*$")
end
