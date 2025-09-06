(import (scheme base)
        (scheme file))

(with-output-to-file "test.txt"
    (lambda () (write "TEST")))
