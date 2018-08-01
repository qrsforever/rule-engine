;=================================================================
; date: 2018-06-14 18:55:54
; title: utils
;=================================================================

;-----------------------------------------------------------------
;   Log function define
;-----------------------------------------------------------------
(deffunction loge ($?args)
    (if (>= ?*LOG-LEVEL* ?*LOG-LEVEL-ERROR*)
     then
        (printout error $?args crlf)
    )
)

(deffunction logw ($?args)
    (if (>= ?*LOG-LEVEL* ?*LOG-LEVEL-WARNING*)
     then
        (printout warn $?args crlf)
    )
)

(deffunction logd ($?args)
    (if (>= ?*LOG-LEVEL* ?*LOG-LEVEL-DEBUG*)
     then
        (printout debug $?args crlf)
    )
)

(deffunction logi ($?args)
    (if (>= ?*LOG-LEVEL* ?*LOG-LEVEL-INFO*)
     then
        (printout info $?args crlf)
    )
)

(deffunction logt ($?args)
    (if (>= ?*LOG-LEVEL* ?*LOG-LEVEL-TRACE*)
     then
        (printout trace $?args crlf)
    )
)

;-----------------------------------------------------------------
;   Load file function define
;-----------------------------------------------------------------

(deffunction load-file (?file)
    ; (bind ?pos (str-index "." ?file))
    ; (if (neq ?pos FALSE)
    ;  then
    ;     (bind ?slash (str-index "/" ?file))
    ;     (bind ?path (resolve-file ?file ?slash))
    ;     (if (eq ?path FALSE)
    ;      then
    ;         (return)
    ;     )
    ;     (bind ?suffix (sub-string (+ ?pos 1) (length ?file) ?file))
    ;     (switch ?suffix
    ;         (case "clp"
    ;          then
    ;             (load* ?path)
    ;         )
    ;         (case "bat"
    ;          then
    ;             (batch* ?path)
    ;         )
    ;         (default (printout warn "Cannot load file: " ?file crlf))
    ;     )
    ;  else
    ;     (printout warn "Only handle .clp or .bat files!" crlf)
    ; )
    (bind ?slash (str-index "/" ?file))
    (bind ?path (resolve-file ?file ?slash))
    (if (eq ?path FALSE)
     then
        (return)
    )
    (load* ?path)
)

(deffunction load-files ($?file-list)
    (bind ?end (length$ $?file-list))
    (loop-for-count (?n 1 ?end)
     do
        (load-file (nth$ ?n $?file-list))
    )
)

;-----------------------------------------------------------------
; Datetime utils
;-----------------------------------------------------------------

; datetime.clock compare
(deffunction timeout(?t1 ?t2 ?timeout)
    return (> (- ?t1 ?timeout) ?t2)
)

;-----------------------------------------------------------------
;   String utils
;-----------------------------------------------------------------

; 1 --> "1" or 1.0 --> "1.0"
(deffunction number-to-string (?num)
    (if (integerp ?num)
     then
        (bind ?num (format nil "%d" ?num))
     else
        (if (floatp ?num)
         then
            (bind ?num (format nil "%f" ?num))
        )
    )
    (return ?num)
)

; "aaa "bbb"" ---> "aaa \"bbb\""
(defmethod escape-quote ((?str STRING))
    (bind ?str-len (str-length ?str))
    (if (bind ?pos (str-index "\"" ?str))
     then
        (str-cat (sub-string 1 (- ?pos 1) ?str)
         "\\\""
         (escape-quote (sub-string (+ ?pos 1) ?str-len ?str))
        )
     else
        (return ?str)
    )
)

; "aaa \"bbb\" ---> "aaa "bbb""
(defmethod unescape-quote ((?str STRING))
    (bind ?str-len (str-length ?str))
    (if (bind ?pos (str-index "\\\"" ?str))
     then
        (str-cat (sub-string 1 (- ?pos 1) ?str)
         (unescape-quote (sub-string (+ ?pos 1) ?str-len ?str))
        )
     else
        (return ?str)
    )
)

;-----------------------------------------------------------------
;   Message push
;-----------------------------------------------------------------
(defmethod send-message ((?what INTEGER) (?arg1 INTEGER))
    (msg-push ?what ?arg1 "nil" "nil")
)

(defmethod send-message ((?what INTEGER) (?arg1 INTEGER) (?message STRING))
    (msg-push ?what ?arg1 "nil" ?message)
)

(defmethod send-message ((?what INTEGER) (?arg2 SYMBOL))
    (msg-push ?what 0 ?arg2 "nil")
)

(defmethod send-message ((?what INTEGER) (?arg2 SYMBOL) (?message STRING))
    (msg-push ?what 0 ?arg2 ?message)
)

(defmethod send-message ((?what INTEGER) (?arg1 INTEGER) (?arg2 SYMBOL) (?message STRING))
    (msg-push ?what ?arg1 ?arg2 ?message)
)

;-----------------------------------------------------------------
;   Rule utils
;-----------------------------------------------------------------

(deffunction make-rule (?name ?salience ?LHS ?RHS)
    (bind ?genrule
        (str-cat (format nil "(defrule %s " ?name)
            (format nil "(declare (salience %d)) " ?salience)
            (format nil "%s => %s)" ?LHS ?RHS)
        )
    )
    (if (>= ?*LOG-LEVEL* ?*LOG-LEVEL-INFO*)
     then
        (logi "make-rule:" ?genrule)
    )
    ; (if (check-syntax ?genrule)
     ; then
     (return (build ?genrule))
    ; )
    ; (return FALSE)
)

; delete one rule in clp script when (assert (delete-rule ruleid-1 ruleid-2))
(defrule delete-rule
    ?f <- (delete-rule $?ruleid-list)
  =>
    (retract ?f)
    (foreach ?ruleid (create$ $?ruleid-list)
        (if (defrule-module ?ruleid)
         then
            (undefrule ?ruleid)
        )
    )
)
