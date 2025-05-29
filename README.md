# Zcode

## Purpose

A small programming language that may be possibly used as a scripting language in C++-programs. 

## Properties

* A program is an infix-form expression.
* Static typing with possibly dynamic checks for enabling contracts.
* Simple static type inference.

## Lexemes

### Comments

Comments start with // and end with the new-line character (one-line comments only):

```
not a comment // A comment
not a comment
```

### Identifiers

Identifiers must start with a "letter" or _ and then may also contain digit characters.
Identifiers containing two consequent underscores `__` are reserved for compiler/runtime needs.

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
* `\{...}` for string interpolation

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