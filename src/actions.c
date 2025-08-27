#include "../include/project.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief Search for a meeting in the meeting list that have the same time
 * values as given.
 *
 * \param meetingList The list of scheduled meetings.
 * \param listSize The number of meetings already in the schedule.
 * \param targetMonth, targetDay, targetHour The meeting time criteria to search
 * for meeting.
 *
 * \returns The position of the meeting if found. The position can be from 0 to
 * listSize-1.
 * \returns -1 if no meeting is found.
 */
int findMeeting(Meeting *meetingList, int listSize, int targetMonth,
                int targetDay, int targetHour) {

  for (int i = 0; i < listSize; i++) {
    if (targetMonth == meetingList[i].month &&
        targetDay == meetingList[i].day && targetHour == meetingList[i].hour) {
      return i;
    }
  }
  return -1;
}

/**
 * \brief Initializes a meeting with data from user entered command arguments.
 *
 * \param newMeeting The pointer to store the new meeting.
 * \param inputDesc Entered meeting description string.
 * \param inputMonth, inputDay, inputHour Entered meeting time values.
 *
 * \returns 0 if fail to allocate memory for meeting description.
 * \returns 1 if all data is assigned to the new meeting.
 */
int init_meeting(Meeting *newMeeting, char *inputDesc, int inputMonth,
                 int inputDay, int inputHour) {

  newMeeting->desc = (char *)calloc(strlen(inputDesc) + 1, sizeof(char));

  if ((!newMeeting->desc)) {
    printf("Cannot allocate memory for new meeting description.\n");
    return 0;
  }

  strcpy(newMeeting->desc, inputDesc);

  newMeeting->month = inputMonth;
  newMeeting->day = inputDay;
  newMeeting->hour = inputHour;

  return 1;
}

/**
 * \brief Add meeting to schedule list.
 *
 * \param meetingList Pointer to the current meeting list array.
 * \param listSize Pointer to the current number of meetings.
 * \param userInput String of user input command arguments.
 *
 * \returns 1 if a new meeting is added to the meeting list successfully.
 * \returns 0 if an error occurs during the process.
 */
int addMeeting(Meeting **meetingList, int *listSize, char *userInput) {

  // copy userInput string to a temporary string to avoid modification when
  // using strtok in numOfArgs function
  char *tempStr = calloc(strlen(userInput) + 1, sizeof(char));
  strcpy(tempStr, userInput);

  // check that the correct number of command arguments is given
  if (numOfArgs(tempStr) != 4) {
    printf("A should be followed by exactly 4 arguments.\n");
    return 0;
  }

  // parse new meeting data from command arguments
  char *subStr = strtok(userInput, " ");
  char *inputDesc = calloc(strlen(subStr) + 1, sizeof(char));
  strcpy(inputDesc, subStr);

  subStr = strtok(NULL, " ");
  char *tempMonth = calloc(strlen(subStr) + 1, sizeof(char));
  strcpy(tempMonth, subStr);

  subStr = strtok(NULL, " ");
  char *tempDay = calloc(strlen(subStr) + 1, sizeof(char));
  strcpy(tempDay, subStr);

  subStr = strtok(NULL, " ");
  char *tempHour = calloc(strlen(subStr) + 1, sizeof(char));
  strcpy(tempHour, subStr);

  // verify that time values are valid
  if (!validateArguments(tempMonth, tempDay, tempHour)) {
    free(inputDesc);
    free(tempMonth);
    free(tempDay);
    free(tempHour);
    return 0;
  }

  // if command arguments are valid, convert date-time values to integers and
  // free temp strings
  int inputMonth = atoi(tempMonth);
  int inputDay = atoi(tempDay);
  int inputHour = atoi(tempHour);

  free(tempMonth);
  free(tempDay);
  free(tempHour);

  // verify that there is no meeting scheduled at the same time slot
  if (findMeeting(*meetingList, *listSize, inputMonth, inputDay, inputHour) !=
      -1) {
    printf("There is another meeting at this time.\n");
    free(inputDesc);
    return 0;
  }

  int insertPos =
      findInsertPos(*meetingList, *listSize, inputMonth, inputDay, inputHour);

  // reallocate memory to increase the size of current meeting list by 1
  Meeting *newList = realloc(*meetingList, (*listSize + 1) * sizeof(Meeting));

  if (!newList) {
    printf("Cannot reallocate memory for meeting list.\n");
    free(inputDesc);
    return 0;
  }

  Meeting tempMeeting;

  if (init_meeting(&tempMeeting, inputDesc, inputMonth, inputDay, inputHour)) {
    // if the insert position for the new meeting is in the middle of the list,
    // shift the positions of later meetings by 1 position to the right
    for (int i = *listSize; i > insertPos; i--) {
      newList[i] = newList[i - 1];
    }

    // insert the new meeting
    newList[insertPos] = tempMeeting;

    // update the list pointer
    *meetingList = newList;

    *listSize += 1;

    free(inputDesc);

    return 1;
  } else {
    free(newList);
    free(inputDesc);
    return 0;
  }
}

