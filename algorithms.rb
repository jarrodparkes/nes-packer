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

module Algorithms

=begin
/*------------------------------------------------------------
 //
 //  RLE "Run-Length Encoding
 //
 //  Unpacked : AAAAAFFFFAF
 //  Packed   : 5-A 4-F 0xff-2-AF
 //  NES Form : 0x05 0x65 0x04 0x70 0xFF 0x02 0x65 0x70
 //
 //  * 0xFF indicates a unique run, followed by a
 //    length byte to indicated run length
 //
 //--------------------------------------------------------*/	
=end
	RLE_MAX_RUN_SIZE = 254
	RLE_UNIQUE_RUN_START = '11111111'

	def rle_pack(data, debug = nil)

		final = []
		queue = []

		pattern = nil
		in_pattern = nil
		run_length = 0

		data.each do |byte|
			
			# are we in a pattern?
			in_pattern = (pattern == byte)

			if in_pattern
				# did we previously have a queue? it so, write it
				if queue.size > 0
					if queue.size > 1
						pattern = queue.pop
						final << RLE_UNIQUE_RUN_START
						final << queue.size.to_binary(BYTE)
						while queue.size > 0
							final << queue.shift
						end
					else
						queue.clear						
					end
					run_length += 1
				end

				# add one to current run
				run_length += 1

				# has our run reached the maximum size?
				if run_length == RLE_MAX_RUN_SIZE
					final << RLE_MAX_RUN_SIZE.to_binary(BYTE)
					final << pattern
					run_length = 0
				end
			else
				# did we previously have a run? if so, write it
				if run_length > 0
					final << run_length.to_binary(BYTE)
					final << pattern
					run_length = 0
				end

				# append to queue
				queue << byte

				# set new pattern
				pattern = byte
			end
		end

		# clean-up
		if queue.size > 0
			puts "here2"
			final << RLE_UNIQUE_RUN_START
			final << queue.size.to_binary(BYTE)
			while queue.size > 0
				final << queue.shift
			end
		else
			final << run_length.to_binary(BYTE)
			final << pattern	
		end

		if debug
			puts "============================="
			puts "Status Report"
			puts "============================="
			puts "Before: #{data.size} bytes"
			puts "After: #{final.size} bytes"
			printf "Compression: %.2f%\n", ((data.size.to_f - final.size.to_f)/data.size.to_f) * 100
		end

		return final
	end

	def rle_unpack(data, debug = nil)

		final = []

	    reading_queue = nil
	    reading_run = nil
	    run_length = 0
	    start_size = data.size

	    while !data.empty?
	    	if data.first == RLE_UNIQUE_RUN_START
	    		data.shift
	    		run_length = data.shift.to_i(2)
	    		run_length.times do
	    			final << data.shift
	    		end
	    	else
	    		run_length = data.shift.to_i(2)
	    		run_length.times do
	    			final << data.first
	    		end
	    		data.shift
	    	end
	    end

		if debug
			puts "============================="
			puts "Status Report"
			puts "============================="
			puts "Before: #{start_size} bytes"
			puts "After: #{final.size} bytes"
		end

		return final
	end

	def lz78_pack(data, debug = nil)
		puts "implement"
	end

	def lz78_unpack(data, debug = nil)
		puts "implement"
	end

	def huff_pack(data, debug = nil)
		puts "implement"
	end

	def huff_unpack(data, debug = nil)
		puts "implement"
	end
end