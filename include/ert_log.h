/* ERT_LOG.h Error, warning and debugging logs
 *
 * (c) 2018 Ellis Rhys Thomas <e.rhys.thomas@gmail.com>
 * Licence: Public Domain
 *
 * Contains macros from simple logging. Verbosity of logging is
 * dictated by the value of LOGLEVEL:
 * 
 *     LOGLEVEL == 0, only error messages
 *     LOGLEVEL == 1, extra warning messages
 *     LOGLEVEL == 2, extra verbose information
 *     LOGLEVEL == 3, extra debugging information
 *
 * When LOGLEVEL is greater than 0, logs have ANSI color formatting.
 * Error and warning macros reset errno
 * 
 */

#ifndef ERT_LOG_H
#define ERT_LOG_H

#include <stdio.h>
#include <errno.h>
#include <string.h>		/* for strerror() */

#define RED        "\x1b[1;31m"
#define YELLOW     "\x1b[1;33m"
#define GREEN      "\x1b[1;32m"
#define CYAN       "\x1b[1;36m"
#define BOLD       "\x1b[1m"
#define ANSI_RESET "\x1b[0m"

#ifndef LOGLEVEL
#define LOGLEVEL 0
#endif

#if LOGLEVEL <= 0		/* Always present - Error messages */

#define log_err(MSG, ...) {					\
	fprintf(stderr, "[ERROR] " MSG, ##__VA_ARGS__);		\
	fprintf(stderr, " (%s)\n",				\
		errno == 0 ? "No errno" : strerror(errno));	\
	errno = 0;						\
    }								\
	
#endif	/* LOGLEVEL == 0 */

#if LOGLEVEL >= 1		/* Level 1 - Warnings */
#define ERT_LOG(STREAM, TITLE, MSG, ...) {			\
	fprintf(STREAM, TITLE					\
		" in " BOLD "%s" ANSI_RESET			\
		" at " BOLD "%s:%d: " ANSI_RESET  MSG,		\
		__func__, __FILE__, __LINE__, ##__VA_ARGS__); }

#define ERRNO_MSG() {						\
	fprintf(stderr, BOLD);					\
	fprintf(stderr, " (%s)\n",				\
		errno == 0 ? "No errno" : strerror(errno));	\
	fprintf(stderr, ANSI_RESET);				\
	errno = 0; }

#define log_err(MSG, ...) {				\
	ERT_LOG(stderr, RED "[ERROR]" ANSI_RESET,	\
		MSG, ##__VA_ARGS__);			\
	ERRNO_MSG(); }

#define log_warn(MSG, ...) {				\
	ERT_LOG(stderr, YELLOW "[WARN]" ANSI_RESET,	\
		MSG, ##__VA_ARGS__);			\
	ERRNO_MSG(); }
#else
#define log_warn(MSG, ...) {}
#endif	/* LOGLEVEL >= 1 */

#if LOGLEVEL >= 2		/* Level 2 - Information */
#define log_info(MSG, ...) {				\
	ERT_LOG(stdout, GREEN "[INFO]" ANSI_RESET,	\
		MSG "\n", ##__VA_ARGS__); }
#else
#define log_info(MSG, ...) {}
#endif	/* LOGLEVEL >= 2 */

#if LOGLEVEL >= 3		/* Level 3 - Debugging information */
#define log_debug(MSG, ...) {				\
	ERT_LOG(stdout, CYAN "[DEBUG]" ANSI_RESET,	\
		MSG "\n", ##__VA_ARGS__); }
#else
#define log_debug(MSG, ...) {}
#endif	/* LOGLEVEL >= 3 */

#endif	/* ERT_LOG_H */
