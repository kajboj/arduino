require 'set'

FINGERS = {
  left: {
    pinky:  0,
    ring:   1,
    middle: 2,
    index:  3,
    thumb:  4
  },
  right: {
    thumb:  5,
    index:  6,
    middle: 7,
    ring:   8,
    pinky:  9
  }
}

LEFT  = FINGERS[:left].values
RIGHT = FINGERS[:right].values

def left(finger)
  FINGERS[:left][finger]
end

def right(finger)
  FINGERS[:right][finger]
end

class Chord
  attr_reader :keys
  def initialize(keys)
    @keys = keys
  end

  def to_s
    render(LEFT) + " " + render(RIGHT)
  end

  def ==(other)
    @keys.sort == other.keys.sort
  end

  def comfort
    features = [
      possible?,
      one? && !include?(:pinky),
      two? && !include?(:pinky),
      one? && include?(:pinky),
      two? && include?(:pinky),
      !one_hand?,
      include?(:index),
      include?(:middle),
      include?(:thumb),
      include?(:ring),
      include?(:pinky)
    ]

    rank = 10**features.size
    features.inject(0) do |a, feature|
      a += b_to_i(feature) * rank
      rank = rank/10
      a
    end
  end

  private

  def possible?
    !(
      (
        include_left?(:pinky) &&
        include_left?(:middle) &&
        !include_left?(:ring)
      ) || (
        include_right?(:pinky) &&
        include_right?(:middle) &&
        !include_right?(:ring)
      )
    )
  end

  def any?(keys)
    (@keys - keys).size < @keys.size
  end

  def all?(keys)
    (@keys - keys).empty?
  end

  def one_hand?
    all?(LEFT) || all?(RIGHT)
  end

  def one?
    @keys.size == 1
  end

  def two?
    @keys.size == 2
  end

  def three?
    @keys.size == 3
  end

  def include_left?(finger)
    @keys.include?(left(finger))
  end

  def include_right?(finger)
    @keys.include?(right(finger))
  end

  def include?(finger)
    include_left?(finger) || include_right?(finger)
  end

  def b_to_i(bool)
    bool ? 1 : 0
  end

  def render(hand)
    hand.map do |finger|
      @keys.include?(finger) ? "O" : "."
    end.join
  end
end

class Mapping
  attr_reader :key

  def initialize(key, chord)
    @key   = key
    @chord = chord
  end

  def to_s
    @key.ljust(16, ' ') + @chord.to_s
  end
end

def parse(fingers)
  fs = fingers.gsub(' ', '').split('').map.with_index do |f, index|
    f == 'O' ? index : nil
  end.compact
  Chord.new(fs)
end

def subtract(a1, a2)
  result = []
  a1.each do |e|
    if !a2.find {|x| x == e}
      result << e
    end
  end
  result
end

# KEY_LEFT_CTRL
# KEY_LEFT_SHIFT
# KEY_LEFT_ALT
# KEY_LEFT_GUI

fixed = {
  'KEY_BACKSPACE'   => parse('..... .O...'),
  'KEY_RETURN'      => parse('...O. .O...'),
  'KEY_UP_ARROW'    => parse('..O.. ..O..'),
  'KEY_DOWN_ARROW'  => parse('..O.. .O...'),
  'KEY_LEFT_ARROW'  => parse('..OO. .....'),
  'KEY_RIGHT_ARROW' => parse('..O.. ...O.'),
  'KEY_ESC'         => parse('....O O....'),
  'KEY_TAB'         => parse('..O.O .....'),
  'KEY_PAGE_UP'     => parse('..O.O ..O..'),
  'KEY_PAGE_DOWN'   => parse('..O.O .O...'),
  'KEY_HOME'        => parse('..OOO .....'),
  'KEY_END'         => parse('..O.O ...O.'),
  ' '               => parse('....O .....'),
  '_'               => parse('....O .O...'),
  '('               => parse('O.... ..O..'),
  ')'               => parse('..O.. ....O'),
  '{'               => parse('..OO. O....'),
  '}'               => parse('....O .OO..'),
  '['               => parse('.O... .OO..'),
  ']'               => parse('..OO. ...O.'),
  '<'               => parse('..O.. OO...'),
  '>'               => parse('...OO ..O..'),
}

#'KEY_F1' =>
#'KEY_F2' =>
#'KEY_F3' =>
#'KEY_F4' =>
#'KEY_F5' =>
#'KEY_F6' =>
#'KEY_F7' =>
#'KEY_F8' =>
#'KEY_F9' =>
#'KEY_F10' =>
#'KEY_F11' =>
#'KEY_F12' =>

free = [
  'e', 't', 'a', 'o', 'i', 'n', 's', 'r', 'h', 'l', 'd', 'c', 'u', 'm', 'f',
  'g', 'p', 'y', 'w', 'b', ',', '.', 'v', 'k', '-', '"', '\'', 'x', ';', '0',
  'j', '1', 'q', '=', '2', ':', 'z', '/', '*', '!', '?', '$', '3', '5', '4',
  '9', '8', '6', '7', '\\', '+', '|', '&', '%', '@', '#', '^', '`', '~',
  'KEY_INSERT', 'KEY_DELETE',
]

chords = (1..3).to_enum.map do |key_count|
  (0..9).to_a.combination(key_count).map do |chord|
    Chord.new(chord)
  end
end.flatten.sort_by(&:comfort).reverse

free_chords = subtract(chords, fixed.values)

mappings = fixed.map do |key, chord|
  Mapping.new(key, chord)
end

mappings += free.zip(free_chords[0..free.size-1]).map do |key, chord|
  Mapping.new(key, chord)
end

def display_mappings(mappings)
  mappings.sort_by(&:key).each.with_index do |chord, i|
    puts "#{i.to_s.rjust(3, ' ')}  #{chord}"
  end
end

def display_chords(chords)
  chords.each.with_index do |chord, i|
    puts "#{i.to_s.rjust(3, ' ')}  #{chord} #{chord.comfort.to_s.rjust(16, ' ')}"
  end
end

display_chords(free_chords)
#display_mappings(mappings)
