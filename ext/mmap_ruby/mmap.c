/* Copyright (c) 2009 Johan Sørensen
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include "ruby.h"

// a memory mapped file
typedef struct {  
  caddr_t map;
	size_t len;
} mmap_ruby_map;

static VALUE mod_mmap_ruby;
static VALUE mr_mmap;
static VALUE mr_map_data;

/*
 * Document-method: new
 * call-seq: SimpleMMap::MappedFile.new(path)
 *
 * mmap() the file at +path+
 */
static VALUE mr_mmap_initialize(VALUE vself, VALUE vlength)
{
  size_t length = NUM2INT(vlength);
  caddr_t base = (caddr_t) - 1;
  VALUE vmr_map;
  mmap_ruby_map *mr_map;
  
  // do the mmap
  base = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);

  if (base == (caddr_t) -1) {
    rb_raise(rb_eArgError, "Failed to mmap memory");
  }
  
  // set the mmap structure as an ivar
  mr_map = ALLOC(mmap_ruby_map);
  vmr_map = Data_Wrap_Struct(mr_map_data, NULL, free, mr_map);
  mr_map->map = base;
  mr_map->len = length;
  rb_ivar_set(vself, rb_intern("@mmap_data"), vmr_map);
  
  return Qnil;
}

/*
 * Document-method: close
 * call-seq: obj.close
 *
 * munmap() the current mmapped file
 */
static VALUE mr_mmap_close(VALUE vself)
{
  VALUE vmr_map;
  mmap_ruby_map *mr_map;
  
  mr_map = ALLOC(mmap_ruby_map);
  vmr_map = rb_ivar_get(vself, rb_intern("@mmap_data"));
  Data_Get_Struct(vmr_map, mmap_ruby_map, mr_map);
  
  if (mr_map->map != (caddr_t) -1)
    munmap(mr_map->map, mr_map->len);
  
  return Qtrue;
}

/*
 * Document-method: mlock
 * call-seq: obj.read_window_data(offset, length)
 *
 * Read +length+ bytes starting at +offset+
 */
static VALUE mr_mmap_mlock(VALUE vself, VALUE voffset, VALUE vlength) 
{
  size_t offset = NUM2INT(voffset);
  size_t length = NUM2INT(vlength);
  VALUE vmr_map;
  mmap_ruby_map *mr_map;
  
  vmr_map = rb_ivar_get(vself, rb_intern("@mmap_data"));
  Data_Get_Struct(vmr_map, mmap_ruby_map, mr_map);
  
  if (offset < 0 || offset > mr_map->len) {
    return Qnil;
  }

  // If the range overflows, return part that overlaps
  if ((offset + length) > mr_map->len) {
    length = mr_map->len - offset;
  }

  if (mlock(mr_map->map, length) == 0) {
    return Qtrue;
  } 
  else {
    return Qfalse;
  }
}

/*
 * Document-method: read_window_data
 * call-seq: obj.read_window_data(offset, length)
 *
 * Read +length+ bytes starting at +offset+
 */
static VALUE mr_mmap_munlock(VALUE vself, VALUE voffset, VALUE vlength) 
{
  size_t offset = NUM2INT(voffset);
  size_t length = NUM2INT(vlength);
  VALUE vmr_map;
  mmap_ruby_map *mr_map;
  
  vmr_map = rb_ivar_get(vself, rb_intern("@mmap_data"));
  Data_Get_Struct(vmr_map, mmap_ruby_map, mr_map);
  
  if (offset < 0 || offset > mr_map->len) {
    return Qnil;
  }

  // If the range overflows, return part that overlaps
  if ((offset + length) > mr_map->len) {
    length = mr_map->len - offset;
  }


  // If the range overflows, return part that overlaps
  if ((offset + length) > mr_map->len) {
    length = mr_map->len - offset;
  }

  // simple enough
  if (mlock(mr_map->map, length) == 0) {
    return Qtrue;
  } 
  else {
    return Qfalse;
  }
}

/*
 * Document-method: read_window_data
 * call-seq: obj.read_window_data(offset, length)
 *
 * Read +length+ bytes starting at +offset+
 */
static VALUE mr_mmap_write(VALUE vself, VALUE voffset, VALUE vlength, VALUE vbytes) 
{
  size_t offset = NUM2INT(voffset);
  size_t length = NUM2INT(vlength);
  char * bytes  = StringValuePtr(vbytes);

  VALUE vmr_map;
  mmap_ruby_map *mr_map;
  
  vmr_map = rb_ivar_get(vself, rb_intern("@mmap_data"));
  Data_Get_Struct(vmr_map, mmap_ruby_map, mr_map);
  
  if (offset < 0 || offset > mr_map->len) {
    return Qnil;
  }

  // If the range overflows, return part that overlaps
  if ((offset + length) > mr_map->len) {
    length = mr_map->len - offset;
  }

  // simple memcpy at this point
  memcpy(mr_map->map + offset, bytes, length);
  return Qtrue;
}

/*
 * Document-method: read_window_data
 * call-seq: obj.read_window_data(offset, length)
 *
 * Read +length+ bytes starting at +offset+
 */
static VALUE mr_mmap_read_window_data(VALUE vself, VALUE voffset, VALUE vlength) 
{
  size_t offset = NUM2INT(voffset);
  size_t length = NUM2INT(vlength);
  VALUE vmr_map;
  mmap_ruby_map *mr_map;
  
  vmr_map = rb_ivar_get(vself, rb_intern("@mmap_data"));
  Data_Get_Struct(vmr_map, mmap_ruby_map, mr_map);
  
  if (offset < 0 || offset > mr_map->len) {
    return Qnil;
  }

  // If the range overflows, return part that overlaps
  if ((offset + length) > mr_map->len) {
    length = mr_map->len - offset;
  }

  return rb_str_new(mr_map->map + offset, length);
}

/*
 * Document-method: size
 * call-seq: obj.size
 *
 * Return size of mapped file
 */

static VALUE mr_mmap_size(VALUE vself)
{
  VALUE vmr_map;
  mmap_ruby_map *mr_map;

  vmr_map = rb_ivar_get(vself, rb_intern("@mmap_data"));
  Data_Get_Struct(vmr_map, mmap_ruby_map, mr_map);
  return UINT2NUM(mr_map->len);
}

void Init_mmap()
{
  mod_mmap_ruby = rb_define_module("MmapRuby");
  
  mr_mmap = rb_define_class_under(mod_mmap_ruby, "Mmap", rb_cObject);
  rb_define_private_method(mr_mmap, "initialize", mr_mmap_initialize, 1);
  rb_define_method(mr_mmap, "close", mr_mmap_close, 0);
  rb_define_method(mr_mmap, "read_window_data", mr_mmap_read_window_data, 2);
  rb_define_method(mr_mmap, "size", mr_mmap_size, 0);

  /* new stuff added by WOODS */
  rb_define_method(mr_mmap, "mlock", mr_mmap_mlock, 2);
  rb_define_method(mr_mmap, "munlock", mr_mmap_munlock, 2);
  rb_define_method(mr_mmap, "write", mr_mmap_write, 3);
  
  mr_map_data = rb_define_class_under(mr_mmap, "MmapData", rb_cObject);
}
