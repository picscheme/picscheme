(import (scheme base)
        (picrin readline history)
        (picrin test))
  
(define testfile "/tmp/picrin_readline_test_file")
(test-begin)

(test 0 (history-length))
(test 0 (where-history))
(add-history "1")
(test 1 (history-length))
(test 0 (where-history))
(let loop ((n 2))
  (if (>= 10 n)
      (begin
        (add-history (number->string n))
        (loop (+ n 1)))))
#|
index     0   1   2   3   4   5   6   7   8   9
history  "1" "2" "3" "4" "5" "6" "7" "8" "9" "10"
offset    1   2   3   4   5   6   7   8   9   10
pos       10  9   8   7   6   5   4   3   2   1
                                              p
|#
(test 10 (history-length))
(test 9 (where-history))
(stifle-history 7)
#|
index     0   1   2   3   4   5   6
history  "4" "5" "6" "7" "8" "9" "10"
offset    4   5   6   7   8   9   10
pos       7   6   5   4   3   2   1
                                  p
|#
(test 7 (history-length))
(test 6 (where-history))
(test #t (history-set-pos 0))
#|
index     0   1   2   3   4   5   6
history  "4" "5" "6" "7" "8" "9" "10"
offset    4   5   6   7   8   9   10
pos       7   6   5   4   3   2   1
          p
|#
(test 0 (where-history))
(add-history "11")
#|
index     0   1   2   3   4   5    6
history  "5" "6" "7" "8" "9" "10" "11"
offset    5   6   7   8   9   10   11
pos       7   6   5   4   3   2    1
                                   p
|#
(test 7 (history-length))
(test 6 (where-history))
(test #t (history-stifled?))
(unstifle-history)
(test #f (history-stifled?))
(test 7 (history-length))
(test 6 (where-history))
(add-history "12")
#|
index     0   1   2   3   4   5    6    7
history  "5" "6" "7" "8" "9" "10" "11" "12"
offset    5   6   7   8   9   10   11   12
pos       8   7   6   5   4   3    2    1
                                        p
|#
(test 8 (history-length))
(test 7 (where-history))
(test "12" (current-history))
(test 7 (where-history))
(test "11" (previous-history))
#|
index     0   1   2   3   4   5    6    7
history  "5" "6" "7" "8" "9" "10" "11" "12"
index     5   6   7   8   9   10   11   12
pos       8   7   6   5   4   3    2    1
                                   p
|#
(test 6 (where-history))
(test "12" (next-history))
#|
index     0   1   2   3   4   5    6    7
history  "5" "6" "7" "8" "9" "10" "11" "12"
offset    5   6   7   8   9   10   11   12
pos       8   7   6   5   4   3    2    1
                                        p
|#
(test 7 (where-history))
(test #t (history-set-pos 5))
#|
index     0   1   2   3   4   5    6    7
history  "5" "6" "7" "8" "9" "10" "11" "12"
offset    5   6   7   8   9   10   11   12
pos       8   7   6   5   4   3    2    1
                              p
|#
(test 5 (where-history))
(test #f (history-set-pos (history-length)))
(test 5 (where-history))
(test #f (history-set-pos -1))
(test 5 (where-history))
(test #t (history-set-pos 0))
#|
index     0   1   2   3   4   5    6    7
history  "5" "6" "7" "8" "9" "10" "11" "12"
offset    5   6   7   8   9   10   11   12
pos       8   7   6   5   4   3    2    1
          p
|#
(test 0 (where-history))
(test #t (history-set-pos (- (history-length) 2)))
#|
index     0   1   2   3   4   5    6    7
history  "5" "6" "7" "8" "9" "10" "11" "12"
offset    5   6   7   8   9   10   11   12
pos       8   7   6   5   4   3    2    1
                                   p
|#
(test 6 (where-history))
(test "11" (current-history))
(test 6 (where-history))
(test "10" (previous-history))
#|
index     0   1   2   3   4   5    6    7
history  "5" "6" "7" "8" "9" "10" "11" "12"
offset    5   6   7   8   9   10   11   12
pos       8   7   6   5   4   3    2    1
                                        p
|#
(test 5 (where-history))
(test "11" (next-history))
#|
index     0   1   2   3   4   5    6    7
history  "5" "6" "7" "8" "9" "10" "11" "12"
offset    5   6   7   8   9   10   11   12
pos       8   7   6   5   4   3    2    1
                                   p
|#
(test 6 (where-history))
(test "10" (history-get 5))
(test "6" (remove-history 1))
#|
index     0   1   2   3   4    5    6 
history  "5" "7" "8" "9" "10" "11" "12"
offset    5   7   8   9   10   11   12
pos       7   6   5   4   3    2    1
                                    p
|#
(test 6 (where-history))
(test "12" (current-history))
(test "11" (previous-history))
(test "12" (history-get 6))
#|
index     0   1   2   3   4    5    6 
history  "5" "7" "8" "9" "10" "11" "12"
offset    5   7   8   9   10   11   12
pos       7   6   5   4   3    2    1
                               p
|#
(test 7 (history-length))
(history-set-pos 6)
#|
index     0   1   2   3   4    5    6 
history  "5" "7" "8" "9" "10" "11" "12"
offset    5   7   8   9   10   11   12
pos       7   6   5   4   3    2    1
                                    p
|#
(test 0 (history-search "7" -1))
#|
index     0   1   2   3   4    5    6 
history  "5" "7" "8" "9" "10" "11" "12"
offset    5   7   8   9   10   11   12
pos       7   6   5   4   3    2    1
              p
|#
(test 1 (where-history))
(test "7" (current-history))
(history-set-pos 5)
(test "11" (current-history))
#|
index     0   1   2   3   4    5    6 
history  "5" "7" "8" "9" "10" "11" "12"
offset    5   7   8   9   10   11   12
pos       7   6   5   4   3    2    1
                                    p
|#
(test -1 (history-search "7" 0))
(test 5 (where-history))
#|
index     0   1   2   3   4    5    6 
history  "5" "7" "8" "9" "10" "11" "12"
offset    5   7   8   9   10   11   12
pos       7   6   5   4   3    2    1
                                    p
|#
(history-set-pos 0)
#|
index     0   1   2   3   4    5    6 
history  "5" "7" "8" "9" "10" "11" "12"
offset    5   7   8   9   10   11   12
pos       7   6   5   4   3    2    1
          p
|#
(test -1 (history-search "7" -1))
#|
index     0   1   2   3   4    5    6 
history  "5" "7" "8" "9" "10" "11" "12"
offset    5   7   8   9   10   11   12
pos       7   6   5   4   3    2    1
          p
|#
(test 0 (where-history))
(history-set-pos 0)
#|
index     0   1   2   3   4    5    6 
history  "5" "7" "8" "9" "10" "11" "12"
offset    5   7   8   9   10   11   12
pos       7   6   5   4   3    2    1
          p
|#
(test 0 (history-search "7" 0))
#|
index     0   1   2   3   4    5    6 
history  "5" "7" "8" "9" "10" "11" "12"
offset    5   7   8   9   10   11   12
pos       7   6   5   4   3    2    1
              p
|#
(test 1 (where-history))
(test 0 (history-search-prefix "1" 0))
#|
index     0   1   2   3   4    5    6 
history  "5" "7" "8" "9" "10" "11" "12"
offset    5   7   8   9   10   11   12
pos       7   6   5   4   3    2    1
                          p
|#
(test 4 (where-history))
(history-set-pos 5)
#|
index     0   1   2   3   4    5    6 
history  "5" "7" "8" "9" "10" "11" "12"
offset    5   7   8   9   10   11   12
pos       7   6   5   4   3    2    1
                               p
|#
(test 0 (history-search-prefix "1" -1))
#|
index     0   1   2   3   4    5    6 
history  "5" "7" "8" "9" "10" "11" "12"
offset    5   7   8   9   10   11   12
pos       7   6   5   4   3    2    1
                               p
|#
(test 4 (where-history))
(test 0 (history-search-prefix "1" 0))
#|
index     0   1   2   3   4    5    6 
history  "5" "7" "8" "9" "10" "11" "12"
offset    5   7   8   9   10   11   12
pos       7   6   5   4   3    2    1
                               p
|#
(test 5 (where-history))
(test 0 (history-search-prefix "9" -1))
#|
index     0   1   2   3   4    5    6 
history  "5" "7" "8" "9" "10" "11" "12"
offset    5   7   8   9   10   11   12
pos       7   6   5   4   3    2    1
                      p
|#
(test 3 (where-history))
(write-history testfile)
(test 7 (history-length))
(clear-history)
#|
index     
history  
offset   
pos       
             p
|#
(test 0 (history-length))
(test 0 (where-history))
(add-history "f")
#|
index      1
history   "f"
offset     1
pos        1
           p
|#
(test 1 (history-length))
(test 0 (where-history))
(test "f" (history-get 0))
(read-history testfile)
;#|
;index     0   1   2   3   4    5    6    7
;history  "f" "5" "7" "8" "9" "10" "11" "12"
;offset    1   2   3   4   5    6    7   8
;pos       8   7   6   5   4    3    2   1
;                                        p
;|#
(test 8 (history-length))
(clear-history)
;; history_truncate_file in libedit's readline API does not include the
;; history magic cookie at the top of the file, this breaks read-history
;(truncate-file testfile 5)
;(read-history testfile)
;;(test 5 (history-length))
(clear-history)
(add-history "foo")
(add-history "bar")
(add-history "baz")
(test "baz" (history-expand "!b"))
(test "baz" (history-expand "!!"))
(test "bar" (history-expand "!?r?"))
(add-history "com arg")
(test "arg" (history-expand "!!:$"))
(test "arg" (history-expand "!$"))
(test "command arg" (history-expand "!!:s/com/command/"))

(test-end)
