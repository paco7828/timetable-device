#include <Wire.h>
#include <RTClib.h>

int currentDay;
int currentTimeInMins;
// Change if needed
const int dailyMaxLessons = 8;

// Initialize RTC
RTC_DS3231 rtc;

struct Lesson {
  String startTime;
  String endTime;
  String subject;
};

struct DaySchedule {
  Lesson lessons[dailyMaxLessons];
};

// Timetable
DaySchedule timetable[] = {
  // Monday
  { { { "13:15", "14:00", "Angol" },
      { "14:05", "14:45", "Angol" },
      { "15:35", "16:15", "Adatbázis" },
      { "16:20", "17:00", "Asztali" },
      { "17:05", "17:45", "Asztali" },
      { "17:50", "18:30", "Frontend" },
      { "18:35", "19:15", "Frontend" } } },
  // Tuesday
  { { { "10:25", "11:10", "Ofő" },
      { "13:15", "14:00", "Fizika" },
      { "14:05", "14:45", "Fizika" },
      { "14:50", "15:30", "Fizika" } } },
  // Wednesday
  { { { "13:15", "14:00", "Angol" },
      { "14:05", "14:45", "Angol" },
      { "14:50", "15:30", "Digitális kultúra" },
      { "16:20", "17:00", "Asztali" },
      { "17:05", "17:45", "Asztali" },
      { "17:50", "18:30", "Backend" },
      { "18:35", "19:15", "Backend" } } },
  // Thursday
  { { { "14:05", "14:45", "Angol" },
      { "14:50", "15:30", "Digitális kultúra" },
      { "15:35", "16:15", "Frontend" },
      { "16:20", "17:00", "Frontend" },
      { "17:05", "17:45", "Backend" },
      { "17:50", "18:30", "Backend" },
      { "18:35", "19:15", "Asztali" } } },
  // Friday
  { { { "12:20", "13:05", "Digitális kultúra" },
      { "13:15", "14:00", "Adatbázis" },
      { "14:05", "14:45", "Adatbázis" } } }
};

// Function to convert a given time into minutes
int timeToMinutes(String time) {
  int colonIndex = time.indexOf(":");
  int hour = time.substring(0, colonIndex).toInt();
  int minute = time.substring(colonIndex + 1).toInt();
  return hour * 60 + minute;
}

// Code in setup due to device only turning on when button is pressed
void setup() {
  Serial.begin(9600);

  // Set up RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC!");
    while (1)
      ;
  }

  // Check if the RTC lost power
  if (rtc.lostPower()) {
    Serial.println("RTC lost power!");
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  // Optionally, you can set the time here if needed
  rtc.adjust(DateTime(2024, 11, 5, 14, 0, 0));

  // Get current datetime
  DateTime now = rtc.now();

  // Get current day
  currentDay = now.dayOfTheWeek();

  currentTimeInMins = now.hour() * 60 + now.minute();
  Serial.print(now.year(), DEC);
  Serial.print('-');
  Serial.print(now.month(), DEC);
  Serial.print('-');
  Serial.print(now.day(), DEC);

  if (currentDay == 0 || currentDay == 6) {
    // Saturday or Sunday
    Serial.println("No school today");
  } else {
    // Display the current or next lesson
    displayCurrentOrNextLesson();
  }
}

// Function to display the upcoming / current lessons
void displayCurrentOrNextLesson() {
  DaySchedule todaySchedule = timetable[currentDay - 1];

  for (int i = 0; i < 8; i++) {
    Lesson lesson = todaySchedule.lessons[i];

    // If the lesson is empty skip
    if (lesson.startTime == "" && lesson.endTime == "" && lesson.subject == "") {
      continue;
    }

    int lessonStart = timeToMinutes(lesson.startTime);
    int lessonEnd = timeToMinutes(lesson.endTime);

    Serial.println("Lesson " + String(i + 1) + ": " + lesson.subject);

    if (currentTimeInMins < lessonStart) {
      // If it's before the start time of the first lesson
      Serial.println("Subject: " + lesson.subject);
      Serial.println("Starts at: " + lesson.startTime);
      Serial.println("Ends at: " + lesson.endTime);
      return;
    } else if (currentTimeInMins >= lessonStart && currentTimeInMins < lessonEnd) {
      // Display the current lesson
      Serial.println("Current lesson:");
      Serial.println("Subject: " + lesson.subject);
      Serial.println("Ends at: " + lesson.endTime);
      return;
    }
  }

  Serial.println("No more lessons");
}

void loop() {
}