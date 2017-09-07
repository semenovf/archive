CWD = os.getcwd()
print("Working directory: " .. CWD)

PFS_LINKS = {}

function is_empty (s)
    return s == nil or s == ''
end

--function reverse_table (t)
--    local i, j = 1, #t
--
--    while i < j do
--        t[i], t[j] = t[j], t[i]
--
--        i = i + 1
--        j = j - 1
--    end
--end

configurations { "debug", "release" }
platforms { "unix32", "unix64" }

flags       { "FatalWarnings" }
cppdialect  "C++11"
targetdir   "../.build"
includedirs { "../include" }
libdirs     { "../.build" }
defines     { }

qt5_core_lib = os.findlib("Qt5Core")
qt4_core_lib = os.findlib("QtCore")

if qt5_core_lib then
    print("Qt5 library found at " .. qt5_core_lib)
end

if qt4_core_lib then
    print("Qt4 library found at " .. qt4_core_lib)
end

if qt5_core_lib then
    require("../../.gbs/qt5_enable")
    defines { "HAVE_QT",  "HAVE_QT5" }
elseif qt4_core_lib then
    require("../../.gbs/qt4_enable")
    defines { "HAVE_QT", "HAVE_QT4" }
end

if qt5_core_lib or qt4_core_lib then
    --Available modules:
    --    core, gui, multimedia, network, opengl, positioning, printsupport,
    --    qml, quick, sensors, sql, svg, testlib, websockets, widgets, xml
    --
    qtmodules     { "core" }
end

filter "debug"
    symbols      "On"
    defines      { "DEBUG" }
    --targetsuffix "-d"

filter "release"
    defines     { "NDEBUG" }

filter "action:gmake"
    buildoptions { "-Wno-long-long", "-pedantic", "-fPIC" }

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
    BOOST_FILESYSTEM_LIB = os.findlib("boost_filesystem")
    STDCXX_FS_INC = os.findheader("filesystem", {
               "/usr/include/c++/5/experimental"
            ,  "/usr/include/c++/6/experimental"} )
    STDCXX_FS_LIB = '';

    if not is_empty(STDCXX_FS_INC) then
        print("`std::experimental::filesystem` found at " .. STDCXX_FS_INC)
        STDCXX_FS_LIB = "stdc++fs";
        defines { "HAVE_STDCXX_FS" }
    end

    if not is_empty(PTHREAD_LIB) then
        print("`pthread` library found at " .. PTHREAD_LIB)
        defines { "HAVE_PTHREAD" }
    end

    if not is_empty(BOOST_SYSTEM_LIB) then
        print("`Boost System` library found at " .. BOOST_SYSTEM_LIB)
        defines { "HAVE_BOOST_SYSTEM" }
    end

    if not is_empty(BOOST_FILESYSTEM_LIB) then
        print("`Boost Filesystem` library found at " .. BOOST_FILESYSTEM_LIB)
        defines { "HAVE_BOOST_FILESYSTEM" }
    end

    if is_empty(STDCXX_FS_LIB) then
        if not is_empty(BOOST_FILESYSTEM_LIB) then
            table.insert(PFS_LINKS, "boost_filesystem")
        end
    else
        table.insert(PFS_LINKS, STDCXX_FS_LIB)
    end

    if not is_empty(PTHREAD_LIB) then
        table.insert(PFS_LINKS, "pthread")
    end

    table.insert(PFS_LINKS, "dl")
    table.insert(PFS_LINKS, "m")
    table.insert(PFS_LINKS, "rt")
