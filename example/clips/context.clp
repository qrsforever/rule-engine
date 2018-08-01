;=================================================================
; date: 2018-07-02 17:32:48
; title: context
;=================================================================

; Rule Context
(defclass RuleContext (is-a Context) (role concrete)
    (slot rule-id (type STRING))
    (slot timeout-ms (type INTEGER) (default ?*RULE-TIMEOUT-MS*))
    (slot retry-count (type INTEGER) (default ?*RULE-RETRY-COUNT*))
    (slot current-try (type INTEGER) (default 0))
    (slot start-time (type INTEGER) (default-dynamic (nth$ 1 (now))))
    (slot act-error (type SYMBOL) (default false) (allowed-symbols true false))
    (multislot unanswer-list (type STRING))
    (multislot response-rules (type SYMBOL))
)

(defmessage-handler RuleContext init after ()

)

(defmessage-handler RuleContext delete before ()
    (foreach ?act (create$ ?self:unanswer-list)
        (logi "unanswer actions: " ?act)
    )
    (foreach ?rul (create$ ?self:response-rules)
        (logi "undefrule: " ?rul)
        (undefrule ?rul)
    )
    (return TRUE)
)

(defmessage-handler RuleContext try-again(?clock)
    (logd ?self:rule-id " try " ?self:current-try "/" ?self:retry-count)
    (if (= ?self:current-try ?self:retry-count)
     then
        (return FALSE)
    )
    (bind ?self:current-try (+ ?self:current-try 1))
    (foreach ?act (create$ ?self:unanswer-list)
        (bind ?funcall (explode$ ?act))
        (bind ?cmd (nth$ 1 ?funcall))
        (bind $?args (rest$ ?funcall))
        (logd "run " ?funcall " again.")
        (send ?self ?cmd (expand$ $?args))
    )
    (bind ?self:start-time ?clock)
    (return TRUE)
)

(defmessage-handler RuleContext action-success (?str)
    (if (stringp ?str)
     then
        (bind ?pos (member$ ?str ?self:unanswer-list))
        (if (neq ?pos FALSE)
         then
            (slot-direct-delete$ unanswer-list ?pos ?pos)
        )
    )
)

(defmessage-handler RuleContext unanswer-count ()
    (return (length$ ?self:unanswer-list))
)

(defmessage-handler RuleContext is-timeout (?clock)
    (if (= ?self:timeout-ms 0)
     then
        (return FALSE)
    )
    (return (> ?clock (+ ?self:start-time ?self:timeout-ms)))
)

(defmessage-handler RuleContext report-result (?msg)
     (assert (rule-response ?self:rule-id ?msg))
     (switch ?msg
        (case sucess
         then
            (send-message ?*MSG-RULE-RESPONSE* ?*RUL-SUCCESS* ?self:rule-id "success")
        )
        (case fail
         then
            (send-message ?*MSG-RULE-RESPONSE* ?*RUL-FAIL* ?self:rule-id "fail")
        )
        (case timeout
         then
            (send-message ?*MSG-RULE-RESPONSE* ?*RUL-TIMEOUT* ?self:rule-id "timeout")
        )
        (default (logd "report result: unkown " ?msg))
     )
)

