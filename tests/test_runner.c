#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/ucg.h"
#include "test_data.c"

int main(int argc, const char** argv) {
    (void)argc;
    (void)argv;

	ucg_int failed = 0;
	ucg_int completed = 0;

	printf("Running official grapheme break tests ...\n");
	for (ucg_int i = 0; i < (ucg_int)(sizeof(official_grapheme_break_test_cases) / sizeof(test_case)); i += 1) {
		test_case t = official_grapheme_break_test_cases[i];

		ucg_int grapheme_count;
		ucg_int result = ucg_grapheme_count((uint8_t*)t.str, (ucg_int)strlen(t.str), NULL, &grapheme_count, NULL);
		if (result != 0) {
			fprintf(stderr, "(#% 4li) failed due to UTF-8 parsing error: %li\n", i, result);
		}
		if (grapheme_count != t.expected_clusters) {
			fprintf(stderr, "(#% 4li) graphemes: %li != %li, %s\n", i, grapheme_count, t.expected_clusters, t.str);
			failed += 1;
		}
		completed += 1;
	}

	printf("Running official emoji tests ...\n");
	for (ucg_int i = 0; i < (ucg_int)(sizeof(official_emoji_test_cases) / sizeof(test_case)); i += 1) {
		test_case t = official_emoji_test_cases[i];
		ucg_int grapheme_count;
		ucg_int result = ucg_grapheme_count((uint8_t*)t.str, (ucg_int)strlen(t.str), NULL, &grapheme_count, NULL);
		if (result != 0) {
			fprintf(stderr, "(#% 4li) failed due to UTF-8 parsing error: %li\n", i, result);
		}
		if (grapheme_count != t.expected_clusters) {
			fprintf(stderr, "(#% 4li) graphemes: %li != %li, %s\n", i, grapheme_count, t.expected_clusters, t.str);
			failed += 1;
		}
		completed += 1;
	}

	fprintf(stderr, "Tests failed: % 4li/% 4li\n", failed, completed);

	return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
