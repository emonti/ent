#include "ruby.h"

#include "randtest.h"
#include "iso8859.h"


VALUE m_ent = Qnil;
VALUE c_random_test = Qnil;

void rb_rt_mark(rt_ctx *ctx) { }

VALUE rb_rt_allocate(VALUE klass) {
  rt_ctx * ctx = rt_new();
  return Data_Wrap_Struct(klass, rb_rt_mark, rt_free, ctx);
}

VALUE rb_rt_initialize(int argc, VALUE *argv, VALUE klass) {
  rt_ctx * ctx;
  VALUE binmode = Qnil;
  return Qnil;
}

void Init_ent_native() {
  rt_init();

  m_ent = rb_define_module("Ent");

  c_random_test = rb_define_class_under(m_ent, "RandomTest", rb_cObject);


}


