#include "ruby.h"

#include <stdio.h>

#include "randtest.h"

#ifndef DBL2NUM
#define DBL2NUM(dbl)  rb_float_new(dbl)
#endif

extern void rt_init();

extern rt_ctx *rt_new();
extern void rt_add(rt_ctx *ctx, void *buf, int bufl);
extern void rt_end(rt_ctx *ctx);
extern void rt_free(rt_ctx *ctx);


VALUE m_ent = Qnil;
VALUE c_random_test = Qnil;

VALUE rb_rt_assert_open(rt_ctx *ctx) {
  if(ctx->ended)
    rb_raise(rb_eIOError, "closed stream");

  return Qnil;
}

void rb_rt_mark(rt_ctx *ctx) { }

VALUE rb_rt_initialize(int argc, VALUE *argv, VALUE klass) {
  rt_ctx *ctx;
  VALUE binmode = Qnil;
  int _bmode = 0;

  rb_scan_args(argc, argv, "01", &binmode);
  if ((binmode != Qnil) && (binmode != Qfalse)) {
    Check_Type(binmode, T_TRUE);
    _bmode=1;
  }

  if(!(ctx = rt_new()))
    rb_sys_fail(0);

  ctx->binary = _bmode;

  return Data_Wrap_Struct(klass, rb_rt_mark, rt_free, ctx);
}


VALUE rb_rt_get_binmode(VALUE self) {
  rt_ctx *ctx;
  Data_Get_Struct(self, rt_ctx, ctx);
  if (ctx->binary)
    return Qtrue;
  else
    return Qfalse;
}


VALUE rb_rt_final(VALUE self) {
  rt_ctx *ctx;
  Data_Get_Struct(self, rt_ctx, ctx);

  if (! ctx->ended) 
    rt_end(ctx);

  return self;
}

VALUE rb_rt_read_string(VALUE self, VALUE rb_buf) {
  rt_ctx *ctx;

  Check_Type(rb_buf, T_STRING);

  Data_Get_Struct(self, rt_ctx, ctx);
  if(ctx->ended)
    rb_raise(rb_eIOError, "data cannot be added after finalizing");

  rt_add(ctx, RSTRING_PTR(rb_buf), RSTRING_LEN(rb_buf));

  return LONG2NUM(RSTRING_LEN(rb_buf));
}


VALUE rb_rt_read_file(VALUE self, VALUE rb_filename) {
  rt_ctx *ctx;
  FILE *fp;
  int oc;
  size_t fsz = 0;

  Check_Type(rb_filename, T_STRING);
  Data_Get_Struct(self, rt_ctx, ctx);
  if(ctx->ended)
    rb_raise(rb_eIOError, "data cannot be added after finalizing");

  if ((fp = fopen(RSTRING_PTR(rb_filename), "rb")) == NULL)
    rb_sys_fail(0);

  while((oc=fgetc(fp)) != EOF) {
    unsigned char ocb = (unsigned char) oc;
    rt_add(ctx, &ocb, 1);
    fsz++;
  }
  fclose(fp);

  return LONG2NUM(fsz);
}


VALUE rb_rt_entropy(VALUE self) {
  rt_ctx *ctx;
  Data_Get_Struct(self, rt_ctx, ctx);

  if(ctx->ended)
    return DBL2NUM(ctx->r_ent);
  else
    return Qnil;
}


VALUE rb_rt_mean(VALUE self) {
  rt_ctx *ctx;

  Data_Get_Struct(self, rt_ctx, ctx);

  if(ctx->ended)
    return DBL2NUM(ctx->r_mean);
  else
    return Qnil;
}


VALUE rb_rt_chisquare(VALUE self) {
  rt_ctx *ctx;
  Data_Get_Struct(self, rt_ctx, ctx);

  if(ctx->ended)
    return DBL2NUM(ctx->r_chisq);
  else
    return Qnil;
}

VALUE rb_rt_chisquare_probability(VALUE self) {
  rt_ctx *ctx;
  double ret, chip;

  Data_Get_Struct(self, rt_ctx, ctx);

  if(! ctx->ended)
    return Qnil;

  chip = pochisq(ctx->r_chisq, (ctx->binary ? 1 : 255));

  if (chip < 0.0001)
    ret = 0.009;
  else if (chip > 0.9999)
    ret = 99.991;
  else
    ret = chip * 100;

  return DBL2NUM(ret);
}


VALUE rb_rt_montepi(VALUE self) {
  rt_ctx *ctx;
  Data_Get_Struct(self, rt_ctx, ctx);

  if(ctx->ended)
    return DBL2NUM(ctx->r_montepicalc);
  else
    return Qnil;
}


