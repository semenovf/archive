local p = premake
p.modules.gbs_configure = {}
p.modules.gbs_configure._VERSION = "1.0.0-dev"

local m = p.modules.gbs_configure

m.cxx_dialect = "C++98"

function is_empty (s)
    return s == nil or s == ''
end

newaction {
    trigger         = "gbs-configure",
    shortname       = "Griotte Build System",
    description     = "Module for control development workflow including configure, build and packaging",

    execute = function()
        if _OPTION["cxx-dialect"] then
            m.cxx_dialect = _OPTION["cxx-dialect"]
        else
            --TODO must be recognized max allowed
        end
    
        local qt5_core_lib = os.findlib("Qt5Core")
        local qt4_core_lib = os.findlib("QtCore")

        if not is_empty(qt5_core_lib) then
            print("Qt5 library found at " .. qt5_core_lib)

            if m.cxx_dialect == "C++98" or m.cxx_dialect == "gnu++98" then
                print("Qt5 require Modern C++ (C++11 or later), so Qt5 disallowed")
                qt5_core_lib = ''
            end
        end

        if not is_empty(qt4_core_lib) then
            print("Qt4 library found at " .. qt4_core_lib)
        end
    end
}

newoption {
      trigger     = "cxx-dialect"
    , value       = "CXXDIALECT"
    , description = "Specifies C++ dialect"
    , allowed     = {
          { "C++98", "C++98" }
        , { "C++11", "C++11" }
        , { "C++14", "C++14" }
        , { "C++17", "C++17" }
    }
}

newoption {
      trigger     = "cxx-dialect-max"
    , description = "Specifies recognition of maximal allowed C++ dialect"
}

newoption {
      trigger     = "cxx-dialect-min"
    , description = "Specifies recognition of mininimal allowed C++ dialect"
}

return m
