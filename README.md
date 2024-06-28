# UCG

**UCG** **C**ounts **G**raphemes. That is its one job, and it does it well.

In short, UCG is a C99 implementation of grapheme segmentation from Unicode®
Standard Annex #29 and width calculation from Annex #11.

More simply put, it counts how many runes and graphemes are in UTF-8 encoded
text and calculates how many monospace cells wide the text should be. Runes are
Unicode codepoints, as opposed to individual bytes. Graphemes are individual
units of a writing system, which can be composed of many runes, such as
combining diacritics or emoji modifiers.

It is designed to be simple and easy to use for other projects that need to
segment text by grapheme cluster boundaries or calculate monospace width for
terminal output.

## Conformity

UCG passes the official Unicode test suite combined of 1,187 grapheme-based and
3,648 emoji-based test cases. It has been implemented based off of the Unicode
version 15.1.0 specification, which was published in 2023.

## Performance

UCG has not been thoroughly optimized. There are opportunities for it, but it's
able to parse a file of nearly a million random Unicode codepoints in ~0.05
seconds on my machine with `O2` optimization using the Clang C compiler. It
parses a million random 7-bit ASCII characters in about ~0.01 seconds. This is
hopefully fast enough for general-purpose use.

## Building

UCG has no external dependencies. Drop right into your project and include.

## License

UCG is licensed under the permissive BSD-3-Clause license.

## References

- https://www.unicode.org/reports/tr29/#Grapheme_Cluster_Boundaries
- https://www.unicode.org/reports/tr29/#Default_Grapheme_Cluster_Table
- https://www.unicode.org/reports/tr29/#Grapheme_Cluster_Boundary_Rules
- https://www.unicode.org/reports/tr29/#Conformance
- https://www.unicode.org/reports/tr11/