VALUE rb_rt_scc(VALUE self) {
  rt_ctx *ctx;
  Data_Get_Struct(self, rt_ctx, ctx);

  if(ctx->ended)
    return DBL2NUM(ctx->r_scc);
  else
    return Qnil;
}

VALUE rb_rt_entropy_force(VALUE self) {
  rt_ctx *ctx;
  Data_Get_Struct(self, rt_ctx, ctx);

  if(! ctx->ended)
    rt_end(ctx);

  return DBL2NUM(ctx->r_ent);
}


VALUE rb_rt_mean_force(VALUE self) {
  rt_ctx *ctx;
  Data_Get_Struct(self, rt_ctx, ctx);

  if(! ctx->ended)
    rt_end(ctx);

  return DBL2NUM(ctx->r_mean);
}


VALUE rb_rt_chisquare_force(VALUE self) {
  rt_ctx *ctx;
  Data_Get_Struct(self, rt_ctx, ctx);

  if(! ctx->ended)
    rt_end(ctx);

  return DBL2NUM(ctx->r_chisq);
}

VALUE rb_rt_chisquare_probability_force(VALUE self) {
  rt_ctx *ctx;
  double ret, chip;

  Data_Get_Struct(self, rt_ctx, ctx);

  if(! ctx->ended)
    rt_end(ctx);

  chip = pochisq(ctx->r_chisq, (ctx->binary ? 1 : 255));

  if (chip < 0.0001)
    ret = 0.009;
  else if (chip > 0.9999)
    ret = 99.991;
  else
    ret = chip * 100;

  return DBL2NUM(ret);
}


VALUE rb_rt_montepi_force(VALUE self) {
  rt_ctx *ctx;
  Data_Get_Struct(self, rt_ctx, ctx);

  if(! ctx->ended)
    rt_end(ctx);

  return DBL2NUM(ctx->r_montepicalc);
}


VALUE rb_rt_scc_force(VALUE self) {
  rt_ctx *ctx;
  Data_Get_Struct(self, rt_ctx, ctx);

  if(! ctx->ended)
    rt_end(ctx);

  return DBL2NUM(ctx->r_scc);
}

VALUE rb_rt_result(VALUE self) {
  rt_ctx *ctx;
  VALUE ret;

  Data_Get_Struct(self, rt_ctx, ctx);

  if(! ctx->ended)
    rt_end(ctx);

  ret = rb_hash_new();
  rb_hash_aset(ret, ID2SYM(rb_intern("entropy")), DBL2NUM(ctx->r_ent));
  rb_hash_aset(ret, ID2SYM(rb_intern("mean")), DBL2NUM(ctx->r_mean));
  rb_hash_aset(ret, ID2SYM(rb_intern("chisquare")), DBL2NUM(ctx->r_chisq));
  rb_hash_aset(ret, ID2SYM(rb_intern("montepi")), DBL2NUM(ctx->r_montepicalc));
  rb_hash_aset(ret, ID2SYM(rb_intern("scc")), DBL2NUM(ctx->r_scc));
  return ret;
}

void Init_ent_native() {
  rt_init();

  m_ent = rb_define_module("Ent");

  c_random_test = rb_define_class_under(m_ent, "RandomTest", rb_cObject);

  rb_define_singleton_method(c_random_test, "new", rb_rt_initialize, -1);
  rb_define_method(c_random_test, "binary?", rb_rt_get_binmode, 0);

  rb_define_method(c_random_test, "read", rb_rt_read_string, 1);
  rb_define_method(c_random_test, "read_string", rb_rt_read_string, 1);
  rb_define_method(c_random_test, "read_file", rb_rt_read_file, 1);
  rb_define_method(c_random_test, "finalize", rb_rt_final, 0);
  rb_define_method(c_random_test, "result", rb_rt_result, 0);
  rb_define_method(c_random_test, "entropy", rb_rt_entropy, 0);
  rb_define_method(c_random_test, "mean", rb_rt_mean, 0);
  rb_define_method(c_random_test, "chisquare", rb_rt_chisquare, 0);
  rb_define_method(c_random_test, "chisquare_probability", rb_rt_chisquare_probability, 0);
  rb_define_method(c_random_test, "montepi", rb_rt_montepi, 0);
  rb_define_method(c_random_test, "scc", rb_rt_scc, 0);
  rb_define_method(c_random_test, "entropy!", rb_rt_entropy_force, 0);
  rb_define_method(c_random_test, "mean!", rb_rt_mean_force, 0);
  rb_define_method(c_random_test, "chisquare!", rb_rt_chisquare_force, 0);
  rb_define_method(c_random_test, "chisquare_probability!", rb_rt_chisquare_probability_force, 0);
  rb_define_method(c_random_test, "montepi!", rb_rt_montepi_force, 0);
  rb_define_method(c_random_test, "scc!", rb_rt_scc_force, 0);
 }


