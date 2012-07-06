/*
 * colorx.c
 *
 *  Created on: 15.06.2012
 *      Author: wladt
 */


#include <cwt/stylex/colorx.h>
#include <cwt/string.h>
#include <cwt/strutils.h>
#include <cwt/algo/hash_tab.h>
#include <cwt/algo/hash_str.h>
#include <cwt/algo/cmp_str.h>

static HashTable *__cwt_color_table = NULL;

static struct {
	const CHAR *name;
	Cwt_Color color;
} __cwt_color_names[] = {
	  { "Transparent",          CWT_Transparent          }
	, { "AliceBlue",            CWT_AliceBlue            }
	, { "AntiqueWhite",         CWT_AntiqueWhite         }
	, { "Aqua",                 CWT_Aqua                 }
	, { "Aquamarine",           CWT_Aquamarine           }
	, { "Azure",                CWT_Azure                }
	, { "Beige",                CWT_Beige                }
	, { "Bisque",               CWT_Bisque               }
	, { "Black",                CWT_Black                }
	, { "BlanchedAlmond",       CWT_BlanchedAlmond       }
	, { "Blue",                 CWT_Blue                 }
	, { "BlueViolet",           CWT_BlueViolet           }
	, { "Brown",                CWT_Brown                }
	, { "BurlyWood",            CWT_BurlyWood            }
	, { "CadetBlue",            CWT_CadetBlue            }
	, { "Chartreuse",           CWT_Chartreuse           }
	, { "Chocolate",            CWT_Chocolate            }
	, { "Coral",                CWT_Coral                }
	, { "CornflowerBlue",       CWT_CornflowerBlue       }
	, { "Cornsilk",             CWT_Cornsilk             }
	, { "Crimson",              CWT_Crimson              }
	, { "Cyan",                 CWT_Cyan                 }
	, { "DarkBlue",             CWT_DarkBlue             }
	, { "DarkCyan",             CWT_DarkCyan             }
	, { "DarkGoldenRod",        CWT_DarkGoldenRod        }
	, { "DarkGray",             CWT_DarkGray             }
	, { "DarkGrey",             CWT_DarkGrey             }
	, { "DarkGreen",            CWT_DarkGreen            }
	, { "DarkKhaki",            CWT_DarkKhaki            }
	, { "DarkMagenta",          CWT_DarkMagenta          }
	, { "DarkOliveGreen",       CWT_DarkOliveGreen       }
	, { "Darkorange",           CWT_Darkorange           }
	, { "DarkOrchid",           CWT_DarkOrchid           }
	, { "DarkRed",              CWT_DarkRed              }
	, { "DarkSalmon",           CWT_DarkSalmon           }
	, { "DarkSeaGreen",         CWT_DarkSeaGreen         }
	, { "DarkSlateBlue",        CWT_DarkSlateBlue        }
	, { "DarkSlateGray",        CWT_DarkSlateGray        }
	, { "DarkSlateGrey",        CWT_DarkSlateGrey        }
	, { "DarkTurquoise",        CWT_DarkTurquoise        }
	, { "DarkViolet",           CWT_DarkViolet           }
	, { "DeepPink",             CWT_DeepPink             }
	, { "DeepSkyBlue",          CWT_DeepSkyBlue          }
	, { "DimGray",              CWT_DimGray              }
	, { "DimGrey",              CWT_DimGrey              }
	, { "DodgerBlue",           CWT_DodgerBlue           }
	, { "FireBrick",            CWT_FireBrick            }
	, { "FloralWhite",          CWT_FloralWhite          }
	, { "ForestGreen",          CWT_ForestGreen          }
	, { "Fuchsia",              CWT_Fuchsia              }
	, { "Gainsboro",            CWT_Gainsboro            }
	, { "GhostWhite",           CWT_GhostWhite           }
	, { "Gold",                 CWT_Gold                 }
	, { "GoldenRod",            CWT_GoldenRod            }
	, { "Gray",                 CWT_Gray                 }
	, { "Grey",                 CWT_Grey                 }
	, { "Green",                CWT_Green                }
	, { "GreenYellow",          CWT_GreenYellow          }
	, { "HoneyDew",             CWT_HoneyDew             }
	, { "HotPink",              CWT_HotPink              }
	, { "IndianRed",            CWT_IndianRed            }
	, { "Indigo",               CWT_Indigo               }
	, { "Ivory",                CWT_Ivory                }
	, { "Khaki",                CWT_Khaki                }
	, { "Lavender",             CWT_Lavender             }
	, { "LavenderBlush",        CWT_LavenderBlush        }
	, { "LawnGreen",            CWT_LawnGreen            }
	, { "LemonChiffon",         CWT_LemonChiffon         }
	, { "LightBlue",            CWT_LightBlue            }
	, { "LightCoral",           CWT_LightCoral           }
	, { "LightCyan",            CWT_LightCyan            }
	, { "LightGoldenRodYellow", CWT_LightGoldenRodYellow }
	, { "LightGray",            CWT_LightGray            }
	, { "LightGrey",            CWT_LightGrey            }
	, { "LightGreen",           CWT_LightGreen           }
	, { "LightPink",            CWT_LightPink            }
	, { "LightSalmon ",         CWT_LightSalmon          }
	, { "LightSeaGreen",        CWT_LightSeaGreen        }
	, { "LightSkyBlue",         CWT_LightSkyBlue         }
	, { "LightSlateGray",       CWT_LightSlateGray       }
	, { "LightSlateGrey",       CWT_LightSlateGrey       }
	, { "LightSteelBlue",       CWT_LightSteelBlue       }
	, { "LightYellow",          CWT_LightYellow          }
	, { "Lime",                 CWT_Lime                 }
	, { "LimeGreen",            CWT_LimeGreen            }
	, { "Linen",                CWT_Linen                }
	, { "Magenta",              CWT_Magenta              }
	, { "Maroon",               CWT_Maroon               }
	, { "MediumAquaMarine",     CWT_MediumAquaMarine     }
	, { "MediumBlue",           CWT_MediumBlue           }
	, { "MediumOrchid",         CWT_MediumOrchid         }
	, { "MediumPurple",         CWT_MediumPurple         }
	, { "MediumSeaGreen",       CWT_MediumSeaGreen       }
	, { "MediumSlateBlue",      CWT_MediumSlateBlue      }
	, { "MediumSpringGreen",    CWT_MediumSpringGreen    }
	, { "MediumTurquoise",      CWT_MediumTurquoise      }
	, { "MediumVioletRed",      CWT_MediumVioletRed      }
	, { "MidnightBlue",         CWT_MidnightBlue         }
	, { "MintCream",            CWT_MintCream            }
	, { "MistyRose",            CWT_MistyRose            }
	, { "Moccasin",             CWT_Moccasin             }
	, { "NavajoWhite",          CWT_NavajoWhite          }
	, { "Navy",                 CWT_Navy                 }
	, { "OldLace",              CWT_OldLace              }
	, { "Olive",                CWT_Olive                }
	, { "OliveDrab",            CWT_OliveDrab            }
	, { "Orange",               CWT_Orange               }
	, { "OrangeRed",            CWT_OrangeRed            }
	, { "Orchid",               CWT_Orchid               }
	, { "PaleGoldenRod",        CWT_PaleGoldenRod        }
	, { "PaleGreen",            CWT_PaleGreen            }
	, { "PaleTurquoise",        CWT_PaleTurquoise        }
	, { "PaleVioletRed",        CWT_PaleVioletRed        }
	, { "PapayaWhip",           CWT_PapayaWhip           }
	, { "PeachPuff",            CWT_PeachPuff            }
	, { "Peru",                 CWT_Peru                 }
	, { "Pink",                 CWT_Pink                 }
	, { "Plum",                 CWT_Plum                 }
	, { "PowderBlue",           CWT_PowderBlue           }
	, { "Purple",               CWT_Purple               }
	, { "Red",                  CWT_Red                  }
	, { "RosyBrown",            CWT_RosyBrown            }
	, { "RoyalBlue",            CWT_RoyalBlue            }
	, { "SaddleBrown",          CWT_SaddleBrown          }
	, { "Salmon",               CWT_Salmon               }
	, { "SandyBrown",           CWT_SandyBrown           }
	, { "SeaGreen",             CWT_SeaGreen             }
	, { "SeaShell",             CWT_SeaShell             }
	, { "Sienna",               CWT_Sienna               }
	, { "Silver",               CWT_Silver               }
	, { "SkyBlue",              CWT_SkyBlue              }
	, { "SlateBlue",            CWT_SlateBlue            }
	, { "SlateGray",            CWT_SlateGray            }
	, { "SlateGrey",            CWT_SlateGrey            }
	, { "Snow",                 CWT_Snow                 }
	, { "SpringGreen",          CWT_SpringGreen          }
	, { "SteelBlue",            CWT_SteelBlue            }
	, { "Tan",                  CWT_Tan                  }
	, { "Teal",                 CWT_Teal                 }
	, { "Thistle",              CWT_Thistle              }
	, { "Tomato",               CWT_Tomato               }
	, { "Turquoise",            CWT_Turquoise            }
	, { "Violet",               CWT_Violet               }
	, { "Wheat",                CWT_Wheat                }
	, { "White",                CWT_White                }
	, { "WhiteSmoke",           CWT_WhiteSmoke           }
	, { "Yellow",               CWT_Yellow               }
	, { "YellowGreen",          CWT_YellowGreen          }
};


