/*
 * socket.cpp
 *
 *  Created on: Oct 14, 2013
 *      Author: wladt
 */

#include "../include/cwt/io/inetsocket.hpp"


/* Sources:
 * Multicast over TCP/IP HOWTO
 * 		http://tldp.org/HOWTO/Multicast-HOWTO.html
 * Socket programming:
 * 		http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6soxoverview.htm
 * Socket application design recommendations:
 * 		http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6designrec.htm
 * A connection-oriented server example:
 * 		http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconoserver.htm
 * A connectionless server example:
 * 		http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconlessserver.htm
 * A connection-oriented client example:
 * 		http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconoclient.htm
 * A connectionless client example:
 * 		http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconlessclient.htm
 * Introduction to non-blocking I/O:
 * 		http://www.kegel.com/dkftpbench/nonblocking.html
 */

namespace cwt { namespace io {

}} // cwt::io
