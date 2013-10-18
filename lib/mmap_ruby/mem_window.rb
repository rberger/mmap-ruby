# Copyright (c) 2009 Johan Sørensen
# 
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# 'Software'), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
# 
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

module MmapRuby
  # Represents a window into a mmap()'ed file, using a familiar index syntax
  # for getting single bytes or Ranges of bytes within that FileWindow
  class MemWindow
    # Create a mmap'ed window for file at +path+
    # You are responsible for closing it when you're done using #close
    def initialize(len)
      @mmap = MmapRuby::Mmap.new(len)
    end
    
    # unmaps the mmap'ed file
    def close
      @mmap.close
      @mmap = nil
    end
    alias_method :unmap, :close
  
    # Read +length+ bytes starting from the current offset
    def read(offset, length)
      @mmap.read_window_data(offset, length)
    end

    def write(offset, length, bytes)
      @mmap.write(offset, length, bytes)  
    end

    def mlock(offset, len)
      @mmap.mlock(offset, len)
    end

    def munlock(offset, len)
      @mmap.munlock(offset, len)
    end

    # Return size of mapped file
    def size
      @mmap.size
    end
  end
end
