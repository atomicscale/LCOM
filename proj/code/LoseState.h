#pragma once

/** @defgroup LoseState LoseState
 * @{
 * Functions for Lose State
 */

#include "bitmap.h"

typedef struct {
	int done; // specifies if the state is done
	Bitmap* background; // specifies the background image in Lose State
} LoseState;

/**
 * @brief Creates a new Lose State
 *
 * @return Non NULL pointer to the state
 */
LoseState* newLoseState();
/**
 * @brief Updates a Lose State
 *
 * @param state Pointer to LoseState
 * @param scancode Scancode read by the Keyboard
 */
void updateLoseState(LoseState* state, unsigned long scancode);
/**
 * @brief Draws a Lose State
 *
 * @param state Pointer to LoseState
 */
void drawLoseState(LoseState* state);
/**
 * @brief Deletes a Lose State
 *
 * @param state Pointer to LoseState
 */
void deleteLoseState(LoseState* state);
