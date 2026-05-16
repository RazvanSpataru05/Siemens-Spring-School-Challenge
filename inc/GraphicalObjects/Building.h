#pragma once

#include <memory>

#include <fea/ChMesh.h>

#include <chrono/fea/ChNodeFEAxyz.h>
#include <chrono/fea/ChElementHexaCorot_8.h>
#include <chrono/fea/ChLinkPointFrame.h>

#include <chrono/physics/ChSystemSMC.h>
#include <chrono/physics/ChBody.h>

#include <chrono/assets/ChBoxShape.h>

#include <GraphicalObjects/ObjectProperties.h>
#include <GraphicalObjects/BodyReference.h>

class Building
{
public:
	Building(uint16_t numberOfCubesOx, uint16_t numberOfCubesOy, uint16_t numberOfCubesOz, double cubeSize);
	
	Building(const Building& another);
	Building(Building&& another) noexcept;

	Building& operator=(const Building& another);
	Building& operator=(Building&& another) noexcept;
	
	~Building() = default;

	const std::shared_ptr<chrono::fea::ChMesh> GetMesh() const;
	const std::vector<bool> GetCubesExistence() const;
	const std::shared_ptr<chrono::ChSystemSMC> GetSystem() const;

	void Build();

	void AddConstraints();

	void EliminateCubesBasedOnCubesExistence(const std::vector<bool>& importance);
	void AddCubesBasedOnCubesExistence(const std::vector<bool>& importance);

private:
	std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> BuildLateralNodesForLayers(
		uint16_t layerOx, uint16_t layerOy, uint16_t layerOz);
	void BuildCube(const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>>& nodesLeftSide,
		const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>>& nodesRightSide);

	void AddNodesInMesh(const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>>& nodes);
	void AddConstraintBetweenNodeAndBase(const std::shared_ptr<chrono::fea::ChNodeFEAxyz>& node,
		const std::shared_ptr<chrono::ChBody>& base);
	void EliminateConstaints();

	int GetNodePositionInVector(const std::shared_ptr<chrono::fea::ChNodeFEAxyz>& node,
		const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAbase>>& nodes);
	int GetElementPositionFromImportanceVector(int position);

	bool NodeAlreadyExistsInMesh(const std::shared_ptr<chrono::fea::ChNodeFEAxyz>& node);
	bool ExistsAnotherElementWithNode(const std::shared_ptr<chrono::fea::ChNodeFEAxyz>& node,
		const std::vector<std::shared_ptr<chrono::fea::ChElementBase>>& elements);

	bool HasAddedElementNeighbors(int position, const std::vector<bool>& importance);

	bool AreNodesEqual(const std::shared_ptr<chrono::fea::ChNodeFEAxyz>& lhsNode, 
		const std::shared_ptr<chrono::fea::ChNodeFEAxyz>& rhsNode);

private:
	std::shared_ptr<chrono::fea::ChMesh> m_mesh;

	uint16_t m_numberOfCubesOx;
	uint16_t m_numberOfCubesOy;
	uint16_t m_numberOfCubesOz;

	double m_cubeSize;
	std::vector<bool> m_cubesExistence;

	BodyReference m_base;

	std::shared_ptr<chrono::ChSystemSMC> m_system;
};
