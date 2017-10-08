function test_project (project_name, ext_libs)
    kind       "ConsoleApp"
    language   "C++"
    targetname ("test-" .. project_name)
    targetdir  "../../.build/tests"
    files      { "../tests/" .. project_name .. "/*.cpp", "../tests/" .. project_name .. "/*.c" }
    defines    { "PFS_TEST" }
    links      { "pfs-test", ext_libs, PFS_LINKS }

    filter "release"
        objdir (".cache/tests/" .. project_name .. "/release")

    filter "debug"
        objdir (".cache/tests/" .. project_name .. "/debug")
end
