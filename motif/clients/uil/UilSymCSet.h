/*
**	This file is automatically generated.  Do not edit it by hand.
**/

/*
 * Character set XmString name table, accessed by sym_k_..._charset code.
 * Each entry is the name which identifies the character set in a XmString.
 */
static char *charset_xmstring_names_table_vec[] =
  {
    0,
    "<userdefined>",
    "GB2312.1980-0",	/* gb_hanzi */
    "GB2312.1980-1",	/* gb_hanzi_gr */
    "ISO8859-6",	/* iso_arabic */
    "ISO8859-5",	/* iso_cyrillic */
    "ISO8859-7",	/* iso_greek */
    "ISO8859-8",	/* iso_hebrew */
    "ISO8859-8",	/* iso_hebrew_lr */
    "ISO8859-1",	/* iso_latin1 */
    "ISO8859-2",	/* iso_latin2 */
    "ISO8859-3",	/* iso_latin3 */
    "ISO8859-4",	/* iso_latin4 */
    "ISO8859-9",	/* iso_latin5 */
    "JISX0208.1983-0",	/* jis_kanji */
    "JISX0208.1983-1",	/* jis_kanji_gr */
    "JISX0201.1976-0",	/* jis_katakana */
    "KSC5601.1987-0",	/* ksc_hangul */
    "KSC5601.1987-1",	/* ksc_hangul_gr */
};
externaldef(uil_sym_glbl) char **charset_xmstring_names_table =
		charset_xmstring_names_table_vec;
/*
 * Character set writing direction table, accessed by sym_k_..._charset code.
 * Each entry is the XmSTRING_DIRECTION_... code which identifies the
 * writing direction for the character set in a XmString.
 */
static unsigned char charset_wrdirection_table_vec[] =
  {
    0,
    0,	/* userdefined */
    XmSTRING_DIRECTION_L_TO_R,	/* gb_hanzi */
    XmSTRING_DIRECTION_L_TO_R,	/* gb_hanzi_gr */
    XmSTRING_DIRECTION_L_TO_R,	/* iso_arabic */
    XmSTRING_DIRECTION_L_TO_R,	/* iso_cyrillic */
    XmSTRING_DIRECTION_L_TO_R,	/* iso_greek */
    XmSTRING_DIRECTION_R_TO_L,	/* iso_hebrew */
    XmSTRING_DIRECTION_R_TO_L,	/* iso_hebrew_lr */
    XmSTRING_DIRECTION_L_TO_R,	/* iso_latin1 */
    XmSTRING_DIRECTION_L_TO_R,	/* iso_latin2 */
    XmSTRING_DIRECTION_L_TO_R,	/* iso_latin3 */
    XmSTRING_DIRECTION_L_TO_R,	/* iso_latin4 */
    XmSTRING_DIRECTION_L_TO_R,	/* iso_latin5 */
    XmSTRING_DIRECTION_L_TO_R,	/* jis_kanji */
    XmSTRING_DIRECTION_L_TO_R,	/* jis_kanji_gr */
    XmSTRING_DIRECTION_L_TO_R,	/* jis_katakana */
    XmSTRING_DIRECTION_L_TO_R,	/* ksc_hangul */
    XmSTRING_DIRECTION_L_TO_R,	/* ksc_hangul_gr */
};
externaldef(uil_sym_glbl) unsigned char *charset_writing_direction_table =
		charset_wrdirection_table_vec;
/*
 * Character set parsing direction table, accessed by sym_k_..._charset code.
 * Each entry is the XmSTRING_DIRECTION_... code which identifies the
 * parsing direction for the character set in a XmString.
 */
