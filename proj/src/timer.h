#ifndef __TIMER_H
#define __TIMER_H

typedef struct {
	int counter;
	int ticked;
} Timer;

Timer* newTimer();
void resetTimer(Timer* timer);
void deleteTimer(Timer* timer);

/** @defgroup timer timer
 * @{
 *
 * Functions for using the i8254 timers
 */

/**
 * @brief Configures a timer to generate a square wave
 * 
 * Does not change the LSB (BCD/binary) of the timer's control word.
 * 
 * @param timer Timer to configure. (Ranges from 0 to 2)
 * @param freq Frequency of the square wave to generate
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_set_square(unsigned long timer, unsigned long freq);

/**
 * @brief Subscribes and enables Timer 0 interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int timer_subscribe_int(void );

/**
 * @brief Unsubscribes Timer 0 interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_unsubscribe_int();

/**
 * @brief Timer 0 interrupt handler
 *
 * Increments counter
 */
void timer_handler(Timer* timer);



#endif /* __TIMER_H */
