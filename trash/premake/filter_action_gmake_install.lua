filter "action:gmake"
    makesettings [[

GBS_INSTALL_FILE    = install -D -m 644 -p
GBS_INSTALL_BIN     = install -D -m 755 -p
GBS_INSTALL_DIR     = cp -f -R
GBS_DEL_FILE        = rm -f
GBS_SYMLINK         = ln -f -s
GBS_DEL_DIR         = rmdir
GBS_BASENAME        = basename

GBS_MAJOR_VER       = 1
GBS_MINOR_VER       = 0
GBS_RELEASE         = 0
GBS_TARGETNAME      = $(shell $(GBS_BASENAME) $(TARGET))
GBS_TARGET0         = ${HOME}/lib/$(GBS_TARGETNAME)
GBS_TARGET1         = ${HOME}/lib/$(GBS_TARGETNAME).$(GBS_MAJOR_VER)
GBS_TARGET2         = ${HOME}/lib/$(GBS_TARGETNAME).$(GBS_MAJOR_VER).$(GBS_MINOR_VER)
GBS_TARGET3         = ${HOME}/lib/$(GBS_TARGETNAME).$(GBS_MAJOR_VER).$(GBS_MINOR_VER).$(GBS_RELEASE)

install: all
	$(GBS_INSTALL_BIN)  $(TARGET) $(GBS_TARGET3)
	$(GBS_SYMLINK) $(GBS_TARGET3) $(GBS_TARGET2)
	$(GBS_SYMLINK) $(GBS_TARGET3) $(GBS_TARGET1)
	$(GBS_SYMLINK) $(GBS_TARGET3) $(GBS_TARGET0)
	$(GBS_INSTALL_DIR) ../include ${HOME}/include

uninstall:
	$(GBS_DEL_FILE) $(GBS_TARGET0)
	$(GBS_DEL_FILE) $(GBS_TARGET1)
	$(GBS_DEL_FILE) $(GBS_TARGET2)
	$(GBS_DEL_FILE) $(GBS_TARGET3)
]]
