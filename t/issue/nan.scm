(import (scheme base)
        (srfi 27)
        (scheme inexact)
        (picrin test))

(test-begin)

(define (rountrip-ok number)
  (let ((radix 10)) 
    (eqv? number (string->number (number->string number radix) radix))))

(test #t (rountrip-ok -nan.0))

(test #t (rountrip-ok +nan.0))