(defmessage-handler RuleContext act-step-control (?id ?slot ?end ?begin ?step)
    (if (or
         (not (numberp ?end))
         (not (numberp ?begin))
         (not (numberp ?step))
        )
     then
        (logw "Parameters: invalid (" ?id ?slot ?end ?begin ?step ")")
        (bind ?act-error true)
        (return)
    )
    (if (instance-existp ?id)
     then
        (if (> ?end ?begin)
         then
            (bind ?flag ">=")
            (if (< ?step 0)
             then
                (bind ?step (* -1 ?step))
            )
         else
            (bind ?flag "<")
            (if (> ?step 0)
             then
                (bind ?step (* -1 ?step))
            )
        )
        (bind ?rulname (sym-cat "_"?self:rule-id"-response-" ?id))
        (bind ?action-str (format nil "act-step-control %s %s %d %d %d" ?id ?slot ?end ?begin ?step))
        (bind ?RHS (str-cat
                    (format nil "%n(if (= ?current %d)%n then%n " ?end)
                    (format nil "(send [%s] action-success \"%s\")" (instance-name ?self) ?action-str)
                    (format nil "%n else%n (if (%s (- %d ?current) %d)" ?flag ?end ?step)
                    (format nil "%n then%n (bind ?value (+ ?current %d))" ?step)
                    (format nil "%n else%n (bind ?value %d)%n )" ?end)
                    (format nil "%n (ins-push \"%s\" \"%s\" (number-to-string ?value))%n)%n" ?id ?slot)
                   ))

        (bind ?pos (member$ ?rulname ?self:response-rules))
        (if (eq ?pos FALSE)
         then
            (logd "act-step-control(" ?id ", " ?slot ", " ?end ", " ?begin ", " ?step ")")
            (if (neq (ins-push ?id ?slot (number-to-string ?begin)) TRUE)
             then
                ; (eval ?RHS)
                (slot-direct-insert$ unanswer-list 1 ?action-str)
                (bind ?clsname (class (symbol-to-instance-name ?id)))
                (bind ?LHS (str-cat (format nil "(object (is-a %s)" ?clsname)
                            (format nil " (ID ?id &:(eq ?id %s))" ?id)
                            (format nil " (%s ?current))" ?slot)
                           ))
                (if (make-rule ?rulname ?*SALIENCE-HIGH* ?LHS ?RHS)
                 then
                    (logi "make rule[" ?rulname "] ok!")
                    (slot-direct-insert$ response-rules 1 ?rulname)
                 else
                    (loge "make rule[" ?rulname "] error!")
                )
            )
        )
     else
        (logw "NOT FOUND: " ?id " instance")
        (bind ?act-error true)
        (send-message ?*MSG-RULE-RHS* ?*RHS-INS-NOT-FOUND* ?self:rule-id (format nil "%s" ?id))
    )
)

; Response:
;   (rule-response ins-id success)
(defmessage-handler RuleContext act-control (?id ?slot ?value $?args)
    (bind ?value (number-to-string ?value))
    (if (instance-existp ?id)
     then
        (bind ?rulname (sym-cat "_"?self:rule-id"-response-" ?id))
        (bind ?action-str (format nil "act-control %s %s %s" ?id ?slot ?value))
        (bind ?RHS (str-cat (format nil "(send [%s]" (instance-name ?self))
                    (format nil " action-success \"%s\")"  ?action-str)
                   ))

        (bind ?pos (member$ ?rulname ?self:response-rules))
        (if (eq ?pos FALSE)
         then
            (logd "act-control(" ?id ", " ?slot ", " ?value ")")
            (if (neq (ins-push ?id ?slot ?value) TRUE)
             then
                (bind ?flag (nth$ 1 $?args))
                (if (eq ?flag 1)
                 then
                    ; (eval ?RHS)
                    (slot-direct-insert$ unanswer-list 1 ?action-str)
                    (bind ?clsname (class (symbol-to-instance-name ?id)))
                    (bind ?LHS (str-cat (format nil "(object (is-a %s)" ?clsname)
                                (format nil " (ID ?id &:(eq ?id %s))" ?id)
                                (format nil " (%s ?v &:(eq ?v %s)))" ?slot ?value)
                               ))
                    (if (make-rule ?rulname ?*SALIENCE-HIGH* ?LHS ?RHS)
                     then
                        (logi "make rule[" ?rulname "] ok!")
                        (slot-direct-insert$ response-rules 1 ?rulname)
                     else
                        (loge "make rule[" ?rulname "] error!")
                    )
                )
            )
        )
     else
        (logw "NOT FOUND: " ?id " instance")
        (bind ?act-error true)
        (send-message ?*MSG-RULE-RHS* ?*RHS-INS-NOT-FOUND* ?self:rule-id (format nil "%s" ?id))
    )
)

