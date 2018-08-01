(defrule rul-001-comehome
    ?f <- (scene-mode (zone room1) (curr comehome) (prev ?prev))
  =>
    (bind ?c (create-rule-context rul-001-comehome))
    (send ?c act-control ins-00a17a88-a01a-02a1-4af9 value 1)
)
