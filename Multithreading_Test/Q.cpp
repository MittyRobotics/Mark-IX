# include <WPILib.h>

void auton ();
void teleop();
void shooter();

int main ()
{
       Task test_task("Task_Test", test_task_func);
       test_task.Start();
}

void test_task_func ()
{
	printf("This is a test of the Task class in WPILib.");
}

