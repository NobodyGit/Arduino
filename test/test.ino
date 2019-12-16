#include <task.h>

// define tasks
void Task1( void *pvParameters );
void Task2( void *pvParameters );
void Task3( void *pvParameters );

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);

  // Now set up two tasks to run independently.
  xTaskCreate(Task1, "Task1", 10000, NULL, 1, NULL);
  xTaskCreate(Task2, "Task2", 10000, NULL, 1, NULL);
  xTaskCreate(Task3, "Task3", 10000, NULL, 1, NULL);

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*---------------------- Tasks ---------------------*/

void Task1(void *pvParameters)  // This is a task1.
{
  while(1)
  {
    Serial.println("Task1 executing...");
    vTaskDelay( 900 / portTICK_PERIOD_MS ); // wait for 900ms
  }
}

void Task2(void *pvParameters)  // This is task2.
{
  while(1)
  {
    Serial.println("Task2 executing...");
    vTaskDelay( 300 / portTICK_PERIOD_MS ); // wait for 300ms
  }
}

void Task3(void *pvParameters)  // This is task3.
{
  while(1)
  {
    Serial.println("Task3 executing...");
    vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for 100ms
  }
}