void _cwt_colorx_init(void)
{
	size_t i;
	size_t n;

	__cwt_color_table = hash_table_new(string_hash, string_equal);
	hash_table_register_free_functions(__cwt_color_table, cwtFree, NULL);

	n = sizeof(__cwt_color_names)/sizeof(__cwt_color_names[0]);
	for( i = 0; i < n; i++  ) {
		CHAR *key = cwtStrDup(__cwt_color_names[i].name);
		str_tolower(key, key, cwtStrLen(key));
		CWT_ASSERT(hash_table_insert(__cwt_color_table
			, key, &__cwt_color_names[i].color));
	}
}

void _cwt_colorx_release(void)
{
	CWT_ASSERT(__cwt_color_table);
	hash_table_free(__cwt_color_table);
}


Cwt_Color cwt_stylex_colorByName(const CHAR *name)
{
	CHAR *key;
	Cwt_Color *pcolor;

	CWT_ASSERT(name);
	key = cwtStrDup(name);
	str_tolower(key, key, strlen(key));

	pcolor = (Cwt_Color*)hash_table_lookup(__cwt_color_table, key);

	CWT_FREE(key);

	if( NULL == pcolor ) {
		return (Cwt_Color)CWT_InvalidColor;
	}
	return *pcolor;
}


/**
 * Выполняет разбор строки, определяющией цвет
 *
 * Строка, определяющая цвет имеет вид:
 *   - '#'RRGGBB (где R, G и B шестнадцатиричные числа)
 *   - название, например, "blue", "red", "green" и т.д.
 *
 *
 * @param str
 * @param pval
 * @return
 */
