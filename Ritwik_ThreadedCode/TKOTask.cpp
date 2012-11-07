# include <Task.h>

void auton ();
void teleop();
void shooter();

int main ()
{
    int num_tasks = 3;
    int test[3] = {1,2,3};
   Task TKOBot[num_tasks] = {autonomous("Autonomous", auton) , teleoperated ("Teleoperated", teleop) , shooter_control ("Shooter", shooter)};
    for ( int i = 0 ; i < num_tasks; i++ )
    {
        TKOBot[i].Start();
    }

}


void teleop ()
{
    // Teleoperation code goes here
}

void auton ()
{
	//Autonomous Code goes here
}



void shooter ()
{
    // Shooter code goes here
}

