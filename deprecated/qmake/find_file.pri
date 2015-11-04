defineReplace(scanParentDirsToFindFile) {

    dir = $$1
    fileName = $$basename(2)
    path = $$dir/$$fileName
    returnValue=
    parentDir = ../

#    message("WD: " $$dir)

    for(i, 1..10) {
	isEmpty(returnValue) {
#	    message(Check $$path)
	    
	    exists($$path) {
#		message(Exists $$path)
		#
		# return() does not stop the loop,
		# it will continue a process adding argument to return list
		#
		returnValue = $$path
		return($$returnValue)
	    } else {
		dir = $$dir$$parentDir
    		path = $$dir$$fileName
#		message("WD: " $$dir)
#		message("path: " $$path)
	    }
	}
    }
}


