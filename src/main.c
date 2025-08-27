#include "../include/project.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief Get the number of entered command arguments.
 *
 * \param userInput A string containing user input argument list. Each argument
 * is separated by a space character.
 *
 * \returns The number of separated arguments.
 * \returns 0 if the given string is empty.
 */
int numOfArgs(char *userInput) {

  int count = 0;

  if (strcmp(userInput, "") == 0) {
    free(userInput);
    return 0;
  }

  char *subStr = strtok(userInput, " ");
  while (subStr) {
    count++;
    subStr = strtok(NULL, " ");
  }

  free(userInput);
  return count;
}

/**
 * \brief Check if some data is numeric.
 */
int isNumeric(char *str) {
  char chr = str[0];

  if (!isdigit(chr) && chr != '-') {
    return 0;
  }

  chr = str[1];
  int i = 1;

  while (chr != '\0' && !isspace(chr)) {
    if (!isdigit(chr)) {
      return 0;
    }

    i++;
    chr = str[i];
  }

  return 1;
}

/**
 * \brief Verify that entered meeting time values are valid (within the correct
 * range.)
 *
 * \param monthStr, dayStr, hourStr Entered argument strings for meeting time.
 *
 * \return 0 if any of the time values is out of corrent range.
 * \return 1 if all time values are valid.
 */
int validateArguments(char *monthStr, char *dayStr, char *hourStr) {

  if (!isNumeric(monthStr) || !isNumeric(dayStr) || !isNumeric(hourStr)) {
    printf("Date-time parts must be numeric.\n");
    return 0;
  }

  int month = atoi(monthStr);
  int day = atoi(dayStr);
  int hour = atoi(hourStr);

  if (month < 1 || month > 12) {
    printf("Month cannot be less than 1 or greater than 12.\n");
    return 0;
  }

  if (day < 1 || day > 31) {
    printf("Day cannot be less than 1 or greater than 31.\n");
    return 0;
  }

  if (hour < 0 || hour > 23) {
    printf("Hour cannot be less than 0 or greater than 23.\n");
    return 0;
  }

  return 1;
}

/**
 * \brief Find position in the meeting list to add a new meeting so that the
 * meeting list is sorted in the order of meeting time.
 *
 * \param meetingList The list of scheduled meetings.
 * \param listSize The number of scheduled meetings.
 * \param targetMonth, targetDay, targetHour Time values of a new meeting.
 *
 * \returns The position of the existing meeting if the new meeting time is
 * earlier than the existing meeting.
 * \returns listSize (the last position) if no existing meeting is later than
 * the new meeting time.
 * \returns 0 (the first position) if meeting list is empty.
 */
int findInsertPos(Meeting *meetingList, int listSize, int targetMonth,
                  int targetDay, int targetHour) {

  int pos = 0;

  if (listSize > 0) {
    for (int i = 0; i < listSize; i++) {
      if (targetMonth < meetingList[i].month) {
        pos = i;
        break;
      } else if (targetMonth == meetingList[i].month) {
        if (targetDay < meetingList[i].day) {
          pos = i;
          break;
        } else if (targetDay == meetingList[i].day) {
          if (targetHour < meetingList[i].hour) {
            pos = i;
            break;
          } // end comparing hours
        } // end comparing days
      } // end comparing months

      pos = listSize;
    }
  }

  return pos;
}

/**
 * \brief Save meeting list to a text file.
 *
 * \param userInput String of user input command arguments (text file name)
 * \param meetingList The list of scheduled meetings.
 * \param listSize The number of scheduled meetings.
 *
 * \returns 1 if all scheduled meetings are saved to given text file.
 * \returns 0 if an error occurs.
 */
int writeToFile(char *userInput, Meeting *meetingList, int listSize) {

  // copy userInput string to a temporary string to avoid modification when
  // using strtok in numOfArgs function
  char *tempStr = calloc(strlen(userInput) + 1, sizeof(char));
  strcpy(tempStr, userInput);

  // check the number of given arguments
  if (numOfArgs(tempStr) != 1) {
    printf("W should be followed by exactly 1 argument.\n");
    return 0;
  }

  // get text file name from given command argument
  char *filename = calloc(strlen(userInput), sizeof(char));
  sscanf(userInput, "%s", filename);

  FILE *f = fopen(filename, "w");

  if (!f) {
    printf("Cannot open file %s for writing.\n", filename);
    free(filename);
    return 0;
  }

  for (int i = 0; i < listSize; i++) {
    fprintf(f, "%s %02d.%02d at %02d\n", meetingList[i].desc,
            meetingList[i].day, meetingList[i].month, meetingList[i].hour);

    if (ferror(f)) {
      fclose(f);
      free(filename);
      return 0;
    }
  }

  fclose(f);
  free(filename);

  return 1;
}

