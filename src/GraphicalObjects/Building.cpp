#include <GraphicalObjects/Building.h>

Building::Building(uint16_t numberOfCubesOx, uint16_t numberOfCubesOy, uint16_t numberOfCubesOz, double cubeSize) :
	m_numberOfCubesOx{ numberOfCubesOx },
	m_numberOfCubesOy{ numberOfCubesOy },
	m_numberOfCubesOz{ numberOfCubesOz },
	m_cubeSize{ cubeSize }
{
	m_mesh = std::make_shared<chrono::fea::ChMesh>();
	m_cubesExistence = std::vector<bool>(m_numberOfCubesOx * m_numberOfCubesOy * m_numberOfCubesOz, true);
	m_base = BodyReference();
	m_system = std::make_shared<chrono::ChSystemSMC>();
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
		m_cubesExistence = std::exchange(another.m_cubesExistence, std::vector<bool>());
		m_cubeSize = std::exchange(another.m_cubeSize, resetValue);
		m_numberOfCubesOx = std::exchange(another.m_numberOfCubesOx, resetValue);
		m_numberOfCubesOy = std::exchange(another.m_numberOfCubesOy, resetValue);
		m_numberOfCubesOz = std::exchange(another.m_numberOfCubesOz, resetValue);
	}
	return *this;
}

const std::shared_ptr<chrono::fea::ChMesh> Building::GetMesh() const
{
	return m_mesh;
}

const std::vector<bool> Building::GetCubesExistence() const
{
	return m_cubesExistence;
}

const std::shared_ptr<chrono::ChSystemSMC> Building::GetSystem() const
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

				nodesLeftSide.clear();
				std::copy(nodesRightSide.begin(), nodesRightSide.end(), std::back_inserter(nodesLeftSide));
			}
		}
	}

	m_system->Add(m_mesh);
}

