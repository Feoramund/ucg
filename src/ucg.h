/*
 * SPDX-FileCopyrightText: (c) 2024 Feoramund
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _UCG_INCLUDED
#define _UCG_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* This is UCG's custom allocator structure, allowing you to specify precisely
 * how `ucg_decode_grapheme_clusters` allocates its memory.
 *
 * An arena or stack allocator that supports fast realloc is recommended. */
typedef struct {
	void* (*malloc)(intptr_t size, void* ctx);
	void* (*realloc)(void* ptr, intptr_t old_size, intptr_t new_size, void* ctx);
	void  (*free)(void* ptr, intptr_t size, void* ctx);

	void* ctx;
} ucg_allocator;

void *ucg_default_malloc(intptr_t size, void *ctx);
void *ucg_default_realloc(void* ptr, intptr_t old_size, intptr_t new_size, void *ctx);
void  ucg_default_free(void *ptr, intptr_t size, void *ctx);

/* This is the default allocator. Pass this if you don't mind using malloc. */
static ucg_allocator ucg_default_allocator = {
	ucg_default_malloc,
	ucg_default_realloc,
	ucg_default_free,
	NULL,
};

/* This is the data that is allocated when an allocator is passed to
 * ucg_decode_grapheme_clusters. */
typedef struct {
	int byte_index;
	int rune_index;
	int width;
} ucg_grapheme;

/* This procedure merely counts the runes, graphemes, and width without
 * incurring any allocations. It is a thin wrapper over
 * ucg_decode_grapheme_clusters that passes NULL for an allocator. */
int ucg_grapheme_count(
	const uint8_t* str,
	int str_len,

	int* out_runes,
	int* out_graphemes,
	int* out_width);

/* This is the heart of the library. If you want specific information about each
 * grapheme, you can pass an allocator and a pointer to a `grapheme*` variable
 * where the allocated data will be passed to.
 *
 * Otherwise, if NULL is passed for an allocator, no allocation will occur, and
 * no data will be written to `out_graphemes`.
 *
 * Each `out_` argument may be NULL to signify that you do not want the data,
 * except for `out_graphemes`. The presence of an allocator is used to determine
 * whether or not to allocate the extra data. Therefore, if an allocator is
 * passed, `out_graphemes` must point to a valid `grapheme*` variable.
 *
 * The return value is 0 on success or negative (one of the error values below)
 * if there was a trouble with parsing the string as UTF-8. */
int ucg_decode_grapheme_clusters(
	ucg_allocator* allocator,
	const uint8_t* str,
	int str_len,

	ucg_grapheme** out_graphemes,
	int* out_rune_count,
	int* out_grapheme_count,
	int* out_width);


/* These procedures are part of how UCG decodes graphemes, and as such, they are
 * made public here in the event that they are useful. */

#define UCG_EOF                 (-1)
#define UCG_EXPECTED_MORE_BYTES (-2)
#define UCG_INVALID_RUNE        (-3)

/* This procedure decodes a byte string and returns a valid Unicode codepoint or
 * one of the errors above. The byte iterator is increased as needed while
 * reading the string. */
int32_t ucg_decode_rune(const uint8_t* str, int str_len, int* byte_iterator);

/* The following procedures all return true or false based on whether a Unicode
 * codepoint fits into a certain class. */

bool ucg_is_control                         (int32_t r);
bool ucg_is_emoji_modifier                  (int32_t r);
bool ucg_is_regional_indicator              (int32_t r);
bool ucg_is_enclosing_mark                  (int32_t r);
bool ucg_is_prepended_concatenation_mark    (int32_t r);
bool ucg_is_spacing_mark                    (int32_t r);
bool ucg_is_nonspacing_mark                 (int32_t r);
bool ucg_is_emoji_extended_pictographic     (int32_t r);
bool ucg_is_grapheme_extend                 (int32_t r);
bool ucg_is_hangul_syllable_leading         (int32_t r);
bool ucg_is_hangul_syllable_vowel           (int32_t r);
bool ucg_is_hangul_syllable_trailing        (int32_t r);
bool ucg_is_hangul_syllable_lv              (int32_t r);
bool ucg_is_hangul_syllable_lvt             (int32_t r);
bool ucg_is_indic_consonant_preceding_repha (int32_t r);
bool ucg_is_indic_consonant_prefixed        (int32_t r);
bool ucg_is_indic_conjunct_break_linker     (int32_t r);
bool ucg_is_indic_conjunct_break_consonant  (int32_t r);
bool ucg_is_indic_conjunct_break_extend     (int32_t r);
bool ucg_is_gcb_prepend_class               (int32_t r);
bool ucg_is_gcb_extend_class                (int32_t r);

#ifdef __cplusplus
}
#endif

#endif /* _UCG_INCLUDED */