/**
 * \brief Clean up memory for a meeting list.
 *
 * \param list A list of scheduled meetings.
 * \param listSize The current number of scheduled meetings.
 */
void freeList(Meeting *list, int listSize) {
  for (int i = 0; i < listSize; i++) {
    free(list[i].desc);
  }
  free(list);
}

/**
 * \brief Load meetings from a text file.
 *
 * \param userInput String of user input command arguments (text file name)
 * \param meetingList Pointer to the current list of scheduled meetings.
 * \param listSize The current number of scheduled meetings.
 *
 * \returns 1 if meetings from text file are loaded and saved to meeting list.
 * \returns 0 if an error occurs.
 */
int loadFromFile(char *userInput, Meeting **meetingList, int *listSize) {

  // copy userInput string to a temporary string to avoid modification when
  // using strtok in numOfArgs function
  char *tempStr = calloc(strlen(userInput) + 1, sizeof(char));
  strcpy(tempStr, userInput);

  if (numOfArgs(tempStr) != 1) {
    printf("O should be followed by exactly 1 argument.\n");
    return 0;
  }

  char *filename = calloc(strlen(userInput), sizeof(char));
  sscanf(userInput, "%s", filename);

  FILE *f = fopen(filename, "r");

  if (!f) {
    printf("Cannot open file %s for reading.\n", filename);
    free(filename);
    return 0;
  }

  int entryCount = 0;
  char output[1000];

  // get the number of meetings stored in the text file
  while (fgets(output, 1000, f)) {
    entryCount += 1;
  }

  // go back to the beginning of the file
  rewind(f);

  Meeting *newList = calloc(entryCount, sizeof(Meeting));

  if (!newList) {
    printf("New list memory allocation failed.\n");
    fclose(f);
    free(filename);
    return 0;
  }

  int i = 0;
  int error = 0;

  while (fgets(output, 1000, f) && (error == 0)) {
    // get length of meeting description
    int descSize;
    char *charPos = strchr(output, ' ');
    descSize = charPos - output + 1;

    newList[i].desc = calloc(descSize, sizeof(char));

    if (!newList[i].desc) {
      printf("Cannot allocate memory for meeting description.\n");
      error = 1;
      break;
    }

    // load meeting data from text file to meeting list
    sscanf(output, "%s %d.%d at %d\n", newList[i].desc, &newList[i].day,
           &newList[i].month, &newList[i].hour);

    if (ferror(f)) {
      printf("Error occurs while reading data.\n");
      error = 1;
      break;
    }

    i++;
  }

  if (error == 1) {
    freeList(newList, i);
    fclose(f);
    free(filename);
    return 0;
  }

  // remove all existing meetings from current meeting list
  freeList(*meetingList, *listSize);

  // update meeting list and number of meetings
  *meetingList = newList;
  *listSize = entryCount;

  fclose(f);
  free(filename);
  return 1;
}

/**
 * \brief Perform action based on user command.
 *
 * \param userInput String of user input command.
 * \param meetingList Pointer to the current meeting list array.
 * \param listSize Pointer to the current number of meetings.
 *
 * \returns 1 if action is performed successfully.
 * \returns -1 if an error occurs during the action.
 * \returns 0 if user stops the program with 'Q'
 *
 */
int performAction(char *userInput, Meeting **meetingList, int *listSize) {

  char actionCommand;
  char argList[1000];

  // get command character
  actionCommand = userInput[0];

  if (strlen(userInput) > 1) {
    // get only command arguments
    strcpy(argList, &userInput[2]);
  }

  switch (toupper(actionCommand)) {
  case ADD:
    if (!addMeeting(meetingList, listSize, argList)) {
      return -1;
    }
    break;

  case DELETE:
    if (!deleteMeeting(meetingList, listSize, argList)) {
      return -1;
    }
    break;

  case LIST:
    displayMeetings(*meetingList, *listSize);
    break;

  case WRITE:
    if (!writeToFile(argList, *meetingList, *listSize)) {
      return -1;
    }
    break;

  case LOAD:
    if (!loadFromFile(argList, meetingList, listSize)) {
      return -1;
    }
    break;

  case QUIT:
    return 0;

  default:
    printf("Invalid command\n");
    return -1;
  }

  return 1;
}

int main(void) {

  Meeting *meetingList = NULL;
  int meetingListSize = 0;
  char userInput[1000];
  int continueAction = 1;

  while (continueAction) {
    fgets(userInput, sizeof(userInput), stdin);

    int actionSuccess =
        performAction(userInput, &meetingList, &meetingListSize);

    if (actionSuccess == 1) {
      printf("SUCCESS\n\n");
    } else if (actionSuccess == 0) {
      printf("SUCCESS\n\n");
      continueAction = 0;
    }
  } // end while loop

  freeList(meetingList, meetingListSize);
}
