(defrule rul-001-leavehome
    ?f <- (scene-mode (zone room1) (curr leavehome) (prev ?prev))
  =>
    (bind ?c (create-rule-context rul-001-leavehome))
    (send ?c act-control ins-00a17a88-a01a-02a1-4af9 value 0)
)
