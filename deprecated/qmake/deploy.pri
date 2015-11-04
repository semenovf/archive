#message($$CWT_CONFIG)

contains(CWT_CONFIG, gbs_dll) {
    header_files.files = $$HEADERS
    target.path = $$CWT_LIB_DIR
    header_files.path = $$CWT_H_DIR

    INSTALLS += header_files target
}

contains(CWT_CONFIG, gbs_app) {
    target.path = $$CWT_BIN_DIR
    INSTALLS += target
}

contains(CWT_CONFIG, gbs_petaloid) {
    target.path = $$CWT_BIN_DIR
    INSTALLS += target
}

contains(CWT_CONFIG, gbs_test) {
}
