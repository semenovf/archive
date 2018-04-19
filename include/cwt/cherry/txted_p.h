/*
 * txted_p.h
 *
 *  Created on: 12.05.2012
 *      Author: user
 */

#ifndef _CWT_TXTED_P_H_
#define _CWT_TXTED_P_H_

EXTERN_C_BEGIN

CwtTextBlockXPtr _cwt_new_textblockx(void);
void _cwt_delete_textblockx(CwtTextBlockXPtr block);

CwtTextDocumentPtr _cwt_new_textdoc(void);
void _cwt_delete_textdoc(CwtTextDocumentPtr doc);
void _cwt_textdoc_set_plaintext(CwtTextDocumentPtr doc, const CWT_CHAR *text, size_t len);
void _cwt_textdoc_insert_plaintext(CwtTextDocumentPtr doc, const CWT_CHAR *text, size_t len, size_t pos);
CwtTextBlockXPtr _cwt_textdoc_first_block(const CwtTextDocumentPtr doc);
CwtTextBlockXPtr _cwt_textdoc_last_block(const CwtTextDocumentPtr doc);
CwtTextBlockXPtr _cwt_textdoc_find_block(const CwtTextDocumentPtr doc, size_t pos);
size_t _cwt_textdoc_block_count(const CwtTextDocumentPtr doc);
size_t _cwt_textdoc_line_count(const CwtTextDocumentPtr doc);

void _cwt_init_textedit(CwtWidgetPtr widget);
void _cwt_destroy_textedit(CwtWidgetPtr widget);
void _cwt_render_textedit(CwtWidgetPtr widget);
/*
void _cwt_destroy_reachview(CwtWidgetPtr widget);
CwtWidgetPtr _cwt_clone_reachview(CwtWidgetPtr orig, CwtWidgetPtr clone);
void _cwt_render_reachview(CwtWidgetPtr widget);
void _cwt_size_min_reachview(CwtWidgetPtr widget, CWT_SIZE *sz);
*/


EXTERN_C_END

#endif /* _CWT_TXTED_P_H_ */
