#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>

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

	const std::shared_ptr<chrono::fea::ChMesh>& GetMesh() const;
	const std::vector<bool>& GetCubesExistence() const;
	const std::shared_ptr<chrono::ChSystemSMC>& GetSystem() const;

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

	bool NodeAlreadyExistsInMesh(const std::shared_ptr<chrono::fea::ChNodeFEAxyz>& node);

	bool HasAddedElementNeighbors(int position, const std::vector<bool>& importance);

	// Node coordinates are always integer multiples of m_cubeSize, so quantising by that
	// step turns a position into an exact lattice index. Packing the three indices into
	// one integer gives a hashable identity for a node, which replaces the linear scans
	// with exact float comparisons that used to dominate mesh edits.
	int64_t NodeKey(const chrono::ChVector<>& position) const;
	int64_t CellKey(size_t cellIndex) const;

private:
	std::shared_ptr<chrono::fea::ChMesh> m_mesh;

	// Mirrors the set of nodes currently in m_mesh, so NodeAlreadyExistsInMesh is O(1).
	std::unordered_set<int64_t> m_meshNodeKeys;

	// Every element shares the same properties; build it once instead of per element.
	std::shared_ptr<chrono::fea::ChContinuumElastic> m_material;

	uint16_t m_numberOfCubesOx;
	uint16_t m_numberOfCubesOy;
	uint16_t m_numberOfCubesOz;

	double m_cubeSize;
	std::vector<bool> m_cubesExistence;

	BodyReference m_base;

	std::shared_ptr<chrono::ChSystemSMC> m_system;
};