; Response:
;   (rule-response notify-id success)
(defmessage-handler RuleContext act-notify (?id ?title ?content $?args)
    (if (and (stringp ?title) (stringp ?content))
     then
        (bind ?id (number-to-string ?id))
        (bind ?rulname (sym-cat "_"?self:rule-id"-response-" ?id))
        (bind ?action-str (implode$ (create$ act-notify ?id ?title ?content)))
        (bind ?RHS (str-cat "(send [" (instance-name ?self) "] action-success \"" (escape-quote ?action-str) "\")"))

        (bind ?pos (member$ ?rulname ?self:response-rules))
        (if (eq ?pos FALSE)
         then
            (logd "act-notify(" ?id ", " ?title ", " ?content ")")
            (if (neq (txt-push ?id ?title ?content) TRUE)
             then
                (bind ?flag (nth$ 1 $?args))
                (if (eq ?flag 1)
                 then
                    ; (eval ?RHS)
                    (slot-direct-insert$ unanswer-list 1 ?action-str)
                    (bind ?LHS (str-cat "(rule-response "?id" success)"))
                    (if (make-rule ?rulname ?*SALIENCE-HIGH* ?LHS ?RHS)
                     then
                        (logi "make rule[" ?rulname "] ok!")
                        (slot-direct-insert$ response-rules 1 ?rulname)
                     else
                        (logi "make rule[" ?rulname "] error!")
                    )
                )
            )
        )
     else
        (logw "Parameters is invalid: (" ?id ", " ?title ", " ?content ")")
        (bind ?act-error true)
        (send-message ?*MSG-RULE-RHS* ?*RHS-NTF-WRONG-TYPE* ?self:rule-id (format nil "%s" ?id))
    )
)

; Response:
;   (rule-response ruleid sucess)
(defmessage-handler RuleContext act-rule (?ruleid $?args)
    (if (defrule-module ?ruleid)
     then
        (bind ?rulname (sym-cat "_"?self:rule-id"-response-" ?ruleid))
        (bind ?action-str (str-cat "act-rule "?ruleid))
        (bind ?RHS (str-cat "(send [" (instance-name ?self) "] action-success \"" (escape-quote ?action-str) "\")"))
        (bind ?pos (member$ ?rulname ?self:response-rules))
        (if (eq ?pos FALSE)
         then
            (logd "(act-rule " ?ruleid ")")
            (assert (rule ?ruleid))
            (bind ?flag (nth$ 1 $?args))
            (if (eq ?flag 1)
             then
                (slot-direct-insert$ unanswer-list 1 ?action-str)
                (bind ?LHS (str-cat "(rule-response "?ruleid" success)"))
                (if (make-rule ?rulname ?*SALIENCE-HIGH* ?LHS ?RHS)
                 then
                    (logi "make rule[" ?rulname "] ok!")
                    (slot-direct-insert$ response-rules 1 ?rulname)
                 else
                    (logi "make rule[" ?rulname "] error!")
                )
            )
        )
     else
        (logw "NOT FOUND: scene:" ?ruleid)
        (bind ?act-error true)
        (send-message ?*MSG-RULE-RHS* ?*RHS-SEE-NOT-FOUND* ?self:rule-id (format nil "%s" ?ruleid))
    )
)

(deffunction create-rule-context (?rule-id $?args)
    (if (instance-existp ?rule-id)
     then
        (logi "create-rule-context, already existp " ?rule-id)
        (return (symbol-to-instance-name ?rule-id))
    )
    (bind ?timeout (nth$ 1 $?args))
    (bind ?trycount (nth$ 2 $?args))
    (if (neq ?timeout nil)
     then
        (if (neq ?trycount  nil)
         then
            (return (make-instance ?rule-id of RuleContext
                     (rule-id ?rule-id)
                     (timeout-ms ?timeout)
                     (retry-count ?trycount)
                    ))
         else
            (return (make-instance ?rule-id of RuleContext
                     (rule-id ?rule-id)
                     (timeout-ms ?timeout)
                    ))
        )
    )
    (return (make-instance ?rule-id of RuleContext (rule-id ?rule-id)))
)

; check rule response
(defrule check-rule-response
    (datetime ?clock $?other)
    ?obj <- (object (is-a RuleContext) (rule-id ?rule-id) (act-error ?error))
  =>
    (if (eq ?error false)
     then
        (if (= (send ?obj unanswer-count) 0)
         then
            (send ?obj report-result sucess)
         else
            (if (send ?obj is-timeout ?clock)
             then
                ; check retry again
                (if (send ?obj try-again ?clock)
                 then
                    (logw ?rule-id " exec timeout, try again.")
                    (return)
                 else
                    (send ?obj report-result timeout)
                )
             else
                ; not timeout, continue
                (return)
            )
        )
     else
        (send ?obj report-result fail)
    )
    ; finish the rule, release resource
    (if (unmake-instance ?obj)
     then
        (logi "unmake-instance context " ?obj " success.")
     else
        (logw "unmake-instance context " ?obj " fail.")
    )
)
