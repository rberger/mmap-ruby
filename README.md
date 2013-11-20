# Usage

This is a very simplistic interface.  It will only map anonymous memory.  It does not handle memory mapped files.  If someone wants to add to this gem please feel free.

## Installation

Add this to your gemfile

    gem 'mmap-ruby', git: 'git@github.com:brianosaurus/mmap-ruby.git' 


## Running
    
    offset = 0
    len = 4
    data = '1234'

    @mw = MmapRuby.new(4096)
    @mw.write(offset, len, data)
    @mw.read(offset, len)

    @mw.lock(4096)
    @mw.unlock(4096)
    @mw.advise(4096, MmapRuby::MADV_SEQUENTIAL)


