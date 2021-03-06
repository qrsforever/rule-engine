;=================================================================
; date: 2018-06-14 19:11:27
; title: All Global Construct
;=================================================================

;-----------------------------------------------------------------
;    Global varible
;       1. Use ?*var* to access the value of global varible
;       2. Use bind function to set the value of global varible.
;-----------------------------------------------------------------

; Salience Level Define
(defglobal
    ?*SALIENCE-HIGHEST*  =  10000
    ?*SALIENCE-HIGHER*   =   5000
    ?*SALIENCE-HIGH*     =   1000
    ?*SALIENCE-NORMAL*   =      0
    ?*SALIENCE-LOWER*    =  -1000
    ?*SALIENCE-LOWEST*   = -10000
)

; Log Level Define: must be consisence with defined in Log.h
(defglobal
    ?*LOG-LEVEL-ERROR*      = 1
    ?*LOG-LEVEL-WARNING*    = 2
    ?*LOG-LEVEL-DEBUG*      = 3
    ?*LOG-LEVEL-INFO*       = 4
    ?*LOG-LEVEL-TRACE*      = 5
)

; varibles: rule engine
(defglobal
    ?*RULE-TIMEOUT-MS*      = 3000
    ?*RULE-RETRY-COUNT*     = 1
)

;-----------------------------------------------------------------
;   Global Template
;-----------------------------------------------------------------

; scene mode
(deftemplate scene-mode
    (slot zone (type SYMBOL))
    (slot prev (type SYMBOL) (default none))
    (slot curr (type SYMBOL) (default none))
)

; timer event
(deftemplate timer-event
    (slot id)
)

;-----------------------------------------------------------------
;    Global Class
;-----------------------------------------------------------------

(defclass Context (is-a USER))

; Base Device Abstract
(defclass DEVICE (is-a USER) (role abstract)
    (slot ID     (visibility public) (type SYMBOL))
    (slot Class  (visibility public) (type SYMBOL) (access initialize-only))
    (slot UUID   (visibility public) (type STRING))
)

(defmessage-handler DEVICE init after ()
    (bind ?self:ID (instance-name-to-symbol (instance-name ?self)))
    (bind ?self:Class (class ?self))
)

; Sub Device define slot must specify facet: (visibility public)
(defmessage-handler DEVICE putData (?slot ?value)
    (bind ?old (nth$ 1 (dynamic-get ?slot)))
    (dynamic-put ?slot ?value ?old)
)

(defmessage-handler DEVICE getData (?slot)
    (return (nth$ 1 (dynamic-get ?slot)))
)

;-----------------------------------------------------------------
;    Global Rule
;-----------------------------------------------------------------

; show facts, ruels, instances and so on debug info
(defrule show-elem
    ?f <- (show ?elem)
  =>
    (retract ?f)
    (switch ?elem
        (case instances then    (instances))
        (case facts     then    (facts))
        (case rules     then    (rules))
        (case agenda    then    (agenda))
        (case classes   then    (list-defclasses))
        (case globals   then    (show-defglobals))
        (case memory    then    (printout info "Memory Used:"(/ (mem-used) 131072)" MB" crlf))
        (default (printout warn "Unkown elem: " ?elem crlf))
    )
)

; (datetime (now)) from program
(defrule retract-datetime
    (declare (salience ?*SALIENCE-LOWEST*))
    ?f <- (datetime $?)
  =>
    (if (>= ?*LOG-LEVEL* ?*LOG-LEVEL-TRACE*)
      then
        (facts)
    )
    (retract ?f)
)

; retract fact: (action-reponse id value)
(defrule retract-rule-response
    (declare (salience ?*SALIENCE-LOWEST*))
    ?f <- (rule-response ?id $?)
  =>
    (retract ?f)
)

; trigger scene enter
(defrule scene-enter-mode
    (declare (salience ?*SALIENCE-HIGH*))
    ?f <- (scene-enter ?zone ?mode)
  =>
    (retract ?f)
    (bind ?fm (nth$ 1 (find-fact ((?sm scene-mode)) (eq ?sm:zone ?zone))))
    (if (neq ?fm nil)
     then
        (bind ?curr (fact-slot-value ?fm curr))
        (modify ?fm (prev ?curr) (curr ?mode))
     else
        (assert (scene-mode (zone ?zone) (curr ?mode)))
    )
)

; remove the timer-event
(defrule remove-timer-event
    (declare (salience ?*SALIENCE-LOWEST*))
    ?f <- (remove-timer-event ?id)
  =>
    (retract ?f)
    (bind ?fact (nth$ 1 (find-fact ((?t timer-event)) (eq ?t:id ?id))))
    (if (neq ?fact nil)
     then
        (retract ?fact)
    )
)