static unsigned char charset_parsdirection_table_vec[] =
  {
    0,
    0,	/* userdefined */
    XmSTRING_DIRECTION_L_TO_R,	/* gb_hanzi */
    XmSTRING_DIRECTION_L_TO_R,	/* gb_hanzi_gr */
    XmSTRING_DIRECTION_L_TO_R,	/* iso_arabic */
    XmSTRING_DIRECTION_L_TO_R,	/* iso_cyrillic */
    XmSTRING_DIRECTION_L_TO_R,	/* iso_greek */
    XmSTRING_DIRECTION_R_TO_L,	/* iso_hebrew */
    XmSTRING_DIRECTION_L_TO_R,	/* iso_hebrew_lr */
    XmSTRING_DIRECTION_L_TO_R,	/* iso_latin1 */
    XmSTRING_DIRECTION_L_TO_R,	/* iso_latin2 */
    XmSTRING_DIRECTION_L_TO_R,	/* iso_latin3 */
    XmSTRING_DIRECTION_L_TO_R,	/* iso_latin4 */
    XmSTRING_DIRECTION_L_TO_R,	/* iso_latin5 */
    XmSTRING_DIRECTION_L_TO_R,	/* jis_kanji */
    XmSTRING_DIRECTION_L_TO_R,	/* jis_kanji_gr */
    XmSTRING_DIRECTION_L_TO_R,	/* jis_katakana */
    XmSTRING_DIRECTION_L_TO_R,	/* ksc_hangul */
    XmSTRING_DIRECTION_L_TO_R,	/* ksc_hangul_gr */
};
externaldef(uil_sym_glbl) unsigned char *charset_parsing_direction_table =
		charset_parsdirection_table_vec;
/*
 * Character set character size table, accessed by sym_k_..._charset code.
 * Each entry is the sym_k_..._charsize literal which names the character
 * size for the character set in a XmString.
 */
static unsigned char charset_charsize_table_vec[] =
  {
    0,
    0,	/* userdefined */
    sym_k_twobyte_charsize,	/* gb_hanzi */
    sym_k_twobyte_charsize,	/* gb_hanzi_gr */
    sym_k_onebyte_charsize,	/* iso_arabic */
    sym_k_onebyte_charsize,	/* iso_cyrillic */
    sym_k_onebyte_charsize,	/* iso_greek */
    sym_k_onebyte_charsize,	/* iso_hebrew */
    sym_k_onebyte_charsize,	/* iso_hebrew_lr */
    sym_k_onebyte_charsize,	/* iso_latin1 */
    sym_k_onebyte_charsize,	/* iso_latin2 */
    sym_k_onebyte_charsize,	/* iso_latin3 */
    sym_k_onebyte_charsize,	/* iso_latin4 */
    sym_k_onebyte_charsize,	/* iso_latin5 */
    sym_k_twobyte_charsize,	/* jis_kanji */
    sym_k_twobyte_charsize,	/* jis_kanji_gr */
    sym_k_onebyte_charsize,	/* jis_katakana */
    sym_k_twobyte_charsize,	/* ksc_hangul */
    sym_k_twobyte_charsize,	/* ksc_hangul_gr */
};
externaldef(uil_sym_glbl) unsigned char *charset_character_size_table =
		charset_charsize_table_vec;
/*
 * All the names under which a character set may be legally named in a 
 * $LANG variable (upper case).
 */
static char *charset_lang_names_table_vec[] =
  {
    "GB_HANZI",	/* gb_hanzi */
    "GB2312.1980-0",	/* gb_hanzi */
    "GB_CHINESE",	/* gb_hanzi */
    "GB_HANZI_GL",	/* gb_hanzi */
    "GB_HANZI_GR",	/* gb_hanzi_gr */
    "GB2312.1980-1",	/* gb_hanzi_gr */
    "ISO_ARABIC",	/* iso_arabic */
    "ISO8859-6",	/* iso_arabic */
    "ISO_LATIN6",	/* iso_arabic */
    "ISOLATIN6",	/* iso_arabic */
    "ISO_CYRILLIC",	/* iso_cyrillic */
    "ISO8859-5",	/* iso_cyrillic */
    "ISO_GREEK",	/* iso_greek */
    "ISO8859-7",	/* iso_greek */
    "ISO_LATIN7",	/* iso_greek */
    "ISOLATIN7",	/* iso_greek */
    "ISO_HEBREW",	/* iso_hebrew */
    "ISO8859-8",	/* iso_hebrew */
    "ISO_LATIN8",	/* iso_hebrew */
    "ISOLATIN8",	/* iso_hebrew */
    "ISO_HEBREW_LR",	/* iso_hebrew_lr */
    "ISO8859-8",	/* iso_hebrew_lr */
    "ISO_LATIN8_LR",	/* iso_hebrew_lr */
    "ISO_LATIN1",	/* iso_latin1 */
    "ISO8859-1",	/* iso_latin1 */
    "ISOLATIN1",	/* iso_latin1 */
    "ISO_LATIN2",	/* iso_latin2 */
    "ISO8859-2",	/* iso_latin2 */
    "ISOLATIN2",	/* iso_latin2 */
    "ISO_LATIN3",	/* iso_latin3 */
    "ISO8859-3",	/* iso_latin3 */
    "ISOLATIN3",	/* iso_latin3 */
    "ISO_LATIN4",	/* iso_latin4 */
    "ISO8859-4",	/* iso_latin4 */
    "ISOLATIN4",	/* iso_latin4 */
    "ISO_LATIN5",	/* iso_latin5 */
    "ISO8859-9",	/* iso_latin5 */
    "ISOLATIN5",	/* iso_latin5 */
    "JIS_KANJI",	/* jis_kanji */
    "JISX0208.1983-0",	/* jis_kanji */
    "JIS_JAPANESE",	/* jis_kanji */
    "JIS_KANJI_GL",	/* jis_kanji */
    "JIS_KANJI_GR",	/* jis_kanji_gr */
    "JISX0208.1983-1",	/* jis_kanji_gr */
    "JIS_KATAKANA",	/* jis_katakana */
    "JISX0201.1976-0",	/* jis_katakana */
    "KSC_HANGUL",	/* ksc_hangul */
    "KSC5601.1987-0",	/* ksc_hangul */
    "KSC_KOREAN",	/* ksc_hangul */
    "KSC_HANGUL_GL",	/* ksc_hangul */
    "KSC_HANGUL_GR",	/* ksc_hangul_gr */
    "KSC5601.1987-1",	/* ksc_hangul_gr */
};
externaldef(uil_sym_glbl) char **charset_lang_names_table =
		charset_lang_names_table_vec;