BOOL cwt_stylex_parseColor(const CHAR *str, Cwt_Color *pcolor)
{
	BOOL ok = FALSE;
	Cwt_Color color;

	if( str && strlen(str) > 0 ) {
		if( *str == '#' ) {
			str++;
			color = cwtStrToUINT32(str, 16, &ok);
		} else {
			color = cwt_stylex_colorByName(str);
			if( color != CWT_InvalidColor ) {
				ok = TRUE;
			}
		}
	}

	if( ok ) {
		if( color == CWT_Transparent || color <= CWT_MaxColor ) {
			if( pcolor )
				*pcolor = color;
		} else {
			ok = FALSE;
		}
	}
	return ok;
}


#ifdef CWT_USE_TESTS
#include <cwt/test.h>
void cwt_stylex_test_parse_color(void)
{
	Cwt_Color color;

	CWT_TEST_OK(cwt_stylex_parseColor("Blue", &color));
	CWT_TEST_OK(color == CWT_Blue);
	CWT_TEST_OK(cwt_stylex_parseColor("#7FFF00", &color));
	CWT_TEST_OK(color == CWT_Chartreuse);
	CWT_TEST_OK(cwt_stylex_parseColor("#000000", &color));
	CWT_TEST_OK(color == CWT_Black);
	CWT_TEST_OK(cwt_stylex_parseColor("#FFFFFF", &color));
	CWT_TEST_OK(color == CWT_White);
	CWT_TEST_NOK(cwt_stylex_parseColor("7FFF00", &color));        /* prefix '#' is omit */
	CWT_TEST_NOK(cwt_stylex_parseColor("#FFFFFFFFFFFF", &color)); /* too big value*/
	CWT_TEST_NOK(cwt_stylex_parseColor("#QWERTY", &color));       /* is not a hex value */
	CWT_TEST_NOK(cwt_stylex_parseColor("#1000000", &color));      /* greater than MaxColor value */
}
#endif
