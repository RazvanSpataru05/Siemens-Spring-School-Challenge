#include <GraphicalObjects/ObjectProperties.h>

std::shared_ptr<chrono::fea::ChContinuumElastic> ObjectProperties::SetMaterial()
{
	auto material = std::make_shared<chrono::fea::ChContinuumElastic>();

	material->Set_E(AlgorithmSettings::GetInstance()->GetYoungModulus());
	material->Set_v(AlgorithmSettings::GetInstance()->GetPoissonRatio());
	material->Set_RayleighDampingK(0.001);
	material->Set_density(AlgorithmSettings::GetInstance()->GetDensity());
	
	return material;
}

void ObjectProperties::SetVisualizationMesh(const std::shared_ptr<chrono::fea::ChMesh>& mesh, 
	const std::shared_ptr<chrono::fea::ChVisualizationFEAmesh>& visualizeMesh)
{
	visualizeMesh->SetFEMdataType(chrono::fea::ChVisualizationFEAmesh::E_PLOT_NODE_SPEED_NORM);
	visualizeMesh->SetColorscaleMinMax(0.0, 5.50);
	visualizeMesh->SetShrinkElements(false, 0);
	visualizeMesh->SetSmoothFaces(true);
	mesh->AddAsset(visualizeMesh);
}

void ObjectProperties::SetVisualizationMeshReference(const std::shared_ptr<chrono::fea::ChMesh>& mesh, 
	const std::shared_ptr<chrono::fea::ChVisualizationFEAmesh>& visualizeMeshReference)
{
	visualizeMeshReference->SetFEMdataType(chrono::fea::ChVisualizationFEAmesh::E_PLOT_SURFACE);
	visualizeMeshReference->SetWireframe(true);
	visualizeMeshReference->SetDrawInUndeformedReference(true);
	mesh->AddAsset(visualizeMeshReference);
}

void ObjectProperties::SetVisualizationMeshPoints(const std::shared_ptr<chrono::fea::ChMesh>& mesh, 
	const std::shared_ptr<chrono::fea::ChVisualizationFEAmesh>& visualizeMeshPoints)
{
	visualizeMeshPoints->SetFEMglyphType(chrono::fea::ChVisualizationFEAmesh::E_GLYPH_NODE_DOT_POS);
	visualizeMeshPoints->SetFEMdataType(chrono::fea::ChVisualizationFEAmesh::E_PLOT_NONE);
	visualizeMeshPoints->SetSymbolsThickness(0.006);
	mesh->AddAsset(visualizeMeshPoints);
}
