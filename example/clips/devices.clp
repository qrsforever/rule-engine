(defclass oic.d.envsensor
    (is-a DEVICE)
    (role concrete) (pattern-match reactive)
    (multislot humidity (type NUMBER) (visibility public) (cardinality 2 2))
    (multislot illuminate (type NUMBER) (visibility public) (cardinality 2 2))
    (multislot temper (type NUMBER) (visibility public) (cardinality 2 2))
)

(defclass oic.d.sosalarm
    (is-a DEVICE)
    (role concrete) (pattern-match reactive)
    (multislot alarm (type NUMBER) (visibility public) (cardinality 2 2)) 
)

(defclass oic.d.doorcontact
    (is-a DEVICE)
    (role concrete) (pattern-match reactive)
    (multislot status (type NUMBER) (visibility public) (cardinality 2 2))
)

(defclass oic.d.light
    (is-a DEVICE)
    (role concrete) (pattern-match reactive)
    (multislot brightness (type NUMBER) (visibility public) (cardinality 2 2))
    (multislot ct (type NUMBER) (visibility public) (cardinality 2 2))
    (multislot value (type NUMBER) (visibility public) (cardinality 2 2))
    (multislot hue (type NUMBER) (visibility public) (cardinality 2 2))
    (multislot saturation (type NUMBER) (visibility public) (cardinality 2 2))
)

(defclass oic.d.tv
    (is-a DEVICE)
    (role concrete) (pattern-match reactive)
    (multislot switch (type NUMBER) (visibility public) (cardinality 2 2))
)
