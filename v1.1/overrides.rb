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

module Overrides
	# open File class and add functionality
	class ::File
		def each_chunk(chunk_size, endianness = nil)
		    yield read(chunk_size) until eof?
		end
	end

	# open String class and add functionality
	class ::String
		def to_binary(chunk_size, endianness = nil)
			self.chomp.to_i.to_s(2).rjust(chunk_size, '0') 
		end
	end

	class ::Fixnum
		def to_binary(chunk_size)
			self.to_s(2).rjust(chunk_size, '0')
		end
	end
end