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
  def initialize(keys)
    @keys = keys
  end

  def to_s
    render(LEFT) + "   " + render(RIGHT)
  end

  def comfort
    features = [
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

  def include?(finger)
    @keys.include?(left(finger)) || @keys.include?(right(finger))
  end

  def b_to_i(bool)
    bool ? 1 : 0
  end

  def render(hand)
    hand.map do |finger|
      @keys.include?(finger) ? " O " : " . "
    end.join
  end
end

chords = (1..3).to_enum.map do |key_count|
  (0..9).to_a.combination(key_count).map do |chord|
    Chord.new(chord)
  end
end.flatten.sort_by(&:comfort).reverse

chords.each.with_index do |chord, i|
  puts "#{i.to_s.rjust(3, ' ')}  #{chord} #{chord.comfort.to_s.rjust(16, ' ')}"
end
