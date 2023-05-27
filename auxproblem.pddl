(define (problem prob1)
(:domain localization)
(:objects
     r0 r1 r2 r3 r4 r5 - region
     R2D2 - robot
)

(:init
    (robot_in R2D2 r0)
    (= (act-cost) 0)
    (= (dummy) 0)
    (= (carrying R2D2) 0)
    (= (assignment_in r3) 1)
    (= (assignment_in r1) 1)
    (is_desk r5)
)

(:goal 
     (and (delivered))
)

; (:metric minimize (act-cost) )
)


