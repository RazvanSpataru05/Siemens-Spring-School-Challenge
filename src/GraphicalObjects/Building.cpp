#include <GraphicalObjects/Building.h>

Building::Building(uint16_t numberOfCubesOx, uint16_t numberOfCubesOy, uint16_t numberOfCubesOz,
	double cubeSize) :
	m_numberOfCubesOx{ numberOfCubesOx },
	m_numberOfCubesOy{ numberOfCubesOy },
	m_numberOfCubesOz{ numberOfCubesOz },
	m_cubeSize{ cubeSize }
{
	m_mesh = std::make_shared<chrono::fea::ChMesh>();
	m_cubesExistence = std::vector<bool>(m_numberOfCubesOx * m_numberOfCubesOy * m_numberOfCubesOz, true);
	m_base = BodyReference();
	m_system = std::make_shared<chrono::ChSystemSMC>();
	m_material = ObjectProperties::SetMaterial();
}

Building::Building(const Building& another)
{
	*this = another;
}

Building::Building(Building&& another) noexcept
{
	*this = std::move(another);
}

Building& Building::operator=(const Building& another)
{
	if (this != &another)
	{
		m_base = another.m_base;
		m_system = another.m_system;
		m_mesh = another.m_mesh;
		m_meshNodeKeys = another.m_meshNodeKeys;
		m_material = another.m_material;
		m_cubesExistence = another.m_cubesExistence;
		m_cubeSize = another.m_cubeSize;
		m_numberOfCubesOx = another.m_numberOfCubesOx;
		m_numberOfCubesOy = another.m_numberOfCubesOy;
		m_numberOfCubesOz = another.m_numberOfCubesOz;
	}
	return *this;
}

Building& Building::operator=(Building&& another) noexcept
{
	if (this != &another)
	{
		int resetValue = 0;
		m_base = std::exchange(another.m_base, BodyReference());
		m_system = std::exchange(another.m_system, nullptr);
		m_mesh = std::exchange(another.m_mesh, nullptr);
		m_meshNodeKeys = std::exchange(another.m_meshNodeKeys, {});
		m_material = std::exchange(another.m_material, nullptr);
		m_cubesExistence = std::exchange(another.m_cubesExistence, std::vector<bool>());
		m_cubeSize = std::exchange(another.m_cubeSize, resetValue);
		m_numberOfCubesOx = std::exchange(another.m_numberOfCubesOx, resetValue);
		m_numberOfCubesOy = std::exchange(another.m_numberOfCubesOy, resetValue);
		m_numberOfCubesOz = std::exchange(another.m_numberOfCubesOz, resetValue);
	}
	return *this;
}

const std::shared_ptr<chrono::fea::ChMesh>& Building::GetMesh() const
{
	return m_mesh;
}

const std::vector<bool>& Building::GetCubesExistence() const
{
	return m_cubesExistence;
}

const std::shared_ptr<chrono::ChSystemSMC>& Building::GetSystem() const
{
	return m_system;
}

void Building::Build()
{
	std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> nodesLeftSide;
	std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> nodesRightSide;

	for (uint16_t layerOy = 0; layerOy < m_numberOfCubesOy; ++layerOy)
	{
		for (uint16_t layerOz = 0; layerOz < m_numberOfCubesOz; ++layerOz)
		{
			for (uint16_t layerOx = 0; layerOx <= m_numberOfCubesOx; ++layerOx)
			{
				nodesRightSide = BuildLateralNodesForLayers(layerOx, layerOy, layerOz);
				AddNodesInMesh(nodesRightSide);

				if (layerOx > 0)
				{
					BuildCube(nodesLeftSide, nodesRightSide);
				}

				nodesLeftSide = nodesRightSide;
			}
		}
	}
	m_system->Add(m_mesh);
}

void Building::AddNodesInMesh(const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>>& nodes)
{
	for (const auto& node : nodes)
	{
		if (m_meshNodeKeys.insert(NodeKey(node->GetPos())).second)
		{
			m_mesh->AddNode(node);
		}
	}
}

void Building::BuildCube(const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>>& nodesLeftSide,
	const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>>& nodesRightSide)
{
	auto element = std::make_shared<chrono::fea::ChElementHexaCorot_8>();

	element->SetNodes(nodesLeftSide[0], nodesLeftSide[1], nodesLeftSide[2], nodesLeftSide[3],
		nodesRightSide[0], nodesRightSide[1], nodesRightSide[2], nodesRightSide[3]);
	element->SetMaterial(m_material);

	m_mesh->AddElement(element);
}

