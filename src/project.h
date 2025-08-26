#ifndef _PROJECT__H_
#define _PROJECT__H_

/**
 * \brief Meeting information
 */
typedef struct {
  char *desc;
  int month;
  int day;
  int hour;
} Meeting;

/**
 * \brief Command options
 */
enum commands {
  ADD = 'A',
  DELETE = 'D',
  LIST = 'L',
  WRITE = 'W',
  LOAD = 'O',
  QUIT = 'Q'
};

#endif //! _PROJECT__H_
