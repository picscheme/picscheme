/**
 * See Copyright Notice in picrin.h
 */

#include <stdlib.h>

#include "picrin.h"
#include "picrin/extra.h"

#ifndef __STDC_VERSION__
/* Not exactly the same but this is all c89 has */
#define _Exit(status) _exit(status)
#endif /* __STDC_VERSION__ */

extern int picrin_argc;
extern char **picrin_argv;
extern char **picrin_envp;

static pic_value
pic_system_cmdline(pic_state *pic)
{
  pic_value v = pic_nil_value(pic);
  int i;

  pic_get_args(pic, "");

  for (i = 0; i < picrin_argc; ++i) {
    pic_push(pic, pic_cstr_value(pic, picrin_argv[i]), v);
  }
  return pic_reverse(pic, v);
}

static pic_value
pic_system_exit(pic_state *pic)
{
  pic_value v;
  int argc, status = EXIT_SUCCESS;

  argc = pic_get_args(pic, "|o", &v);
  if (argc == 1) {
    if (pic_float_p(pic, v)) {
      status = (int)pic_float(pic, v);
    } else if (pic_int_p(pic, v)) {
      status = pic_int(pic, v);
    }
  }

  pic_close(pic);

  exit(status);
}

static pic_value
pic_system_emergency_exit(pic_state *pic)
{
  pic_value v;
  int argc, status = EXIT_FAILURE;

  argc = pic_get_args(pic, "|o", &v);
  if (argc == 1) {
    if (pic_float_p(pic, v)) {
      status = (int)pic_float(pic, v);
    } else if (pic_int_p(pic, v)) {
      status = pic_int(pic, v);
    }
  }

  _Exit(status);
}

static pic_value
pic_system_getenv(pic_state *pic)
{
  char *str, *val;

  pic_get_args(pic, "z", &str);

  val = getenv(str);

  if (val == NULL)
    return pic_nil_value(pic);
  else
    return pic_cstr_value(pic, val);
}

static pic_value
pic_system_getenvs(pic_state *pic)
{
  char **envp;
  pic_value data = pic_nil_value(pic);
  size_t ai = pic_enter(pic);

  pic_get_args(pic, "");

  if (! picrin_envp) {
    return pic_nil_value(pic);
  }

  for (envp = picrin_envp; *envp; ++envp) {
    pic_value key, val;
    int i;

    for (i = 0; (*envp)[i] != '='; ++i)
      ;

    key = pic_str_value(pic, *envp, i);
    val = pic_cstr_value(pic, getenv(pic_str(pic, key, NULL)));

    /* push */
    data = pic_cons(pic, pic_cons(pic, key, val), data);

    pic_leave(pic, ai);
    pic_protect(pic, data);
  }

  return data;
}

void
pic_init_system(pic_state *pic)
{
  pic_defun(pic, "scheme.process-context:command-line", pic_system_cmdline);
  pic_defun(pic, "scheme.process-context:exit", pic_system_exit);
  pic_defun(pic, "scheme.process-context:emergency-exit", pic_system_emergency_exit);
  pic_defun(pic, "scheme.process-context:get-environment-variable", pic_system_getenv);
  pic_defun(pic, "scheme.process-context:get-environment-variables", pic_system_getenvs);
}
