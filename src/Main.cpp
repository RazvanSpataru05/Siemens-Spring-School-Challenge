#include <Services/Application.h>

int main()
{
	chrono::SetChronoDataPath(CHRONO_DATA_DIR);
	Application app{};
	app.Run();
}