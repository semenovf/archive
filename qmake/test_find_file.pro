include(find_file.pri)

path = $$scanParentDirsToFindFile(./, .profile)
fileNotFound = $$scanParentDirsToFindFile(./, abrakadabra)

message(Found $$path)

isEmpty(fileNotFound) {
    message(abrakadabra: file not found)
}