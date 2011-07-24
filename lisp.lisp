(label defun
  (lambda (name params expr)
    (label name
      (lambda params expr))))



(defun null. (x)
  (eq x (quote ())))

(null. (quote a))
(null. (quote ()))



(defun and. (x y)
  (cond (x (cond (y (quote t))
                 ((quote t) (quote ()))))
        ((quote t) (quote ()))))

(and. (atom (quote a)) (eq (quote a) (quote a)))
(and. (atom (quote a)) (eq (quote a) (quote b)))



(defun not. (x)
  (cond (x (quote ()))
        ((quote t) (quote t))))

(not. (eq (quote a) (quote a)))
(not. (eq (quote a) (quote b)))



(defun append. (x y)
  (cond ((null. x) y)
        ((quote t) (cons (car x) (append. (cdr x) y)))))

(append. (quote (a b)) (quote (c d)))
(append. (quote ()) (quote (c d)))



(defun pair. (x y)
  (cond ((and. (null. x) (null. y)) (quote ()))
        ((and. (not. (atom x)) (not. (atom y)))
         (cons (cons (car x)
                     (cons (car y) (quote ())))
               (pair. (cdr x) (cdr y))))))

(pair. (quote (x y z)) (quote (a b c)))



(defun assoc. (x y)
  (cond ((eq (car (car y)) x) (car (cdr (car y))))
        ('t (assoc. x (cdr y)))))

(assoc. (quote x) (quote ((x a) (y b))))
(assoc. (quote x) (quote ((x new) (x a) (y b))))



(defun caar   (l) (car (car   l)))
(defun cadr   (l) (car (cdr   l)))
(defun cdar   (l) (cdr (car   l)))
(defun cddr   (l) (cdr (cdr   l)))
(defun cadar  (l) (car (cdar  l)))
(defun caddr  (l) (car (cddr  l)))
(defun cddar  (l) (cdr (cdar  l)))
(defun caddar (l) (car (cddar l)))

(cadr  (quote ((a b) (c d) e)))
(caddr (quote ((a b) (c d) e)))
(cdar  (quote ((a b) (c d) e)))



(defun list-1 (a)     (cons a (quote ())))
(defun list-2 (a b)   (cons a (list-1 b)))
(defun list-3 (a b c) (cons a (list-2 b c)))

(list-3 (quote 1) (quote 2) (quote 3))



(defun eval. (e a)
  (cond
    ((atom e) (assoc. e a))
    ((atom (car e))
     (cond
       ((eq (car e) (quote quote)) (cadr e))
       ((eq (car e) (quote atom))  (atom   (eval. (cadr  e) a)))
       ((eq (car e) (quote eq))    (eq     (eval. (cadr  e) a)
                                           (eval. (caddr e) a)))
       ((eq (car e) (quote car))   (car    (eval. (cadr  e) a)))
       ((eq (car e) (quote cdr))   (cdr    (eval. (cadr  e) a)))
       ((eq (car e) (quote cons))  (cons   (eval. (cadr  e) a)
                                           (eval. (caddr e) a)))
       ((eq (car e) (quote cond))  (evcon. (cdr e) a))
       ((quote t) (eval. (cons (assoc. (car e) a)
                               (cdr e))
                         a))))
    ((eq (caar e) (quote label))
     (eval. (cons (caddar e) (cdr e))
            (cons (list-2 (cadar e) (car e)) a)))
    ((eq (caar e) (quote lambda))
     (eval. (caddar e)
            (append. (pair. (cadar e) (evlis. (cdr e) a))
                     a)))))

(defun evcon. (c a)
  (cond ((null. c) (quote ()))
        ((eval. (caar  c) a)
         (eval. (cadar c) a))
        ((quote t) (evcon. (cdr c) a))))

(defun evlis. (m a)
  (cond ((null. m) (quote ()))
        ((quote t) (cons (eval.  (car m) a)
                         (evlis. (cdr m) a)))))

(eval. (quote x) (quote ((x a) (y b))))
(eval. (quote (eq (quote a) (quote a))) (quote ()))
(eval. (quote (cons x (quote (b c))))
       (quote ((x a) (y b))))
(eval. (quote (cond ((atom x) (quote atom))
                    ((quote t) (quote list))))
       (quote ((x (quote (a b))))))
(eval. (quote (f (quote (b c))))
       (quote ((f (lambda (x) (cons (quote a) x))))))

(eval. (quote ((label firstatom (lambda (x)
                                  (cond ((atom x) x)
                                        ((quote t) (firstatom (car x))))))
         y))
       (quote ((y ((a b) (c d))))))

(eval. (quote ((label firstatom (lambda (x)
                                  (cond ((atom x) x)
                                        ((quote t) (firstatom (car x))))))
         y))
       (quote ((y ((a b) (c d))))))

(eval. (quote ((lambda (x y) (cons x (cdr y)))
               (quote a)
               (quote (b c d))))
       (quote ()))

(eval. (quote (cond ((quote ())
                     (quote ())))))

