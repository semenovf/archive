/*
 * colorx.c
 *
 *  Created on: 15.06.2012
 *      Author: wladt
 */


#include <cwt/stylex/colorx.h>
#include <cwt/string.h>
#include <cwt/str.h>
#include <cwt/algo/hash_tab.h>
#include <cwt/algo/hash_str.h>
#include <cwt/algo/cmp_str.h>

static HashTable *__cwt_color_table = NULL;

static struct {
	const CWT_CHAR *name;
	Cwt_Color color;
} __cwt_color_names[] = {
	  { _T("Transparent"),          CWT_Transparent          }
	, { _T("AliceBlue"),            CWT_AliceBlue            }
	, { _T("AntiqueWhite"),         CWT_AntiqueWhite         }
	, { _T("Aqua"),                 CWT_Aqua                 }
	, { _T("Aquamarine"),           CWT_Aquamarine           }
	, { _T("Azure"),                CWT_Azure                }
	, { _T("Beige"),                CWT_Beige                }
	, { _T("Bisque"),               CWT_Bisque               }
	, { _T("Black"),                CWT_Black                }
	, { _T("BlanchedAlmond"),       CWT_BlanchedAlmond       }
	, { _T("Blue"),                 CWT_Blue                 }
	, { _T("BlueViolet"),           CWT_BlueViolet           }
	, { _T("Brown"),                CWT_Brown                }
	, { _T("BurlyWood"),            CWT_BurlyWood            }
	, { _T("CadetBlue"),            CWT_CadetBlue            }
	, { _T("Chartreuse"),           CWT_Chartreuse           }
	, { _T("Chocolate"),            CWT_Chocolate            }
	, { _T("Coral"),                CWT_Coral                }
	, { _T("CornflowerBlue"),       CWT_CornflowerBlue       }
	, { _T("Cornsilk"),             CWT_Cornsilk             }
	, { _T("Crimson"),              CWT_Crimson              }
	, { _T("Cyan"),                 CWT_Cyan                 }
	, { _T("DarkBlue"),             CWT_DarkBlue             }
	, { _T("DarkCyan"),             CWT_DarkCyan             }
	, { _T("DarkGoldenRod"),        CWT_DarkGoldenRod        }
	, { _T("DarkGray"),             CWT_DarkGray             }
	, { _T("DarkGrey"),             CWT_DarkGrey             }
	, { _T("DarkGreen"),            CWT_DarkGreen            }
	, { _T("DarkKhaki"),            CWT_DarkKhaki            }
	, { _T("DarkMagenta"),          CWT_DarkMagenta          }
	, { _T("DarkOliveGreen"),       CWT_DarkOliveGreen       }
	, { _T("Darkorange"),           CWT_Darkorange           }
	, { _T("DarkOrchid"),           CWT_DarkOrchid           }
	, { _T("DarkRed"),              CWT_DarkRed              }
	, { _T("DarkSalmon"),           CWT_DarkSalmon           }
	, { _T("DarkSeaGreen"),         CWT_DarkSeaGreen         }
	, { _T("DarkSlateBlue"),        CWT_DarkSlateBlue        }
	, { _T("DarkSlateGray"),        CWT_DarkSlateGray        }
	, { _T("DarkSlateGrey"),        CWT_DarkSlateGrey        }
	, { _T("DarkTurquoise"),        CWT_DarkTurquoise        }
	, { _T("DarkViolet"),           CWT_DarkViolet           }
	, { _T("DeepPink"),             CWT_DeepPink             }
	, { _T("DeepSkyBlue"),          CWT_DeepSkyBlue          }
	, { _T("DimGray"),              CWT_DimGray              }
	, { _T("DimGrey"),              CWT_DimGrey              }
	, { _T("DodgerBlue"),           CWT_DodgerBlue           }
	, { _T("FireBrick"),            CWT_FireBrick            }
	, { _T("FloralWhite"),          CWT_FloralWhite          }
	, { _T("ForestGreen"),          CWT_ForestGreen          }
	, { _T("Fuchsia"),              CWT_Fuchsia              }
	, { _T("Gainsboro"),            CWT_Gainsboro            }
	, { _T("GhostWhite"),           CWT_GhostWhite           }
	, { _T("Gold"),                 CWT_Gold                 }
	, { _T("GoldenRod"),            CWT_GoldenRod            }
	, { _T("Gray"),                 CWT_Gray                 }
	, { _T("Grey"),                 CWT_Grey                 }
	, { _T("Green"),                CWT_Green                }
	, { _T("GreenYellow"),          CWT_GreenYellow          }
	, { _T("HoneyDew"),             CWT_HoneyDew             }
	, { _T("HotPink"),              CWT_HotPink              }
	, { _T("IndianRed"),            CWT_IndianRed            }
	, { _T("Indigo"),               CWT_Indigo               }
	, { _T("Ivory"),                CWT_Ivory                }
	, { _T("Khaki"),                CWT_Khaki                }
	, { _T("Lavender"),             CWT_Lavender             }
	, { _T("LavenderBlush"),        CWT_LavenderBlush        }
	, { _T("LawnGreen"),            CWT_LawnGreen            }
	, { _T("LemonChiffon"),         CWT_LemonChiffon         }
	, { _T("LightBlue"),            CWT_LightBlue            }
	, { _T("LightCoral"),           CWT_LightCoral           }
	, { _T("LightCyan"),            CWT_LightCyan            }
	, { _T("LightGoldenRodYellow"), CWT_LightGoldenRodYellow }
	, { _T("LightGray"),            CWT_LightGray            }
	, { _T("LightGrey"),            CWT_LightGrey            }
	, { _T("LightGreen"),           CWT_LightGreen           }
	, { _T("LightPink"),            CWT_LightPink            }
	, { _T("LightSalmon "),         CWT_LightSalmon          }
	, { _T("LightSeaGreen"),        CWT_LightSeaGreen        }
	, { _T("LightSkyBlue"),         CWT_LightSkyBlue         }
	, { _T("LightSlateGray"),       CWT_LightSlateGray       }
	, { _T("LightSlateGrey"),       CWT_LightSlateGrey       }
	, { _T("LightSteelBlue"),       CWT_LightSteelBlue       }
	, { _T("LightYellow"),          CWT_LightYellow          }
	, { _T("Lime"),                 CWT_Lime                 }
	, { _T("LimeGreen"),            CWT_LimeGreen            }
	, { _T("Linen"),                CWT_Linen                }
	, { _T("Magenta"),              CWT_Magenta              }
	, { _T("Maroon"),               CWT_Maroon               }
	, { _T("MediumAquaMarine"),     CWT_MediumAquaMarine     }
	, { _T("MediumBlue"),           CWT_MediumBlue           }
	, { _T("MediumOrchid"),         CWT_MediumOrchid         }
	, { _T("MediumPurple"),         CWT_MediumPurple         }
	, { _T("MediumSeaGreen"),       CWT_MediumSeaGreen       }
	, { _T("MediumSlateBlue"),      CWT_MediumSlateBlue      }
	, { _T("MediumSpringGreen"),    CWT_MediumSpringGreen    }
	, { _T("MediumTurquoise"),      CWT_MediumTurquoise      }
	, { _T("MediumVioletRed"),      CWT_MediumVioletRed      }
	, { _T("MidnightBlue"),         CWT_MidnightBlue         }
	, { _T("MintCream"),            CWT_MintCream            }
	, { _T("MistyRose"),            CWT_MistyRose            }
	, { _T("Moccasin"),             CWT_Moccasin             }
	, { _T("NavajoWhite"),          CWT_NavajoWhite          }
	, { _T("Navy"),                 CWT_Navy                 }
	, { _T("OldLace"),              CWT_OldLace              }
	, { _T("Olive"),                CWT_Olive                }
	, { _T("OliveDrab"),            CWT_OliveDrab            }
	, { _T("Orange"),               CWT_Orange               }
	, { _T("OrangeRed"),            CWT_OrangeRed            }
	, { _T("Orchid"),               CWT_Orchid               }
	, { _T("PaleGoldenRod"),        CWT_PaleGoldenRod        }
	, { _T("PaleGreen"),            CWT_PaleGreen            }
	, { _T("PaleTurquoise"),        CWT_PaleTurquoise        }
	, { _T("PaleVioletRed"),        CWT_PaleVioletRed        }
	, { _T("PapayaWhip"),           CWT_PapayaWhip           }
	, { _T("PeachPuff"),            CWT_PeachPuff            }
	, { _T("Peru"),                 CWT_Peru                 }
	, { _T("Pink"),                 CWT_Pink                 }
	, { _T("Plum"),                 CWT_Plum                 }
	, { _T("PowderBlue"),           CWT_PowderBlue           }
	, { _T("Purple"),               CWT_Purple               }
	, { _T("Red"),                  CWT_Red                  }
	, { _T("RosyBrown"),            CWT_RosyBrown            }
	, { _T("RoyalBlue"),            CWT_RoyalBlue            }
	, { _T("SaddleBrown"),          CWT_SaddleBrown          }
	, { _T("Salmon"),               CWT_Salmon               }
	, { _T("SandyBrown"),           CWT_SandyBrown           }
	, { _T("SeaGreen"),             CWT_SeaGreen             }
	, { _T("SeaShell"),             CWT_SeaShell             }
	, { _T("Sienna"),               CWT_Sienna               }
	, { _T("Silver"),               CWT_Silver               }
	, { _T("SkyBlue"),              CWT_SkyBlue              }
	, { _T("SlateBlue"),            CWT_SlateBlue            }
	, { _T("SlateGray"),            CWT_SlateGray            }
	, { _T("SlateGrey"),            CWT_SlateGrey            }
	, { _T("Snow"),                 CWT_Snow                 }
	, { _T("SpringGreen"),          CWT_SpringGreen          }
	, { _T("SteelBlue"),            CWT_SteelBlue            }
	, { _T("Tan"),                  CWT_Tan                  }
	, { _T("Teal"),                 CWT_Teal                 }
	, { _T("Thistle"),              CWT_Thistle              }
	, { _T("Tomato"),               CWT_Tomato               }
	, { _T("Turquoise"),            CWT_Turquoise            }
	, { _T("Violet"),               CWT_Violet               }
	, { _T("Wheat"),                CWT_Wheat                }
	, { _T("White"),                CWT_White                }
	, { _T("WhiteSmoke"),           CWT_WhiteSmoke           }
	, { _T("Yellow"),               CWT_Yellow               }
	, { _T("YellowGreen"),          CWT_YellowGreen          }
};


