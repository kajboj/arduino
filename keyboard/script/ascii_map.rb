SHIFT = 0x80
ASCII_MAP = {
  'a'           => [ 4, false ],
  'b'           => [ 5, false ],
  'c'           => [ 6, false ],
  'd'           => [ 7, false ],
  'e'           => [ 8, false ],
  'f'           => [ 9, false ],
  'g'           => [ 10, false ],
  'h'           => [ 11, false ],
  'i'           => [ 12, false ],
  'j'           => [ 13, false ],
  'k'           => [ 14, false ],
  'l'           => [ 15, false ],
  'm'           => [ 16, false ],
  'n'           => [ 17, false ],
  'o'           => [ 18, false ],
  'p'           => [ 19, false ],
  'q'           => [ 20, false ],
  'r'           => [ 21, false ],
  's'           => [ 22, false ],
  't'           => [ 23, false ],
  'u'           => [ 24, false ],
  'v'           => [ 25, false ],
  'w'           => [ 26, false ],
  'x'           => [ 27, false ],
  'y'           => [ 28, false ],
  'z'           => [ 29, false ],
  '1'           => [ 30, false ],
  '!'           => [ 30, true  ],
  '2'           => [ 31, false ],
  '@'           => [ 31, true  ],
  '3'           => [ 32, false ],
  '#'           => [ 32, true  ],
  '4'           => [ 33, false ],
  '$'           => [ 33, true  ],
  '5'           => [ 34, false ],
  '%'           => [ 34, true  ],
  '6'           => [ 35, false ],
  '^'           => [ 35, true  ],
  '7'           => [ 36, false ],
  '&'           => [ 36, true  ],
  '8'           => [ 37, false ],
  '*'           => [ 37, true  ],
  '9'           => [ 38, false ],
  '('           => [ 38, true  ],
  '0'           => [ 39, false ],
  ')'           => [ 39, true  ],
  'RETURN'      => [ 40, false ],
  'ESC'         => [ 41, false ],
  'BACKSPACE'   => [ 42, false ],
  'TAB'         => [ 43, false ],
  ' '           => [ 44, false ],
  '-'           => [ 45, false ],
  '_'           => [ 45, true  ],
  '='           => [ 46, false ],
  '+'           => [ 46, true  ],
  '['           => [ 47, false ],
  '{'           => [ 47, true  ],
  ']'           => [ 48, false ],
  '}'           => [ 48, true  ],
  '\\'          => [ 49, false ],
  '|'           => [ 49, true  ],
  ';'           => [ 51, false ],
  ':'           => [ 51, true  ],
  '\''          => [ 52, false ],
  '"'           => [ 52, true  ],
  '`'           => [ 53, false ],
  '~'           => [ 53, true  ],
  ','           => [ 54, false ],
  '<'           => [ 54, true  ],
  '.'           => [ 55, false ],
  '>'           => [ 55, true  ],
  '/'           => [ 56, false ],
  '?'           => [ 56, true  ],
  'CAPS_LOCK'   => [ 57, false ],
  'F1'          => [ 58, false ],
  'F2'          => [ 59, false ],
  'F3'          => [ 60, false ],
  'F4'          => [ 61, false ],
  'F5'          => [ 62, false ],
  'F6'          => [ 63, false ],
  'F7'          => [ 64, false ],
  'F8'          => [ 65, false ],
  'F9'          => [ 66, false ],
  'F10'         => [ 67, false ],
  'F11'         => [ 68, false ],
  'F12'         => [ 69, false ],

  'INSERT'      => [ 73, false ],
  'HOME'        => [ 74, false ],
  'PAGE_UP'     => [ 75, false ],
  'DELETE'      => [ 76, false ],
  'END'         => [ 77, false ],
  'PAGE_DOWN'   => [ 78, false ],
  'RIGHT_ARROW' => [ 79, false ],
  'LEFT_ARROW'  => [ 80, false ],
  'DOWN_ARROW'  => [ 81, false ],
  'UP_ARROW'    => [ 82, false ],
}

ASCII_MAP_UK = ASCII_MAP.merge(
  '@'  => [ 52,  true  ],
  '"'  => [ 31,  true  ],
  '¬'  => [ 53,  true  ],
  '£'  => [ 32,  true  ],
  '\\' => [ 100, false ],
  '|'  => [ 100, true  ],
  '#'  => [ 50,  false ],
  '~'  => [ 50,  true  ],
)
