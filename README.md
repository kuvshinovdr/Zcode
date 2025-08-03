# Zcode

## Purpose

A small programming language that may be possibly used as a scripting language in C++-programs. 
The original idea was to invent another version of simplified and streamlined C and then augment it with new features.
It's hard to consistently follow that principle, though.


## Properties

* A program is an infix-form expression.
* Static typing with possibly dynamic checks for enabling contracts.
* Simple static type inference.


## Lexemes

Lexemes are defined in a way to enable lexing all lines in parallel.

### Comments

Comments start with // and end with the new-line character (one-line comments only):

```
not a comment // A comment
not a comment
```

### Identifiers

Identifiers must start with a "letter" or _ and then may also contain digit characters.
Identifiers containing two consequent underscores `__` are reserved for compiler/runtime needs.
Defining such identifiers in user code is a compilation error.

### Strings

Strings may be presented by "one-liners", which can not contain new-line characters, and "multi-liners".
Both can be of two subtypes: with or without escape sequences.

One-liner without escape sequences:

```
'this is a string literal, it can not contain apostrophes and new-line characters'
```

One-liner with escape sequences is more C-like:

```
"So we can escape \\ and \" or \n as\tusual"
```

Supported escape sequences:

* `\\` for backslash \\;
* `\"` for quote mark ";
* `\t` for horizontal tabulation;
* `\r` for carriage return;
* `\n` for new line;
* `\0` for NUL character (code 0);
* `\xh` or `\xhh`, where h is a hexadecimal digit, for a byte with a specified value;
* `\uhhhh` for a Unicode codepoint (either four or eight hexadecimal digits);
* `\{...}` for string interpolation, but nested interpolation and new-lines between \{ and \} are forbidden!

Multi-liner without escape sequences occupies separate lines each starting with `\ ` (backslash and space).
New lines are counted as a part of the string:

```
\ This is a multi-liner. It can contain ' or " or \ just like
\ ordinary characters.
```

Multi-liner with escape sequences occupies separate lines each starting with `\\ ` (two backslashes and a space) and can contain the same escape sequences as `"..."` one-liner can:

```
\\ This is a multi-liner with escapes. New lines are still part of the string.
\\ Such literal can contain sequences like \\\\ and \" (although " is ok as well)
\\ and interpolation \{6*7} too.
```

String literals have special compiler-time type or (**literal type**) `Literal.String`.
By default it converts to a UTF-8 encoded byte-array (without BOM).

### Characters

A character represent a Unicode codepoint.
The way do write it is using a backquote and a character (single Unicode codepoint!) or an escape-sequence:

* ``` `a ``` U+0065, small Latin a;
* ``` `` ``` backquote symbol, U+0060, being a prefix, we don't need to escape it;
* escape sequences: ``` `\\ ```, ``` `\t ```, ``` `\r ```, ``` `\n ```, ``` `\0 ```, ``` `\xhh ``` (one or two hex digits), ``` `\uhhhh ``` (four or eight hex digigs);
* ``` `😀 ``` U+1F600, etc.

### Numbers

A number may start with optional base prefix of the following form:

* `#` hexadecimal prefix;
* `n#` base-n prefix, where n may be 2, 4 or 8.

Thus number literals may be encoded in base-2 (bits), base-4 (e.g. nucleotides), base-8 (PDP-8 legacy), base-10 (the default option with no prefix) or base-16 (nibbles, using abcdef/ABCDEF as additional digits).

Apostrophe `'` 
