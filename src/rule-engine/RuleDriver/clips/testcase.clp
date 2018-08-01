;=================================================================
; date: 2018-06-30 09:54:55
; title: testcase
;=================================================================

; case template as message
(deftemplate testcase
    (slot what (type STRING))
    (slot arg1 (type INTEGER))
    (slot arg2 (type INTEGER))
    (multislot obj)
)

(defclass Door
  (is-a DEVICE)
  (role concrete) (pattern-match reactive)
  (slot switch (type INTEGER) (allowed-integers 0 1))
)

(defclass Light
  (is-a DEVICE)
  (role concrete) (pattern-match reactive)
  (slot switch (type INTEGER) (allowed-integers 0 1))
  (slot temprature (type FLOAT) (range -15.0 95.0))
  (slot color (type SYMBOL) (allowed-symbols red geen blue))
)

; Test: do control when instance matched
(defrule rul-001 "test1"
    (object (is-a Light)
       (ID ?id &:(eq ?id ins-DC330D799327))
       (switch ?switch &:(= ?switch 1))
     )
  =>
    (bind ?context (create-rule-context rul-001 1000 2))
    (if (eq ?context FALSE) then (return))
    (send ?context act-control ins-0007A895C7C7 switch 1)
    (send ?context act-control ins-DC330D799328 color blue)
    (send ?context act-notify 10000001 "tellYou" "Girlfriend Birthday")
    (send ?context act-scene rul-002)
)

; Test: manually
(defrule rul-002 "test2"
    ?f <- (scene rul-002)
  =>
    (retract ?f)
    (bind ?context (create-rule-context rul-002 2000 2))
    (if (eq ?context FALSE) then (return))
    (send ?context act-notify 10000001 "tellYou" "Girlfriend Birthday")
)

;-----------------------------------------------------------------
;   Test Case Rule
;-----------------------------------------------------------------

(defrule test-suite
    ?f <- (test-suite init)
  =>
    (assert (testcase (what "rul-001") (arg1 0)))
    (retract ?f)
)

(defrule testcase-handler
    ?f <- (testcase (what ?what) (arg1 ?arg1) (arg2 ?arg2))
  =>
    (retract ?f)
    (printout t "(testcase (what "?what") (arg1 "?arg1") (arg2 "?arg2"))" crlf)
    (switch ?what
     (case "rul-001"
      then
        (switch ?arg1
         (case 0 ; create instance
          then
            (printout info "create instances" crlf)
            (make-instance ins-DC330D799327 of Light (switch 0))
            (make-instance ins-DC330D799328 of Light (switch 0))
            (make-instance ins-0007A895C7C7 of Door (switch 0))
         )
         (case 1 ; update new value (trigger the rul-001)
          then
            (send [ins-DC330D799327] put-switch 1)
         )
         (case 2 ; update new value (not trigger rule, instance is wrong)
          then
            (send [ins-DC330D799328] put-color blue)
         )
         (case 3 ; trigger inner rule to response the rul-001 result
          then
            (send [ins-0007A895C7C7] put-switch 1)
         )
         (default ; enter next case
          then
            (assert (testcase (what "rul-002") (arg1 0) (arg2 0)))
            (return TRUE)
         )
        )
        ; goto next case
        (assert (testcase (what ?what) (arg1 (+ ?arg1 1)) (arg2 ?arg2)))
        (agenda)
        (return TRUE)
     )
     (case "rul-002"
      then
        (switch ?arg1
         (case 0 ; trigger rule 002
          then
            (assert (scene rul-002))
         )
         (case 1
          then
         )
         (default ; enter next case
          then
            (return TRUE)
         )
        )
        ; goto next case
        (assert (testcase (what ?what) (arg1 (+ ?arg1 1)) (arg2 ?arg2)))
        (agenda)
        (return TRUE)
     )
    )
    (printout t "Unkown testcase !!!" crlf)
    (return FALSE)
)
