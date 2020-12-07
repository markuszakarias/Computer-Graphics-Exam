#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <memory>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Material.h"

#include "IndexBuffer.h"
#include "VertexArray.h"

#include "Renderer.h"

class Model {

private:

	std::shared_ptr<VertexArray>		modelVAO;
	std::unique_ptr<VertexBuffer>		modelVBO;
	std::unique_ptr<VertexBufferLayout> modelVBLayout;
	std::shared_ptr<IndexBuffer>		modelIBO;

	std::shared_ptr<Renderer> modelRenderer;

	void loadNode(aiNode* node, const aiScene* scene);
	void loadMesh(aiMesh* mesh, const aiScene* scene);
	void loadMaterials(const aiScene* scene);

	std::vector<unsigned int> meshToTex;

public:

	Model();
	~Model();

	void loadModel(const std::string& fileName);
	void renderElements();
	void renderInstanced(int numInstanced);

	auto getVertexArray() { return modelVAO; }

	std::vector<std::unique_ptr<Material>> textureList;
	std::vector<std::shared_ptr<Renderer>> rendererList;

};