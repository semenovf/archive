/*
 * timer.h
 *
 *  Created on: Nov 25, 2015
 *      Author: wladt
 */

#ifndef __TIMER_H__
#define __TIMER_H__

#define MAX_NTIMERS 16

enum timer_type_enum {
	  Timer_Invalid
	, Timer_OneShot
	, Timer_Periodic
};

#ifndef __cplusplus
typedef enum timer_type_enum timer_type_enum;
#endif

typedef int timer_id_t;

#ifdef __cplusplus
extern "C" {
#endif

int timer_init   (void);

/**
 * @brief Set timer.
 * @param seconds Specified timeout for timer.
 * @return Timer identifier.
 */
int timer_set    (timer_type_enum type, const double seconds);
int timer_passed (const int id);
int timer_unset  (const int id);
void timer_unset_all  (void);
int timer_done   (void);
int timer_count  (void);

/**
 * @brief Resets the timer: periodic will restarted, one-shot will unset.
 * @param id Timer identifier.
 */
void timer_reset (const int id);

/**
 * @brief Checks if timer is periodic.
 * @param id Timer identifier.
 * @return @c true if timer is periodic, @c false otherwise.
 */
int timer_is_periodic (const int id);

/**
 * @brief Checks if timer is one-shot.
 * @param id Timer identifier.
 * @return @c true if timer is one-shot, @c false otherwise.
 */
int timer_is_oneshot  (const int id);

#ifdef __cplusplus
}
#endif

#endif /* __TIMER_H__ */