std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> Building::BuildLateralNodesForLayers(
	uint16_t layerOx, uint16_t layerOy, uint16_t layerOz)
{
	std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> lateralNodes;

	lateralNodes.emplace_back(std::make_shared<chrono::fea::ChNodeFEAxyz>(
		chrono::ChVector<>(layerOx * m_cubeSize, layerOy * m_cubeSize, layerOz * m_cubeSize)));
	lateralNodes.emplace_back(std::make_shared<chrono::fea::ChNodeFEAxyz>(
		chrono::ChVector<>(layerOx * m_cubeSize, layerOy * m_cubeSize, (layerOz + 1) * m_cubeSize)));
	lateralNodes.emplace_back(std::make_shared<chrono::fea::ChNodeFEAxyz>(
		chrono::ChVector<>(layerOx * m_cubeSize, (layerOy - 1) * m_cubeSize, (layerOz + 1) * m_cubeSize)));
	lateralNodes.emplace_back(std::make_shared<chrono::fea::ChNodeFEAxyz>(
		chrono::ChVector<>(layerOx * m_cubeSize, (layerOy - 1) * m_cubeSize, layerOz * m_cubeSize)));

	return lateralNodes;
}

void Building::AddConstraints()
{
	m_system->Add(m_base.GetBody());

	for (size_t inode = 0; inode < m_mesh->GetNnodes(); ++inode)
	{
		if (auto node = std::dynamic_pointer_cast<chrono::fea::ChNodeFEAxyz>(m_mesh->GetNode(inode)))
		{
			if (node->GetPos().y() == -m_cubeSize)
			{
				AddConstraintBetweenNodeAndBase(node, m_base.GetBody());
			}
		}
	}
}

void Building::AddConstraintBetweenNodeAndBase(const std::shared_ptr<chrono::fea::ChNodeFEAxyz>& node,
	const std::shared_ptr<chrono::ChBody>& base)
{
	auto constraint = std::make_shared<chrono::fea::ChLinkPointFrame>();
	constraint->Initialize(node, base);

	auto constraintCube = std::make_shared<chrono::ChBoxShape>();
	constraintCube->GetBoxGeometry().Size = chrono::ChVector<>(0.005);
	constraint->AddAsset(constraintCube);
	constraint->GetConstrainedNode();

	m_system->Add(constraint);
}

void Building::EliminateConstaints()
{
	std::vector<std::shared_ptr<chrono::ChLinkBase>> constraints = m_system->Get_linklist();

	for (const auto& constraint : constraints)
	{
		auto pointFrame = std::dynamic_pointer_cast<chrono::fea::ChLinkPointFrame>(constraint);
		if (!pointFrame) continue;

		auto constraintNode = pointFrame->GetConstrainedNode();

		if (!constraintNode || !NodeAlreadyExistsInMesh(constraintNode))
		{
			m_system->RemoveLink(constraint);
		}
	}
}

void Building::EliminateCubesBasedOnCubesExistence(const std::vector<bool>& importance)
{
	const std::vector<std::shared_ptr<chrono::fea::ChElementBase>> elements = m_mesh->GetElements();
	const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAbase>> nodes = m_mesh->GetNodes();

	std::unordered_map<int64_t, size_t> elementIndexByKey;
	std::unordered_map<int64_t, int> elementsPerNode;
	elementIndexByKey.reserve(elements.size());
	elementsPerNode.reserve(nodes.size());

	for (size_t elementIndex = 0; elementIndex < elements.size(); ++elementIndex)
	{
		const auto& element = elements[elementIndex];

		for (size_t indexNode = 0; indexNode < element->GetNnodes(); ++indexNode)
		{
			if (auto node = std::dynamic_pointer_cast<chrono::fea::ChNodeFEAxyz>(element->GetNodeN(indexNode)))
			{
				const int64_t key = NodeKey(node->GetPos());
				++elementsPerNode[key];

				if (indexNode == 0)
				{
					elementIndexByKey[key] = elementIndex;
				}
			}
		}
	}

	std::unordered_map<int64_t, size_t> nodeIndexByKey;
	nodeIndexByKey.reserve(nodes.size());

	for (size_t indexNode = 0; indexNode < nodes.size(); ++indexNode)
	{
		if (auto node = std::dynamic_pointer_cast<chrono::fea::ChNodeFEAxyz>(nodes[indexNode]))
		{
			nodeIndexByKey[NodeKey(node->GetPos())] = indexNode;
		}
	}

	std::vector<bool> elementRemoved(elements.size(), false);
	std::vector<bool> nodeRemoved(nodes.size(), false);

	for (size_t index = 0; index < importance.size(); ++index)
	{
		if (importance[index] || !m_cubesExistence[index]) continue;

		m_cubesExistence[index] = false;

		const auto elementEntry = elementIndexByKey.find(CellKey(index));
		if (elementEntry == elementIndexByKey.end()) continue;

		const size_t elementPosition = elementEntry->second;
		if (elementRemoved[elementPosition]) continue;

		elementRemoved[elementPosition] = true;

		const auto& element = elements[elementPosition];

		for (size_t indexNode = 0; indexNode < element->GetNnodes(); ++indexNode)
		{
			if (auto node = std::dynamic_pointer_cast<chrono::fea::ChNodeFEAxyz>(element->GetNodeN(indexNode)))
			{
				const int64_t key = NodeKey(node->GetPos());

				if (--elementsPerNode[key] > 0) continue;

				const auto nodeEntry = nodeIndexByKey.find(key);
				if (nodeEntry != nodeIndexByKey.end())
				{
					nodeRemoved[nodeEntry->second] = true;
				}
			}
		}
	}

	m_mesh->ClearNodes();
	m_meshNodeKeys.clear();

	for (size_t index = 0; index < nodes.size(); ++index)
	{
		if (nodeRemoved[index]) continue;

		m_mesh->AddNode(nodes[index]);

		if (auto node = std::dynamic_pointer_cast<chrono::fea::ChNodeFEAxyz>(nodes[index]))
		{
			m_meshNodeKeys.insert(NodeKey(node->GetPos()));
		}
	}

	m_mesh->ClearElements();

	for (size_t index = 0; index < elements.size(); ++index)
	{
		if (!elementRemoved[index])
		{
			m_mesh->AddElement(elements[index]);
		}
	}

	EliminateConstaints();
}

