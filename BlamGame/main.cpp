#include "GameObject.h"
#include <cstdlib>
#include <ctime>
int main()
{
	srand(time(NULL));	
	GameObject app;
	app.Run();
	
	return 0;
}