#include <Services/IOIndividualManager.h>

void IOIndividualManager::ReadIndividualsDetailsAndCreateBuildings()
{
	std::ifstream file;

	std::shared_ptr<Building> readBuilding = nullptr;

	file.open(FILE_NAME_INDIVIDUAL, std::ios::in);
	if (file.is_open())
	{
		int sizeOx, sizeOy, sizeOz;
		double elementSize;
		bool cubeExistence;
		std::vector<bool> cubesExistence;

		while (!file.eof())
		{
			file >> sizeOx >> sizeOy >> sizeOz >> elementSize;

			for (int index = 0; index < sizeOx * sizeOy * sizeOz; ++index)
			{
				file >> cubeExistence;
				cubesExistence.emplace_back(cubeExistence);
			}

			readBuilding = Individual::CreateBuildingFromDetails(sizeOx, sizeOy, sizeOz, elementSize, cubesExistence);
			Scene scene;
			scene.Show(readBuilding);

			cubesExistence.clear();
		}
		file.close();
	}
	else
	{
		std::cerr << "Could not open " + FILE_NAME_INDIVIDUAL + " file!";
	}
}

std::vector<bool> IOIndividualManager::ReadInitialIndividual(int individualSize)
{
	std::ifstream file;

	file.open(FILE_NAME_INITIAL_INDIVIDUAL, std::ios::in);

	bool cubeExistence;
	std::vector<bool> cubesExistence;

	if (file.is_open())
	{
		for (int index = 0; index < individualSize; ++index)
		{
			file >> cubeExistence;
			cubesExistence.emplace_back(cubeExistence);
		}

		file.close();
	}
	else
	{
		std::cerr << "Could not open " + FILE_NAME_INITIAL_INDIVIDUAL + " file!";
	}

	return cubesExistence;
}

void IOIndividualManager::WriteIndividualDetailsInFile(IIndividual* individual)
{
	std::ofstream file;

	file.open(FILE_NAME_INDIVIDUAL);

	if (file.is_open())
	{
		Individual* castedIndividual = dynamic_cast<Individual*>(individual);
		file << *castedIndividual;
		file.close();
	}
	else
	{
		std::cerr << "Could not open " + FILE_NAME_INDIVIDUAL + " file!";
	}
}

void IOIndividualManager::WriteIndividualValueInFile(int epoch, double fitnessValue, bool append)
{
	std::ofstream file;

	if (append)
		file.open(FILE_NAME_INDIVIDUAL_VALUES, std::ios_base::app);
	else
		file.open(FILE_NAME_INDIVIDUAL_VALUES);

	if (file.is_open())
	{
		if (!append)
		{
			file << "EPOCH" << "," << "VALUE" << std::endl;
		}

		file << epoch << "," << fitnessValue << std::endl;
		file.close();
	}
	else
	{
		std::cerr << "Could not open " + FILE_NAME_INDIVIDUAL_VALUES + " file!";
	}
}
