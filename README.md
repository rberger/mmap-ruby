# Usage

This is a very simplistic interface.  It will only map anonymous memory.  It does not handle memory mapped files.  If someone wants to add to this gem please feel free.

## Installation

Add this to your gemfile

    gem 'mmap-ruby', git: 'git@github.com:brianosaurus/mmap-ruby.git' 


## Running
    
    offset = 0
    len = 4
    data = '1234'

    @mw = MmapRuby::MemWindow.new(4096)
    @mw.write(offset, len, data)
    @mw.read(offset, len)

    @mw.mlock(offset, 4096)
    @mf.unlock(offset, 4096)


