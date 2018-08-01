(defclass MAIN::Person
    (is-a USER)
    (slot nname
        (type STRING)
    )
    (slot gender
        (type SYMBOL)
        (allowed-symbols male female)
        (default male)
    )
    (slot age
        (type INTEGER)
        (range 1 150)
    )
)

(defclass MAIN::Student 
    (is-a Person)
    (role concrete)
    (pattern-match reactive)
    (slot grade
        (type FLOAT)
        (range 0.0 100.0)
        (default 50.0)
    )
)
