=begin
/*------------------------------------------------------------
 //
 //  NESPAK v1.1
 //  Copyright (c) 2013 Jarrod Parkes. All rights reserved.
 //
 //  Release Candidates
 //  v1.0 - 12/14/12 - Added RLE/Huffman compressions
 //	 v1.1 - 12/28/13 - Ported to Ruby
 //
 //--------------------------------------------------------*/
=end

VERSION = 1.1

BYTE = 8
WORD = 16
DWORD = 32
QUAD = 64

require './options.rb'
require './overrides.rb'
require './algorithms.rb'

# include overrides for standard classes
include Overrides

class Nespak
	attr_accessor :pre, :pre_size, :post, :post_size

	def initialize(i_size, o_size)
		@pre_size = i_size
		@post_size = o_size
		@pre = []
		@post = []
	end

	include Algorithms

	# processes unpacked data item by item
	def pack(input_path, output_path, algorithm)

		# open input, convert to binary
		in_file = File.read(input_path)
		in_file.each_line do |line|
			@pre << line.to_binary(@pre_size)
		end

		# run compression algorithm
		case algorithm
			when :rle
				@post = rle_pack(@pre, true)
			when :lz78
				@post = lz78_pack(@pre)
			when :huff
				@post = huff_pack(@pre)
		end

		# open output, write binary
		out_file = File.open(output_path, 'wb')
		@post.each do |byte|
			out_file.write byte
		end

	end

	def unpack(input_path, output_path, algorithm)

		# open input as binary
		in_stream = File.open(input_path, 'rb').read
		@pre = in_stream.scan(/.{8}/)

		# run compression algorithm
		case algorithm
			when :rle
				@post = rle_unpack(@pre, true)
			when :lz78
				@post = lz78_unpack(@pre)
			when :huff
				@post = huff_unpack(@pre)
		end

		# open output, write binary
		out_file = File.open(output_path, 'wb')
		@post.each do |byte|
			out_file.puts byte.to_i(2)
		end
	end
end

# parse command line options
options = Options.parse(ARGV)

# create nespak, run compression/decompression
nespak = Nespak.new(BYTE, BYTE)
nespak.pack(options[:input], options[:output], options[:algorithm]) if options[:packing] == :pack
nespak.unpack(options[:input], options[:output], options[:algorithm]) if options[:packing] == :unpack
