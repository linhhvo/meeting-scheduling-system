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

/**
 * \brief scheduling actions
 */

int findMeeting(Meeting *meetingList, int listSize, int targetMonth,
                int targetDay, int targetHour);

int init_meeting(Meeting *newMeeting, char *inputDesc, int inputMonth,
                 int inputDay, int inputHour);

int addMeeting(Meeting **meetingList, int *listSize, char *userInput);

int deleteMeeting(Meeting **meetingList, int *listSize, char *userInput);

void displayMeetings(Meeting *meetingList, int listSize);

int numOfArgs(char *userInput);

int isNumeric(char *str);

int validateArguments(char *monthStr, char *dayStr, char *hourStr);

int findInsertPos(Meeting *meetingList, int listSize, int targetMonth,
                  int targetDay, int targetHour);

int writeToFile(char *userInput, Meeting *meetingList, int listSize);

void freeList(Meeting *list, int listSize);

int loadFromFile(char *userInput, Meeting **meetingList, int *listSize);

int performAction(char *userInput, Meeting **meetingList, int *listSize);

#endif //! _PROJECT__H_
