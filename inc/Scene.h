#pragma once

#include <chrono/fea/ChLinkPointFrame.h>
#include <chrono/physics/ChSystemSMC.h>

#include <GraphicalObjects/Building.h>

#include <System/ConfigureSystem.h>

class Scene
{
public:
	Scene();
	~Scene() = default;

	void Show(const std::shared_ptr<Building>& building);

	std::shared_ptr<Building> CreateCustomWallScene();

private:
	std::shared_ptr<Building> CreateSimpleWallScene();
	std::shared_ptr<Building> CreateComplexWallScene();

	void InitializeSystem(const std::shared_ptr<chrono::ChSystemSMC>& system);
	void SetVisualizationProperties(const std::shared_ptr<Building>& building);
};