void Building::AddNodesInMesh(const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>>& nodes)
{
	for (const auto& node : nodes)
	{
		if (!NodeAlreadyExistsInMesh(node))
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
	element->SetMaterial(ObjectProperties::SetMaterial());

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

	for (uint16_t inode = 0; inode < m_mesh->GetNnodes(); ++inode)
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

	std::vector<std::shared_ptr<chrono::fea::ChNodeFEAbase>> nodes = m_mesh->GetNodes();

	for (const auto& constraint : constraints)
	{
		auto constraintNode = std::dynamic_pointer_cast<chrono::fea::ChLinkPointFrame>(constraint)->GetConstrainedNode();

		if (!NodeAlreadyExistsInMesh(constraintNode))
		{
			m_system->RemoveLink(constraint);
		}
	}
}

void Building::EliminateCubesBasedOnCubesExistence(const std::vector<bool>& importance)
{
	std::vector<std::shared_ptr<chrono::fea::ChElementBase>> elements = m_mesh->GetElements();
	std::vector<std::shared_ptr<chrono::fea::ChNodeFEAbase>> nodes = m_mesh->GetNodes();

	std::vector<std::shared_ptr<chrono::fea::ChElementBase>> newElements = elements;
	std::vector<std::shared_ptr<chrono::fea::ChNodeFEAbase>> newNodes = nodes;

	for (uint16_t index = 0; index < importance.size(); ++index)
	{
		if (!importance[index] && m_cubesExistence[index])
		{
			m_cubesExistence[index] = false;
			int elementPosition = GetElementPositionFromImportanceVector(index);
			for (uint16_t indexNode = 0; indexNode < elements[elementPosition]->GetNnodes(); ++indexNode)
			{
				if (auto node = std::dynamic_pointer_cast<chrono::fea::ChNodeFEAxyz>(elements[elementPosition]->GetNodeN(indexNode)))
				{
					if (!ExistsAnotherElementWithNode(node, newElements))
					{
						int nodePosition = GetNodePositionInVector(node, newNodes);
						if (nodePosition != -1)
						{
							newNodes.erase(newNodes.begin() + nodePosition);
						}
					}
				}
			}

			auto elementIterator = std::find(newElements.begin(), newElements.end(), elements[elementPosition]);
			newElements.erase(elementIterator);
		}
	}

	m_mesh->ClearNodes();
	for (const auto& node : newNodes)
	{
		m_mesh->AddNode(node);
	}

	m_mesh->ClearElements();
	for (const auto& element : newElements)
	{
		m_mesh->AddElement(element);
	}

	EliminateConstaints();
}

void Building::AddCubesBasedOnCubesExistence(const std::vector<bool>& importance)
{
	std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> nodesLeftSide;
	std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> nodesRightSide;

	for (uint16_t index = 0; index < importance.size(); ++index)
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

int Building::GetElementPositionFromImportanceVector(int position)
{
	uint16_t layerOx = position % m_numberOfCubesOx;
	uint16_t layerOy = position / (m_cubesExistence.size() / m_numberOfCubesOy);
	uint16_t layerOz = (position / m_numberOfCubesOx) % m_numberOfCubesOz;

	std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> nodesLeftSide = BuildLateralNodesForLayers(layerOx, layerOy, layerOz);
	std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> nodesRightSide = BuildLateralNodesForLayers(layerOx + 1, layerOy, layerOz);

	nodesLeftSide.insert(nodesLeftSide.end(), nodesRightSide.begin(), nodesRightSide.end());

	std::vector<std::shared_ptr<chrono::fea::ChElementBase>> elements = m_mesh->GetElements();

	for (int index = 0; index < m_mesh->GetNelements(); ++index)
	{
		bool areNodesTheSame = true;
		for (uint16_t indexNode = 0; indexNode < elements[index]->GetNnodes(); ++indexNode)
		{
			if (auto node = std::dynamic_pointer_cast<chrono::fea::ChNodeFEAxyz>(elements[index]->GetNodeN(indexNode)))
			{
				if (!AreNodesEqual(nodesLeftSide[indexNode], node)) {
					areNodesTheSame = false;
					break;
				}
			}
		}
		if (areNodesTheSame)
			return index;
	}
	return -1;
}

bool Building::AreNodesEqual(const std::shared_ptr<chrono::fea::ChNodeFEAxyz>& lhsNode,
	const std::shared_ptr<chrono::fea::ChNodeFEAxyz>& rhsNode)
{
	if (lhsNode->GetPos().x() == rhsNode->GetPos().x() &&
		lhsNode->GetPos().y() == rhsNode->GetPos().y() &&
		lhsNode->GetPos().z() == rhsNode->GetPos().z())

		return true;

	return false;
}

bool Building::NodeAlreadyExistsInMesh(const std::shared_ptr<chrono::fea::ChNodeFEAxyz>& node)
{
	for (uint16_t indexNode = 0; indexNode < m_mesh->GetNnodes(); ++indexNode)
	{
		if (auto existentNode = std::dynamic_pointer_cast<chrono::fea::ChNodeFEAxyz>(m_mesh->GetNode(indexNode)))
		{
			if (AreNodesEqual(existentNode, node))
				return true;
		}
	}

	return false;
}

bool Building::ExistsAnotherElementWithNode(const std::shared_ptr<chrono::fea::ChNodeFEAxyz>& node,
	const std::vector<std::shared_ptr<chrono::fea::ChElementBase>>& elements)
{
	int numberOfElements = 0;

	for (const auto& element : elements)
	{
		for (uint16_t indexNode = 0; indexNode < element->GetNnodes(); ++indexNode)
		{
			if (auto elementNode = std::dynamic_pointer_cast<chrono::fea::ChNodeFEAxyz>(element->GetNodeN(indexNode)))
			{
				if (AreNodesEqual(node, elementNode))
				{
					++numberOfElements;
					if (numberOfElements > 1)
						return true;
				}
			}
		}
	}
	return false;
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

int Building::GetNodePositionInVector(const std::shared_ptr<chrono::fea::ChNodeFEAxyz>& node,
	const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAbase>>& nodes)
{
	for (uint16_t indexNode = 0; indexNode < nodes.size(); ++indexNode)
	{
		if (auto existentNode = std::dynamic_pointer_cast<chrono::fea::ChNodeFEAxyz>(nodes[indexNode]))
		{
			if (AreNodesEqual(existentNode, node))
			{
				return indexNode;
			}
		}
	}
	return -1;
}