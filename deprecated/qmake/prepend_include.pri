defineReplace(prependIncludePath) {
    inc = $$INCLUDEPATH
    INCLUDEPATH = $$1
    INCLUDEPATH += $$inc
    return($$INCLUDEPATH)
}


