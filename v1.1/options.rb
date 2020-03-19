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

require 'optparse'

class Options
	
	attr_accessor :options

	def self.parse(args)

		# default values
		@options = {}
		@options[:packing] = nil
		@options[:algorithm] = nil
		@options[:input] = nil
		@options[:output] = nil

		# define command line options
		opts = OptionParser.new do |opts|

			opts.banner = "Usage: nespak.rb [packing] [options]"
		    opts.separator "Try \'nespak -help\' for more information."

			opts.separator ""
		    opts.separator "Packing options:"

			opts.on("-p", "--packing [TYPE]", [:unpack, :pack], "Packing or unpacking") do |pack|
				@options[:packing] = pack
			end

			opts.separator ""
		    opts.separator "Specific options:"

			opts.on("-a", "--algorithm [TYPE]", [:rle, :lz78, :huff], "Compression algorithm", "(rle, lz78, huff)") do |alg|
				@options[:algorithm] = alg
			end

			opts.on("-i", "--input [PATH]", String, "Input file path") do |path|
				@options[:input] = path
			end

			opts.on("-o", "--output [PATH]", String, "Output file path") do |path|
				@options[:output] = path
			end

			opts.separator ""
		    opts.separator "Common options:"

			opts.on_tail("-h", "--help", "Show help") do
				puts opts
				exit
			end

			# print the version.
			opts.on_tail("-v", "--version", "Show version") do
				puts "NESPAK v#{VERSION}"
				puts "Author: Jarrod Parkes"
				exit
			end

			# parse command line arguments
			begin
			  opts.parse!(args)
			  mandatory = [:packing, :algorithm, :input, :output]
			  missing = mandatory.select{|param| @options[param].nil?}
			  if not missing.empty?
			    puts "Missing options: #{missing.join(', ')}"
			    puts opts
			    exit
			  end
			rescue OptionParser::InvalidOption, OptionParser::MissingArgument
			  puts $!.to_s
			  puts opts
			  exit
			end
		end # end opt
		return @options
	end # end of self.parse
end	# end of Options