/*
 * The sym_k_..._charset codes for charset_lang_names
 */
static unsigned short int charset_lang_codes_table_vec[] =
  {
    sym_k_gb_hanzi_charset,
    sym_k_gb_hanzi_charset,
    sym_k_gb_hanzi_charset,
    sym_k_gb_hanzi_charset,
    sym_k_gb_hanzi_gr_charset,
    sym_k_gb_hanzi_gr_charset,
    sym_k_iso_arabic_charset,
    sym_k_iso_arabic_charset,
    sym_k_iso_arabic_charset,
    sym_k_iso_arabic_charset,
    sym_k_iso_cyrillic_charset,
    sym_k_iso_cyrillic_charset,
    sym_k_iso_greek_charset,
    sym_k_iso_greek_charset,
    sym_k_iso_greek_charset,
    sym_k_iso_greek_charset,
    sym_k_iso_hebrew_charset,
    sym_k_iso_hebrew_charset,
    sym_k_iso_hebrew_charset,
    sym_k_iso_hebrew_charset,
    sym_k_iso_hebrew_lr_charset,
    sym_k_iso_hebrew_lr_charset,
    sym_k_iso_hebrew_lr_charset,
    sym_k_iso_latin1_charset,
    sym_k_iso_latin1_charset,
    sym_k_iso_latin1_charset,
    sym_k_iso_latin2_charset,
    sym_k_iso_latin2_charset,
    sym_k_iso_latin2_charset,
    sym_k_iso_latin3_charset,
    sym_k_iso_latin3_charset,
    sym_k_iso_latin3_charset,
    sym_k_iso_latin4_charset,
    sym_k_iso_latin4_charset,
    sym_k_iso_latin4_charset,
    sym_k_iso_latin5_charset,
    sym_k_iso_latin5_charset,
    sym_k_iso_latin5_charset,
    sym_k_jis_kanji_charset,
    sym_k_jis_kanji_charset,
    sym_k_jis_kanji_charset,
    sym_k_jis_kanji_charset,
    sym_k_jis_kanji_gr_charset,
    sym_k_jis_kanji_gr_charset,
    sym_k_jis_katakana_charset,
    sym_k_jis_katakana_charset,
    sym_k_ksc_hangul_charset,
    sym_k_ksc_hangul_charset,
    sym_k_ksc_hangul_charset,
    sym_k_ksc_hangul_charset,
    sym_k_ksc_hangul_gr_charset,
    sym_k_ksc_hangul_gr_charset,
};
externaldef(uil_sym_glbl) unsigned short int *charset_lang_codes_table =
		charset_lang_codes_table_vec;
/*
 * The number of entries in charset_lang_..._table tables
 */
externaldef(uil_sym_glbl) unsigned short int charset_lang_table_max = 52;
