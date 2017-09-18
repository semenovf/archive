CWD = os.getcwd()
print("Working directory: " .. CWD)

function is_empty (s)
    return s == nil or s == ''
end

PFS_LINKS = {}

if is_empty(PFS_CXX_DIALECT) then
--local PFS_CXX_DIALECT=""
    PFS_CXX_DIALECT="C++98"
--local PFS_CXX_DIALECT="C++11"
--local PFS_CXX_DIALECT="C++14"
--local PFS_CXX_DIALECT="C++17"
--local PFS_CXX_DIALECT="gnu++98"
--local PFS_CXX_DIALECT="gnu++11"
--local PFS_CXX_DIALECT="gnu++14"
--local PFS_CXX_DIALECT="gnu++17"
end

configurations { "debug", "release" }
platforms { "unix32", "unix64" }

flags       { "FatalWarnings" }

if not is_empty(PFS_CXX_DIALECT) then
    cppdialect (PFS_CXX_DIALECT)
end

targetdir   "../.build"
includedirs { "../include" }
libdirs     { "../.build" }
defines     { }

qt5_core_lib = os.findlib("Qt5Core")
qt4_core_lib = os.findlib("QtCore")

if not is_empty(qt5_core_lib) then
    print("Qt5 library found at " .. qt5_core_lib)

    if PFS_CXX_DIALECT == "C++98" or PFS_CXX_DIALECT == "gnu++98" then
        print("Qt5 require Modern C++ (C++11 or later), so Qt5 disallowed")
        qt5_core_lib = ''
    end
end

if not is_empty(qt4_core_lib) then
    print("Qt4 library found at " .. qt4_core_lib)
end

--Available modules:
--    core, gui, multimedia, network, opengl, positioning, printsupport,
--    qml, quick, sensors, sql, svg, testlib, websockets, widgets, xml
--
if not is_empty(qt5_core_lib) then
    require("../../.gbs/qt5_enable")
    defines { "HAVE_QT",  "HAVE_QT5" }
    qtmodules     { "core" }
elseif not is_empty(qt4_core_lib) then
    require("../../.gbs/qt4_enable")
    defines { "HAVE_QT", "HAVE_QT4" }
    qtmodules     { "core" }
end

filter "debug"
    symbols      "On"
    defines      { "DEBUG" }
    --targetsuffix "-d"

filter "release"
    defines     { "NDEBUG" }

filter "action:gmake"
    buildoptions { "-Wall"
        , "-Wextra"
        , "-Wlogical-op"
--        , "-Wduplicated-cond"
        , "-Wno-long-long"
        , "-pedantic"
        , "-fPIC" }

filter { "release", "action:gmake" }
    buildoptions { "-Wunused" }

filter { "debug", "action:gmake" }
    linkoptions  { "-rdynamic" }

--filter { "debug", "action:gmake", "language:C++" }
filter { "debug", "action:gmake", "files:*.cpp" }
    buildoptions { "-ftemplate-backtrace-limit=0" }

filter "action:gmake"

    PTHREAD_LIB = os.findlib("pthread")
    BOOST_SYSTEM_LIB = os.findlib("boost_system")
    BOOST_REGEX_LIB = os.findlib("boost_regex")
    BOOST_THREAD_LIB = os.findlib("boost_thread")
    BOOST_CHRONO_LIB = os.findlib("boost_chrono")
    BOOST_FILESYSTEM_LIB = os.findlib("boost_filesystem")
    BOOST_RATIO_INC = os.findheader("boost/ratio.hpp")
    STDCXX_FS_INC = os.findheader("filesystem", {
               "/usr/include/c++/5/experimental"
            ,  "/usr/include/c++/6/experimental"} )
    STDCXX_FS_LIB = '';
    LIBINTL_INC = os.findheader("libintl.h")

    if not is_empty(STDCXX_FS_INC) then
        print("`std::experimental::filesystem` found at " .. STDCXX_FS_INC)
        STDCXX_FS_LIB = "stdc++fs";
        defines { "HAVE_STDCXX_FS" }
    end

    if not is_empty(PTHREAD_LIB) then
        print("`pthread` library found at " .. PTHREAD_LIB)
        defines { "HAVE_PTHREAD" }
    end

    if not is_empty(BOOST_RATIO_INC) then
        print("`Boost.Ratio` header found at " .. BOOST_RATIO_INC)
        defines { "HAVE_BOOST_RATIO" }
    end

    if not is_empty(BOOST_SYSTEM_LIB) then
        print("`Boost.System` library found at " .. BOOST_SYSTEM_LIB)
        defines { "HAVE_BOOST_SYSTEM" }
        table.insert(PFS_LINKS, "boost_system")
    end

    if not is_empty(BOOST_REGEX_LIB) then
        print("`Boost.Regex` library found at " .. BOOST_REGEX_LIB)
        defines { "HAVE_BOOST_REGEX" }
        table.insert(PFS_LINKS, "boost_regex")
    end

    if not is_empty(BOOST_THREAD_LIB) then
        print("`Boost.Thread` library found at " .. BOOST_THREAD_LIB)
        defines { "HAVE_BOOST_THREAD" }
        table.insert(PFS_LINKS, "boost_thread")
    end

    if not is_empty(BOOST_CHRONO_LIB) then
        print("`Boost.Chrono` library found at " .. BOOST_CHRONO_LIB)
        defines { "HAVE_BOOST_CHRONO" }
        table.insert(PFS_LINKS, "boost_chrono")
    end

    if not is_empty(BOOST_FILESYSTEM_LIB) then
        print("`Boost.Filesystem` library found at " .. BOOST_FILESYSTEM_LIB)
        defines { "HAVE_BOOST_FILESYSTEM" }
        table.insert(PFS_LINKS, "boost_filesystem")
    end

    if not is_empty(STDCXX_FS_LIB) then
        table.insert(PFS_LINKS, STDCXX_FS_LIB)
    end

    if not is_empty(BOOST_THREAD_LIB) then
        table.insert(PFS_LINKS, "boost_thread")
    elseif not is_empty(PTHREAD_LIB) then
        table.insert(PFS_LINKS, "pthread")
    end

    if not is_empty(LIBINTL_INC) then
        defines {"HAVE_GETTEXT"}
    end

    table.insert(PFS_LINKS, "dl")
    table.insert(PFS_LINKS, "m")
    table.insert(PFS_LINKS, "rt")
