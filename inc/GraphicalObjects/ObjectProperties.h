#pragma once

#include <chrono/fea/ChContinuumMaterial.h>
#include <chrono/fea/ChVisualizationFEAmesh.h>

#include <Services/AlgorithmSettings.h>

class ObjectProperties
{
public:
	static std::shared_ptr<chrono::fea::ChContinuumElastic> SetMaterial();

	static void SetVisualizationMesh(const std::shared_ptr<chrono::fea::ChMesh>& mesh,
		const std::shared_ptr<chrono::fea::ChVisualizationFEAmesh>& visualizeMesh);
	static void SetVisualizationMeshReference(const std::shared_ptr<chrono::fea::ChMesh>& mesh,
		const std::shared_ptr<chrono::fea::ChVisualizationFEAmesh>& visualizeMeshReference);
	static void SetVisualizationMeshPoints(const std::shared_ptr<chrono::fea::ChMesh>& mesh,
		const std::shared_ptr<chrono::fea::ChVisualizationFEAmesh>& visualizeMeshPoints);
};
