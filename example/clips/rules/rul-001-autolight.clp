(defrule rul-001-autolight
    (object (is-a oic.d.envsensor)
        (ID ?id &:(eq ?id ins-00124b00-12d2-6263-d0d0))
        (illuminate ?ill ?ill-old)
    )
    ?ins <- (object (is-a oic.d.light)
        (ID ?id &:(eq ?id ins-00a17a88-a01a-03a3-6a2b))
    )
    (scene-mode (zone room1) (curr ?curr &:(neq ?curr sleepmode)))
  =>
    (bind ?c (create-rule-context rul-001-autolight))
    (switch ?curr
         (case entermovie then (bind ?target 50))
         (default (bind ?target 30))
    )
    (bind ?delta (* ?target 0.1))
    (bind ?low (- ?target ?delta))
    (bind ?high (+ ?target ?delta))
    (bind ?bright (send ?ins getData brightness))
    (printout t "illuminate: " ?ill " target: " ?target " brightness: " ?bright crlf)
    (if (> ?ill ?high)
      then
            (if (> ?bright 0)
              then
                    (bind ?nv (integer (- ?bright (/ ?bright 2))))
                    (if (> ?nv 5)
                      then
                            (while (> ?bright ?nv)
                                  (bind ?bright (- ?bright 5))
                                  (send ?c act-control ?id brightness ?bright)
                            )
                      else
                            (send ?c act-control ?id brightness 0)
                    )
            )
            (return)
    )
    (if (< ?ill ?low)
      then
            (if (< ?bright 100)
              then
                   (bind ?nv (integer (+ ?bright (/ (- 100 ?bright) 2))))
                   (if (< ?nv  95)
                     then
                            (while (< ?bright ?nv)
                                  (bind ?bright (+ ?bright 5))
                                  (send ?c act-control ?id brightness ?bright)
                            )
                     else
                          (send ?c act-control ?id brightness 100)
                  )
            )
            (return)
    )
)
