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

class Individual : public IIndividual
{
public:
	Individual(int sizeOx, int sizeOy, int sizeOz, double elementSize);
	Individual(int sizeOx, int sizeOy, int sizeOz, double elementSize, const std::vector<bool>& cubesExistence);

	Individual(const Individual& another);
	Individual(Individual&& another) noexcept;

	Individual& operator=(const Individual& another);
	Individual& operator=(Individual&& another) noexcept;

	~Individual() = default;

	void SetMaximStress(double maximStress);

	const std::shared_ptr<Building> GetBuilding() const;

	double Evaluate() override;

	void Crossover(IIndividual& other) override;
	void Mutation(double mutationProbability) override;

	bool operator==(const Individual& other) const;

	friend std::ostream& operator<<(std::ostream& out, const Individual& individual);

	static std::shared_ptr<Building> CreateBuildingFromDetails(int sizeOx, int sizeOy, int sizeOz,
		double elementSize, const std::vector<bool>& cubesExistence);

private:
	int GetNumberOfRemovedElements();
	double SimulateAndGetMaximStress();
	bool IsOnTopLayer(size_t possition);

private:
	std::shared_ptr<Building> m_building;

	double m_maximStress;
	int m_sizeOx;
	int m_sizeOy;
	int m_sizeOz;
	double m_elementSize;

	std::vector<bool> m_initialGenes;
};