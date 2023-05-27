(define (domain localization)

(:requirements :typing :durative-actions :numeric-fluents :equality :fluents)


(:types 	
	robot 
	region 
)

(:predicates
		(robot_in ?v - robot ?r - region)
		(is_desk ?r - region)
		(delivered)
	      
)

(:functions 
		(act-cost) (triggered ?from ?to - region) (dummy) (carrying ?r - robot) (assignment_in ?r - region)
)

(:durative-action goto_region
		:parameters (?v - robot ?from ?to - region)
		:duration (= ?duration 100)
		:condition (and (at start (robot_in ?v ?from)))
	    :effect (and (at start (not (robot_in ?v ?from))) (at start (increase (triggered ?from ?to) 1))
		(at end (robot_in ?v ?to)) (at end (assign (triggered ?from ?to) 0)) (at end (increase (act-cost) (dummy))))
)

(:action pick_up
	:parameters (?r - robot ?l - region)
	:precondition (and (robot_in ?r ?l) (> (assignment_in ?l) 0))
	:effect (and (increase (carrying ?r) 1) (decrease (assignment_in ?l) 1))
)

(:action deliver
	:parameters (?r - robot ?l - region)
	:precondition (and (>= (carrying ?r) 2) (robot_in ?r ?l) (is_desk ?l))
	:effect (and (delivered))
)




;;(:durative-action localize
;; ...................
;;)



)