void _cwt_colorx_init(void)
{
	CwtStrNS *strNS = cwtStrNS();
	size_t i;
	size_t n;

	__cwt_color_table = hash_table_new(string_hash, string_equal);
	hash_table_register_free_functions(__cwt_color_table, cwtFree, NULL);

	n = sizeof(__cwt_color_names)/sizeof(__cwt_color_names[0]);
	for( i = 0; i < n; i++  ) {
		CWT_CHAR *key = strNS->strdup(__cwt_color_names[i].name);
		strNS->tolowerStr(key, key, strNS->strlen(key));
		CWT_ASSERT(hash_table_insert(__cwt_color_table
			, key, &__cwt_color_names[i].color));
	}
}

void _cwt_colorx_release(void)
{
	CWT_ASSERT(__cwt_color_table);
	hash_table_free(__cwt_color_table);
}


Cwt_Color cwt_stylex_colorByName(const CWT_CHAR *name)
{
	CwtStrNS *strNS = cwtStrNS();
	CWT_CHAR *key;
	Cwt_Color *pcolor;

	CWT_ASSERT(name);
	key = strNS->strdup(name);
	strNS->tolowerStr(key, key, strNS->strlen(key));

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
BOOL cwt_stylex_parseColor(const CWT_CHAR *str, Cwt_Color *pcolor)
{
	CwtStrNS *strNS = cwtStrNS();
	BOOL ok = FALSE;
	Cwt_Color color;

	if( str && strNS->strlen(str) > 0 ) {
		if( *str == _T('#') ) {
			str++;
			color = strNS->toUINT32(str, 16, &ok);
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

	CWT_TEST_OK(cwt_stylex_parseColor(_T("Blue"), &color));
	CWT_TEST_OK(color == CWT_Blue);
	CWT_TEST_OK(cwt_stylex_parseColor(_T("#7FFF00"), &color));
	CWT_TEST_OK(color == CWT_Chartreuse);
	CWT_TEST_OK(cwt_stylex_parseColor(_T("#000000"), &color));
	CWT_TEST_OK(color == CWT_Black);
	CWT_TEST_OK(cwt_stylex_parseColor(_T("#FFFFFF"), &color));
	CWT_TEST_OK(color == CWT_White);
	CWT_TEST_NOK(cwt_stylex_parseColor(_T("7FFF00"), &color));        /* prefix '#' is omit */
	CWT_TEST_NOK(cwt_stylex_parseColor(_T("#FFFFFFFFFFFF"), &color)); /* too big value*/
	CWT_TEST_NOK(cwt_stylex_parseColor(_T("#QWERTY"), &color));       /* is not a hex value */
	CWT_TEST_NOK(cwt_stylex_parseColor(_T("#1000000"), &color));      /* greater than MaxColor value */
}
#endif
