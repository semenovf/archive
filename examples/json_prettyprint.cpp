/*
 * json_prettyprint.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: wladt
 */

#include <cstdio>
#include "../include/cwt/json.hpp"

using namespace cwt;

static const char *json_object_str = "{                                \
		\"Image\": {                                                   \
			\"Width\":  800,                                           \
			\"Height\": 600,                                           \
			\"Title\":  \"View from 15th Floor\",                      \
			\"Thumbnail\": {                                           \
				\"Url\":    \"http://www.example.com/image/481989943\",\
				\"Height\": 125,                                       \
				\"Width\":  \"100\"                                    \
			},                                                         \
			\"IDs\": [116, 943, 234, 38793]                            \
		  }                                                            \
	 }";


int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);

	Json json_object(String::fromUtf8(json_object_str));
    JsonPrettyPrint jsonPrettyPrint(JsonPrettyPrint::StyleCompact);

    printf("%s\n", jsonPrettyPrint.toString(json_object).toUtf8().data());

    return EXIT_SUCCESS;
}



