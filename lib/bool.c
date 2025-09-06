/**
 * See Copyright Notice in picrin.h
 */

#include "picrin.h"
#include "object.h"

#if !PIC_NAN_BOXING

#ifndef __STDC_VERSION__
#define c89isnan(x) ((x) != (x))
#else
#include <math.h>
#define c89isnan(x) isnan(x)
#endif

bool
pic_eq_p(pic_state *PIC_UNUSED(pic), pic_value x, pic_value y)
{
  if (pic_type(pic, x) != pic_type(pic, y))
    return false;

  switch (pic_type(pic, x)) {
  case PIC_TYPE_NIL:
    return true;
  case PIC_TYPE_TRUE: case PIC_TYPE_FALSE:
    return pic_type(pic, x) == pic_type(pic, y);
  default:
    return obj_ptr(pic, x) == obj_ptr(pic, y);
  }
}

bool
pic_eqv_p(pic_state *PIC_UNUSED(pic), pic_value x, pic_value y)
{
  float x_v, y_v;

  if (pic_type(pic, x) != pic_type(pic, y))
    return false;

  switch (pic_type(pic, x)) {
  case PIC_TYPE_NIL:
    return true;
  case PIC_TYPE_TRUE: case PIC_TYPE_FALSE:
    return pic_type(pic, x) == pic_type(pic, y);
  case PIC_TYPE_FLOAT:
    x_v = pic_float(pic, x);
    y_v = pic_float(pic, y);
    if (c89isnan(x_v) && c89isnan(y_v))
      return 1;
    return x_v == y_v;
  case PIC_TYPE_INT:
    return pic_int(pic, x) == pic_int(pic, y);
  default:
    return obj_ptr(pic, x) == obj_ptr(pic, y);
  }
}

#endif

KHASH_DECLARE(m, void *, int)
KHASH_DEFINE2(m, void *, int, 0, kh_ptr_hash_func, kh_ptr_hash_equal)

static bool
internal_equal_p(pic_state *pic, pic_value x, pic_value y, int depth, khash_t(m) *h)
{
  pic_value localx = pic_nil_value(pic);
  pic_value localy = pic_nil_value(pic);
  int cx = 0;
  int cy = 0;

  if (depth > 10) {
    if (depth > 200) {
      pic_error(pic, "stack overflow in equal", 0);
    }
    if (pic_pair_p(pic, x) || pic_vec_p(pic, x)) {
      int ret;
      kh_put(m, h, obj_ptr(pic, x), &ret);
      if (ret != 0) {
        return true;            /* `x' was seen already.  */
      }
    }
  }

 LOOP:

  if (pic_eqv_p(pic, x, y)) {
    return true;
  }
  if (pic_type(pic, x) != pic_type(pic, y)) {
    return false;
  }

  switch (pic_type(pic, x)) {
  case PIC_TYPE_ID: {
    struct identifier *id1, *id2;
    pic_value s1, s2;

    id1 = pic_id_ptr(pic, x);
    id2 = pic_id_ptr(pic, y);

    s1 = pic_find_identifier(pic, obj_value(pic, id1->u.id), obj_value(pic, id1->env));
    s2 = pic_find_identifier(pic, obj_value(pic, id2->u.id), obj_value(pic, id2->env));

    return pic_eq_p(pic, s1, s2);
  }
  case PIC_TYPE_STRING: {
    int xlen, ylen;
    const char *xstr, *ystr;

    xstr = pic_str(pic, x, &xlen);
    ystr = pic_str(pic, y, &ylen);

    if (xlen != ylen) {
      return false;
    }
    return strcmp(xstr, ystr) == 0;
  }
  case PIC_TYPE_BLOB: {
    int xlen, ylen;
    const unsigned char *xbuf, *ybuf;

    xbuf = pic_blob(pic, x, &xlen);
    ybuf = pic_blob(pic, y, &ylen);

    if (xlen != ylen) {
      return false;
    }
    return memcmp(xbuf, ybuf, xlen) == 0;
  }
  case PIC_TYPE_PAIR: {
    if (! internal_equal_p(pic, pic_car(pic, x), pic_car(pic, y), depth + 1, h))
      return false;

    /* Floyd's cycle-finding algorithm */
    if (pic_nil_p(pic, localx)) {
      localx = x;
    }
    x = pic_cdr(pic, x);
    cx++;
    if (pic_nil_p(pic, localy)) {
      localy = y;
    }
    y = pic_cdr(pic, y);
    cy++;
    if (cx == 2) {
      cx = 0;
      localx = pic_cdr(pic, localx);
      if (pic_eq_p(pic, localx, x)) {
        if (cy < 0 ) return true; /* both lists circular */
        cx = INT_MIN; /* found a cycle on x */
      }
    }
    if (cy == 2) {
      cy = 0;
      localy = pic_cdr(pic, localy);
      if (pic_eq_p(pic, localy, y)) {
        if (cx < 0 ) return true; /* both lists circular */
        cy = INT_MIN; /* found a cycle on y */
      }
    }
    goto LOOP;                  /* tail-call optimization */
  }
  case PIC_TYPE_VECTOR: {
    int i, xlen, ylen;

    xlen = pic_vec_len(pic, x);
    ylen = pic_vec_len(pic, y);

    if (xlen != ylen) {
      return false;
    }
    for (i = 0; i < xlen; ++i) {
      if (! internal_equal_p(pic, pic_vec_ref(pic, x, i), pic_vec_ref(pic, y, i), depth + 1, h))
        return false;
    }
    return true;
  }
  case PIC_TYPE_DATA: {
    return pic_data(pic, x) == pic_data(pic, y);
  }
  default:
    return false;
  }
}

