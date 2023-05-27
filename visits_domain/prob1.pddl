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
          (desk r5)
          (= (total_assignments) 2)
          (= (assignment_in r1) 1)
          (= (assignment_in r2) 1)
          (= (assignment_in r3) 1)
          (= (assignment_in r4) 1)
          (= (collected) 0)
     )

     (:goal
          (and
               (given_assignments)
          )
     )

     (:metric minimize
          (act-cost)
     )
)