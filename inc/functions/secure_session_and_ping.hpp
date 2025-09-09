#ifndef SECURE_SESSION_AND_PING_HPP
#define SECURE_SESSION_AND_PING_HPP

/**
 * @file secure_session_and_ping.hpp
 * @author Wuard
 * @brief This function establishes a secure communication and sends a ping and the tropic returns it.
 *        Based on the 'lt_ex_hello_world_separate_API.c' example from the libtropic library.
 *
 * @license For the license see file LICENSE.txt file in the root directory of this source tree.
 */

extern "C" {
    #include "libtropic.h"
    #include "libtropic_examples.h"
    #include "libtropic_logging.h"
    #include "lt_l2.h"
    #include "lt_l3.h"
}


#include <Arduino.h>


/** @brief Message to send with Ping L3 command. */
#define PING_MSG "This is Hello World message from TROPIC01!!"
/** @brief Size of the Ping message, including '\0'. */
#define PING_MSG_SIZE 44


int lt_secure_session_and_ping(lt_handle_t *h);


#endif