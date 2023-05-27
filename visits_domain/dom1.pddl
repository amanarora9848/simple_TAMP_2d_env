(define (domain localization)

	(:requirements :typing :durative-actions :numeric-fluents :negative-preconditions :action-costs :conditional-effects :equality :fluents)

	(:types
		robot region
	)

	(:predicates
		(robot_in ?v - robot ?r - region)
		(given_assignments)
		(desk ?r - region)
	)

	(:functions
		(act-cost)
		(triggered ?from ?to - region)
		(assignment_in ?r - region)
		(collected)
		(total_assignments)
		(dummy)
	)

	(:durative-action goto_region
		:parameters (?v - robot ?from ?to - region)
		:duration (= ?duration 100)
		:condition (and (at start (robot_in ?v ?from)))
		:effect (and (at start (not (robot_in ?v ?from))) (at start (increase (triggered ?from ?to) 1))
			(at end (robot_in ?v ?to)) (at end (assign (triggered ?from ?to) 0))
			(at end (increase (act-cost) (dummy))))
	)

	(:action collect
		:parameters (?v - robot ?r - region)
		:precondition (and (robot_in ?v ?r) (> (assignment_in ?r) 0))
		:effect (and (decrease (assignment_in ?r) 1) (increase (collected) 1))
	)

	(:action deliver
		:parameters (?v - robot ?r - region)
		:precondition (and (robot_in ?v ?r) (desk ?r) (>= (collected) (total_assignments)))
		:effect (and (assign (collected) 0) (given_assignments))
	)

	; (:durative-action goto_collect
	; 	:parameters (?v - robot ?from ?to - region)
	; 	:duration (= ?duration 100)
	; 	:condition (and (at start (robot_in ?v ?from)) (at start (not (desk ?to))))
	; 	:effect (and (at start (not (robot_in ?v ?from))) (at start (increase (triggered ?from ?to) 1))
	; 		(at end (robot_in ?v ?to)) (at end (assign (triggered ?from ?to) 0))
	; 		(at end (decrease (assignment_in ?to) 1)) (at end (increase (collected) 1))
	; 		(at end (increase (act-cost) (dummy)))
	; 	)
	; )

	; (:durative-action goto_deliver
	; 	:parameters (?v - robot ?from ?to - region)
	; 	:duration (= ?duration 100)
	; 	:condition (and (at start (robot_in ?v ?from)) (at start (desk ?to)))
	; 	:effect (and (at start (not (robot_in ?v ?from))) (at start (increase (triggered ?from ?to) 1))
	; 		(at end (robot_in ?v ?to)) (at end (assign (triggered ?from ?to) 0))
	; 		(at end (assign (collected) 0)) (at end (given_assignments))
	; 		(at end (increase (act-cost) (dummy)))
	; 	)
	; )
)