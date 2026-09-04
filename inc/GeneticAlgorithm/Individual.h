#pragma once

#include <vector>
#include <math.h>
#include <iostream>

#include <fea/ChMesh.h>

#include <GeneticAlgorithm/IIndividual.h>

#include <GraphicalObjects/Building.h>

#include <Services/constants.h>
#include <Services/AlgorithmSettings.h>
#include <Services/RandomNumbersGenerator.h>

#include <System/ConfigureSystem.h>

#include <FitnessFunctions/IFitnessFunction.h>

class Individual : public IIndividual
{
public:
	Individual(int sizeOx, int sizeOy, int sizeOz, double elementSize,
		double maximStress, std::unique_ptr<IFitnessFunction> fitnessFunction);

	Individual(int sizeOx, int sizeOy, int sizeOz, double elementSize,
		const std::vector<bool>& cubesExistence,
		double maximStress, std::unique_ptr<IFitnessFunction> fitnessFunction);

	Individual(const Individual& another);
	Individual(Individual&& another) noexcept;

	Individual& operator=(const Individual& another);
	Individual& operator=(Individual&& another) noexcept;

	~Individual() = default;

	const std::shared_ptr<Building>& GetBuilding() const;
	int GetNumberOfRemovedElements() const;
	int GetNumberOfRemovedElementsWithoutInitialGene() const;
	double GetCurrentMaxStress() const;

	int GetSizeOx() const;
	int GetSizeOy() const;
	int GetSizeOz() const;
	bool IsOnTopLayer(size_t possition) const;

	double Evaluate() override;

	void Mutation(double mutationProbability) override;

	friend std::ostream& operator<<(std::ostream& out, const Individual& individual);

	static std::shared_ptr<Building> CreateBuildingFromDetails(int sizeOx, int sizeOy, int sizeOz,
		double elementSize, const std::vector<bool>& cubesExistence);

private:
	double SimulateAndGetMaximStress();

private:
	std::shared_ptr<Building> m_building;

	double m_maximStress;
	double m_currentMaxStress = 0.0;
	int m_sizeOx;
	int m_sizeOy;
	int m_sizeOz;
	double m_elementSize;

	std::unique_ptr<IFitnessFunction> m_fitnessFunction;

	std::vector<bool> m_initialGenes;
};