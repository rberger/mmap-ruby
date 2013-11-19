$:.unshift File.dirname(__FILE__)
require "mmap_ruby/mmap"

class MmapRuby
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
    @mmap.read(offset, length)
  end

  def write(offset, length, bytes)
    @mmap.write(offset, length, bytes)  
  end

  def mlock(len)
    @mmap.mlock(len)
  end

  def munlock(len)
    @mmap.munlock(len)
  end

  def madvise(len, advice)
    @mmap.madvise(len, advice)
  end

  # Return size of mapped file
  def size
    @mmap.size
  end
end
