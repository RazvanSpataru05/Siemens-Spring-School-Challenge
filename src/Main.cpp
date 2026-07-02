#include <Services/GAApplication.h>

int main(int argc, char* argv[])
{
	chrono::SetChronoDataPath(CHRONO_DATA_DIR);
	GAApplication app{};
	app.Run();

	return 0;
}