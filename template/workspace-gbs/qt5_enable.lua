-- See https://github.com/dcourtois/premake-qt
--
-- Special for Qt projects
require(os.getenv("GBS_HOME") .. "/premake-qt/qt")
local qt = premake.extensions.qt
qt.enable()

-- Set the path for the Qt headers.
-- If this is not used, the addon will set the includepath 
-- to the include child folder of the one set by qtpath,
-- or one of the environment variable.
--
qtincludepath "/usr/include/x86_64-linux-gnu/qt5"
--

-- Set the path for the Qt libraries. 
-- If this is not used, the addon will set the includepath
-- to the lib child folder of the one set by qtpath,
-- or one of the environment variable.
--
qtlibpath "/usr/lib/x86_64-linux-gnu/qt5"

-- Set the path for the Qt binaries.
-- If this is not used, the addon will set the includepath
-- to the bin child folder of the one set by qtpath,
-- or one of the environment variable.
--
qtbinpath "/usr/lib/x86_64-linux-gnu/qt5"

-- Specify a prefix used by the libs.
-- `Qt'  for Qt4 version
-- `Qt5' for Qt5 version
--
qtprefix "Qt5"
