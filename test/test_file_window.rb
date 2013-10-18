require "test/unit"

require File.dirname(__FILE__) + "/../lib/mmap_ruby"

class TestMemWindow < Test::Unit::TestCase
  PAGE_SIZE = 4096
  def setup
    @mw = MmapRuby::MemWindow.new(PAGE_SIZE)
  end
  
  def teardown
    @mw.close
  end
  
  def test_write
    @mw.write(0, 7, "abcdefg")
  end

  def test_mlock
    assert_equal(true, @mw.mlock(0, PAGE_SIZE))
  end

  def test_munlock
    assert_equal(true, @mw.munlock(0, PAGE_SIZE))
  end

  def test_read
    @mw.write(0, 7, "abcdefg")
    assert_equal "abc", @mw.read(0, 3)
  end

  def test_size
    assert_equal PAGE_SIZE, @mw.size
  end
end