void Building::AddCubesBasedOnCubesExistence(const std::vector<bool>& importance)
{
	std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> nodesLeftSide;
	std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> nodesRightSide;

	for (size_t index = 0; index < importance.size(); ++index)
	{
		if (importance[index] && !m_cubesExistence[index])
		{
			if (HasAddedElementNeighbors(index, importance))
			{
				m_cubesExistence[index] = true;

				uint16_t layerOx = index % m_numberOfCubesOx;
				uint16_t layerOy = index / (m_cubesExistence.size() / m_numberOfCubesOy);
				uint16_t layerOz = (index / m_numberOfCubesOx) % m_numberOfCubesOz;

				nodesLeftSide = BuildLateralNodesForLayers(layerOx, layerOy, layerOz);
				nodesRightSide = BuildLateralNodesForLayers(layerOx + 1, layerOy, layerOz);
				AddNodesInMesh(nodesLeftSide);
				AddNodesInMesh(nodesRightSide);

				BuildCube(nodesLeftSide, nodesRightSide);

				if (layerOy == 0)
				{
					AddConstraintBetweenNodeAndBase(nodesLeftSide[2], m_base.GetBody());
					AddConstraintBetweenNodeAndBase(nodesLeftSide[3], m_base.GetBody());
					AddConstraintBetweenNodeAndBase(nodesRightSide[2], m_base.GetBody());
					AddConstraintBetweenNodeAndBase(nodesRightSide[3], m_base.GetBody());
				}
			}
		}
	}
}

bool Building::NodeAlreadyExistsInMesh(const std::shared_ptr<chrono::fea::ChNodeFEAxyz>& node)
{
	return m_meshNodeKeys.find(NodeKey(node->GetPos())) != m_meshNodeKeys.end();
}
bool Building::HasAddedElementNeighbors(int position, const std::vector<bool>& importance)
{
	if (position + 1 < importance.size())
		if (importance[position + 1])
			return true;

	if (position - 1 >= 0)
		if (importance[position - 1])
			return true;

	if (position + m_numberOfCubesOx < importance.size())
		if (importance[position + m_numberOfCubesOx])
			return true;

	if (position - m_numberOfCubesOx >= 0)
		if (importance[position - m_numberOfCubesOx])
			return true;

	if (position + (importance.size() / m_numberOfCubesOy) < importance.size())
		if (importance[position + (importance.size() / m_numberOfCubesOy)])
			return true;

	if (position - (static_cast<int>(importance.size()) / static_cast<int>(m_numberOfCubesOy)) >= 0)
		if (importance[position - (static_cast<int>(importance.size()) / static_cast<int>(m_numberOfCubesOy))])
			return true;

	return false;
}

int64_t Building::NodeKey(const chrono::ChVector<>& position) const
{
	static constexpr int64_t K_KEY_OFFSET = 1 << 20;
	static constexpr int K_KEY_BITS = 21;

	const int64_t x = std::llround(position.x() / m_cubeSize) + K_KEY_OFFSET;
	const int64_t y = std::llround(position.y() / m_cubeSize) + K_KEY_OFFSET;
	const int64_t z = std::llround(position.z() / m_cubeSize) + K_KEY_OFFSET;

	return (x << (2 * K_KEY_BITS)) | (y << K_KEY_BITS) | z;
}

int64_t Building::CellKey(size_t cellIndex) const
{
	const uint16_t layerOx = static_cast<uint16_t>(cellIndex % m_numberOfCubesOx);
	const uint16_t layerOy = static_cast<uint16_t>(cellIndex / (m_cubesExistence.size() / m_numberOfCubesOy));
	const uint16_t layerOz = static_cast<uint16_t>((cellIndex / m_numberOfCubesOx) % m_numberOfCubesOz);

	return NodeKey(chrono::ChVector<>(layerOx * m_cubeSize, layerOy * m_cubeSize, layerOz * m_cubeSize));
}
