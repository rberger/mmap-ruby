require "test/unit"

require File.dirname(__FILE__) + "/../lib/mmap_ruby"

class TestMmapRuby < Test::Unit::TestCase
  PAGE_SIZE = 4096
  def setup
    @mw = MmapRuby.new(PAGE_SIZE)
  end
  
  def teardown
    @mw.close
  end
  
  def test_write
    @mw.write(0, 7, "abcdefg")
  end

  def test_lock
    assert_equal(true, @mw.lock(PAGE_SIZE))
  end

  def test_unlock
    assert_equal(true, @mw.unlock(PAGE_SIZE))
  end

  def test_read
    @mw.write(0, 7, "abcdefg")
    assert_equal("abc", @mw.read(0, 3))
  end

  def test_advise
    assert_equal(true, @mw.advise(PAGE_SIZE, MmapRuby::Mmap::MADV_NORMAL))
  end

  def test_size
    assert_equal PAGE_SIZE, @mw.size
  end
end
