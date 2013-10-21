# Usage

How to use mmap-ruby

## Installation

Add this to your gemfile

    gem 'mmap-ruby', git: 'git@github.com:brianosaurus/mmap-ruby.git' 


## Running
    
    offset = 0
    len = 4
    data = '1234'

    @mw = MmapRuby::MemWindow.new(4096)
    @mw.write(offset, len, data)
    @mw.read(offset)

    @mw.mlock(offset, 4096)
    @mf.unlock(offset, 4096)


