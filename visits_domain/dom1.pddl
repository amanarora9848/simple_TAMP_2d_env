(define (domain localization)

(:requirements :strips :typing :durative-actions :numeric-fluents :negative-preconditions :action-costs :conditional-effects :equality :fluents )


(:types 	robot region 
)

(:predicates
		(robot_in ?v - robot ?r - region) (visited ?r - region )
		(assignment_in ?r - region)
		(is_desk ?r - region)
		(delivered)
	      
)

(:functions 
		(act-cost) (triggered ?from ?to - region) (dummy) (carrying ?r - robot)
)

(:durative-action goto_region
		:parameters (?v - robot ?from ?to - region)
		:duration (= ?duration 100)
		:condition (and (at start (robot_in ?v ?from)))
	        :effect (and (at start (not (robot_in ?v ?from))) (at start (increase (triggered ?from ?to) 1))
		(at end (robot_in ?v ?to)) (at end (assign (triggered ?from ?to) 0)) (at end (visited ?to)) 	
                (at end (increase (act-cost) (dummy))))
)

(:action aux
	:parameters (?r - robot ?l - region)
	:precondition (and (robot_in ?r ?l) (delivered))
	:effect (and (not (delivered)))
)


; (:action pick_up
; 	:parameters (?r - robot ?l - region)
; 	:precondition (and (robot_in ?r ?l) (assignment_in ?l))
; 	:effect (and (increase (carrying ?r) 1) (not (assignment_in ?l)))
; )

;;(:durative-action localize
;; ...................
;;)



)

