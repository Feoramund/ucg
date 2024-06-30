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

/* For the sake of compatibility, the general-purpose integers used throughout
 * this library are defined to be at least 32-bits large but may be larger. */
typedef int_fast32_t ucg_int;

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
extern const ucg_allocator ucg_default_allocator;

/* This is the data that is allocated when an allocator is passed to
 * ucg_decode_grapheme_clusters. */
typedef struct {
	ucg_int byte_index;
	ucg_int rune_index;
	ucg_int width;
} ucg_grapheme;

/* This procedure merely counts the runes, graphemes, and width without
 * incurring any allocations. It is a thin wrapper over
 * ucg_decode_grapheme_clusters that passes NULL for an allocator. */
ucg_int ucg_grapheme_count(
	const uint8_t* str,
	ucg_int str_len,

	ucg_int* out_runes,
	ucg_int* out_graphemes,
	ucg_int* out_width);

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
ucg_int ucg_decode_grapheme_clusters(
	ucg_allocator* allocator,
	const uint8_t* str,
	ucg_int str_len,

	ucg_grapheme** out_graphemes,
	ucg_int* out_rune_count,
	ucg_int* out_grapheme_count,
	ucg_int* out_width);


/* These procedures are part of how UCG decodes graphemes, and as such, they are
 * made public here in the event that they are useful. */

typedef ucg_int ucg_rune;

#define UCG_EOF                 (-1)
#define UCG_EXPECTED_MORE_BYTES (-2)
#define UCG_INVALID_RUNE        (-3)

/* This procedure decodes a byte string and returns a valid Unicode codepoint or
 * one of the errors above. The byte iterator is increased as needed while
 * reading the string. */
ucg_rune ucg_decode_rune(const uint8_t* str, ucg_int str_len, ucg_int* byte_iterator);

/* The following procedures all return true or false based on whether a Unicode
 * codepoint fits into a certain class. */

bool ucg_is_control                         (ucg_rune r);
bool ucg_is_emoji_modifier                  (ucg_rune r);
bool ucg_is_regional_indicator              (ucg_rune r);
bool ucg_is_enclosing_mark                  (ucg_rune r);
bool ucg_is_prepended_concatenation_mark    (ucg_rune r);
bool ucg_is_spacing_mark                    (ucg_rune r);
bool ucg_is_nonspacing_mark                 (ucg_rune r);
bool ucg_is_emoji_extended_pictographic     (ucg_rune r);
bool ucg_is_grapheme_extend                 (ucg_rune r);
bool ucg_is_hangul_syllable_leading         (ucg_rune r);
bool ucg_is_hangul_syllable_vowel           (ucg_rune r);
bool ucg_is_hangul_syllable_trailing        (ucg_rune r);
bool ucg_is_hangul_syllable_lv              (ucg_rune r);
bool ucg_is_hangul_syllable_lvt             (ucg_rune r);
bool ucg_is_indic_consonant_preceding_repha (ucg_rune r);
bool ucg_is_indic_consonant_prefixed        (ucg_rune r);
bool ucg_is_indic_conjunct_break_linker     (ucg_rune r);
bool ucg_is_indic_conjunct_break_consonant  (ucg_rune r);
bool ucg_is_indic_conjunct_break_extend     (ucg_rune r);
bool ucg_is_gcb_prepend_class               (ucg_rune r);
bool ucg_is_gcb_extend_class                (ucg_rune r);

#ifdef __cplusplus
}
#endif

#endif /* _UCG_INCLUDED */
