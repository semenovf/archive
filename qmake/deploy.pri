message($$CWT_CONFIG)

contains(CWT_CONFIG, gbs_dll) {
    header_files.files = $$HEADERS
    target.path = $$(HOME)/lib
    header_files.path = $$(HOME)/include

    INSTALLS += header_files target
}

contains(CWT_CONFIG, gbs_app) {
    target.path = $$(HOME)/bin
    INSTALLS += target
}

contains(CWT_CONFIG, gbs_petaloid) {
    target.path = $$(HOME)/bin
    INSTALLS += target
}

contains(CWT_CONFIG, gbs_test) {
}