bool
pic_equal_p(pic_state *pic, pic_value x, pic_value y)
{
  khash_t(m) h;

  kh_init(m, &h);

  return internal_equal_p(pic, x, y, 0, &h);
}

static pic_value
pic_bool_eq_p(pic_state *pic)
{
  pic_value x, y;

  pic_get_args(pic, "oo", &x, &y);

  return pic_bool_value(pic, pic_eq_p(pic, x, y));
}

static pic_value
pic_bool_eqv_p(pic_state *pic)
{
  pic_value x, y;

  pic_get_args(pic, "oo", &x, &y);

  return pic_bool_value(pic, pic_eqv_p(pic, x, y));
}

static pic_value
pic_bool_equal_p(pic_state *pic)
{
  pic_value x, y;

  pic_get_args(pic, "oo", &x, &y);

  return pic_bool_value(pic, pic_equal_p(pic, x, y));
}

static pic_value
pic_bool_not(pic_state *pic)
{
  pic_value v;

  pic_get_args(pic, "o", &v);

  return pic_bool_value(pic, pic_false_p(pic, v));
}

static pic_value
pic_bool_boolean_p(pic_state *pic)
{
  pic_value v;

  pic_get_args(pic, "o", &v);

  return pic_bool_value(pic, pic_bool_p(pic, v));
}

static pic_value
pic_bool_boolean_eq_p(pic_state *pic)
{
  int argc, i;
  pic_value *argv;

  pic_get_args(pic, "*", &argc, &argv);

  for (i = 0; i < argc; ++i) {
    if (! (pic_true_p(pic, argv[i]) || pic_false_p(pic, argv[i]))) {
      return pic_false_value(pic);
    }
    if (! pic_eq_p(pic, argv[i], argv[0])) {
      return pic_false_value(pic);
    }
  }
  return pic_true_value(pic);
}

void
pic_init_bool(pic_state *pic)
{
  pic_defun(pic, "eq?", pic_bool_eq_p);
  pic_defun(pic, "eqv?", pic_bool_eqv_p);
  pic_defun(pic, "equal?", pic_bool_equal_p);
  pic_defun(pic, "not", pic_bool_not);
  pic_defun(pic, "boolean?", pic_bool_boolean_p);
  pic_defun(pic, "boolean=?", pic_bool_boolean_eq_p);
}
