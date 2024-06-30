#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/ucg.h"
#include "test_data.c"

int main(int argc, const char** argv) {
    (void)argc;
    (void)argv;

	int failed = 0;
	int completed = 0;

	printf("Running official grapheme break tests ...\n");
	for (int i = 0; i < (int)(sizeof(official_grapheme_break_test_cases) / sizeof(test_case)); i += 1) {
		test_case t = official_grapheme_break_test_cases[i];
		int grapheme_count;
		int result = ucg_grapheme_count((uint8_t*)t.str, strlen(t.str), NULL, &grapheme_count, NULL);
		if (result != 0) {
			printf("(#% 4i) failed due to UTF-8 parsing error: %i\n", i, result);
		}
		if (grapheme_count != t.expected_clusters) {
			printf("(#% 4i) graphemes: %i != %i, %s\n", i, grapheme_count, t.expected_clusters, t.str);
			failed += 1;
		}
		completed += 1;
	}

	printf("Running official emoji tests ...\n");
	for (int i = 0; i < (int)(sizeof(official_emoji_test_cases) / sizeof(test_case)); i += 1) {
		test_case t = official_emoji_test_cases[i];
		int grapheme_count;
		int result = ucg_grapheme_count((uint8_t*)t.str, strlen(t.str), NULL, &grapheme_count, NULL);
		if (result != 0) {
			printf("(#% 4i) failed due to UTF-8 parsing error: %i\n", i, result);
		}
		if (grapheme_count != t.expected_clusters) {
			printf("(#% 4i) graphemes: %i != %i, %s\n", i, grapheme_count, t.expected_clusters, t.str);
			failed += 1;
		}
		completed += 1;
	}

	printf("Tests failed: % 4i/% 4i\n", failed, completed);

	return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
