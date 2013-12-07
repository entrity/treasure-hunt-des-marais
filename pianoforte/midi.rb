#! /usr/bin/env ruby
#
# This script looks for file ./sounds/*.mid, parses all of them and creates
# ./sounds/*.c. Each of the .c files holds code for creating an MjMidi::Song.
#
# If this file is run with a command line arg, that arg will be the directory
# acted upon instead of ./sounds
#
# This file holds a modified example, reader2text.rb, from the examples
# included with the midilib gem
# - Markham

require 'rubygems'
require 'midilib'

require 'midilib/sequence'
require 'midilib/io/midifile'

require 'pry'

DEFAULT_MIDI_TEST_FILE = 'data-short.mid'
DEFAULT_DIR = 'sounds'
$offset = 55

def run
  source_directory = ARGV[0] || DEFAULT_DIR
  puts "Searching for .mid files in #{source_directory}"
  Dir[File.join(source_directory, '*.mid')].each do |input_file|
    process_file(input_file)
  end
end

def process_file(input_file)
  @seq = MIDI::Sequence.new()
  output_file = input_file.sub(/.mid$/,'.c')
  $out = []
  # Input from midi file
  File::open(input_file,'rb') do |input|
    @seq.read(input){|track, num_tracks, i|
      puts "reading track #{i} of #{num_tracks}"
    }
  end
  # Merge all tracks into one
  (1...@seq.tracks.length).each do |i|
    @seq.tracks.first.merge @seq.tracks[i].events
  end
  @seq.tracks.first.recalc_times
  format_events @seq.tracks.first
  # Output to c file
  puts "writing to #{output_file}"
  File::open(output_file,"wb"){|output|
    song_name = File::basename(input_file,'.mid').gsub(/-/,'_')
    array_name = "#{song_name}_midiEvents"
    output << "MjMidi::event #{array_name}[] = {"
    output << $out.map{|x| "\n\t{#{x}}" }.join(",")
    output << "\n};"
    output.puts
    output.puts
    output.puts "MjMidi::Song #{song_name}(&#{array_name}[0], #{$out.length});"
  }
end

def format_events(track)
  track.events.each do |event|
    match = /(\d+): ch \d+ (on|off) (\w+) ?(\w*)/.match event.to_s
    if match
      offset, action, note, unknown = match.captures
      note = Integer(note, 16)
      action_text = case(action)
      when 'on'; 'ON '
      when 'off'; 'OFF'
      else; raise "bad action: #{action}"
      end
      $out << "%3d,MIDI_NOTE_%s,%3d" % [offset, action_text, note]
    end
  end
end

# ================================================================

run()