/**
 * \brief Delete a meeting.
 *
 * \param meetingList Pointer to the current meeting list array.
 * \param listSize Pointer to the current number of meetings.
 * \param userInput String of user input command arguments.
 *
 * \returns 1 if successfully delete an existing meeting
 * \returns 0 if an error occurs.
 */
int deleteMeeting(Meeting **meetingList, int *listSize, char *userInput) {

  // copy userInput string to a temporary string to avoid modification when
  // using strtok in numOfArgs function
  char *tempStr = calloc(strlen(userInput) + 1, sizeof(char));
  strcpy(tempStr, userInput);

  // check that the correct number of command arguments is given
  if (numOfArgs(tempStr) != 3) {
    printf("D should be followed by exactly 3 arguments.\n");
    return 0;
  }

  // parse data from command arguments
  char *subStr = strtok(userInput, " ");
  char *tempMonth = calloc(strlen(subStr) + 1, sizeof(char));
  strcpy(tempMonth, subStr);

  subStr = strtok(NULL, " ");
  char *tempDay = calloc(strlen(subStr) + 1, sizeof(char));
  strcpy(tempDay, subStr);

  subStr = strtok(NULL, " ");
  char *tempHour = calloc(strlen(subStr) + 1, sizeof(char));
  strcpy(tempHour, subStr);

  // verify that time values are valid
  if (!validateArguments(tempMonth, tempDay, tempHour)) {
    free(tempMonth);
    free(tempDay);
    free(tempHour);
    return 0;
  }

  // if command arguments are valid, convert date-time values to integers and
  // free temp strings
  int inputMonth = atoi(tempMonth);
  int inputDay = atoi(tempDay);
  int inputHour = atoi(tempHour);

  free(tempMonth);
  free(tempDay);
  free(tempHour);

  // find the existing meeting that matches given time data
  int meetingPos =
      findMeeting(*meetingList, *listSize, inputMonth, inputDay, inputHour);

  // there is nothing to delete if no meeting is found
  if (meetingPos == -1) {
    printf("The time slot %02d.%02d at %02d is not in the calendar.\n",
           inputDay, inputMonth, inputHour);
    return 0;
  }

  // free memory allocated for deleting meeting description string
  free(meetingList[0][meetingPos].desc);

  // if a meeting is found, shift the scheduled meetings from the found position
  // by 1 to the left and decrease the number of meetings by 1
  for (int i = meetingPos; i < *listSize - 1; i++) {
    meetingList[0][i] = meetingList[0][i + 1];
  }

  *listSize -= 1;

  return 1;
}

/**
 * \brief Print out all scheduled meetings.
 *
 * \param meetingList The list of scheduled meetings.
 * \param listSize The number of scheduled meetings.
 */
void displayMeetings(Meeting *meetingList, int listSize) {

  for (int i = 0; i < listSize; i++) {
    printf("%s %02d.%02d at %02d\n", meetingList[i].desc, meetingList[i].day,
           meetingList[i].month, meetingList[i].hour);
  }
}
