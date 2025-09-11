# Development notes

# Adding a none NAN boxed type
* In include/picrin/value.h
  * Add the new PIC_TYPE_XXX to the enum
  * Define a pic_xxx_p (see the DEFPRED section)
* In object.h
  * Define the struct for the new type
* In gc.c
  * Add the new struct to the object union
  * Add the enum to the case statement in gc_mark_object() and handle appropriately
  * In the gc_finalize_object() case statement add the type enum and free as necessary
* In bool.c handle equal?, eq? and eqv?
* in ext/read.c add the necessary code to read the new type
* in ext/write.c add the necessary code to write the new type: ensure read/write/read works in appropriate
