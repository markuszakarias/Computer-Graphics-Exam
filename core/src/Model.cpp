#include "Model.h"

/**
*  Assimp functionality is mostly served through this class. Loading and rendering.
*  Together with the mesh class this constructs the models.
*
*  @name Model.cpp
*  @author Markus Zakarias Faljord Strømseth, NTNU
*/

/**
*	Default constructor for the Model class
*
*/
Model::Model() {

}

/**
*  @brief Loads the model file by passing in the name as a parameter.
*
*  @param	fileName - The model files name gets passed and read by Assimp.
* 
*  @see		loadNode(aiNode, aiScene), loadMaterials
* 
*/
void Model::loadModel(const std::string& fileName) {

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene) {

		std::cout << "Model (" << fileName << ") failed to load: " << importer.GetErrorString() << std::endl;
		return;
	}

	loadNode(scene->mRootNode, scene);

	loadMaterials(scene);
}

/**
*	Loads the node, with the Assimp node and Assimp scene. It goes through the number of meshes, and children,
*	model has. This is for models that have multiple meshes combined. It works in a recursive way for the node.
*
*	@param	node  - The Assimp aiNode
*	@param	scene - The Assimp aiScene
*	
*	@see	loadMesh(), loadNode()
* 
*/
void Model::loadNode(aiNode* node, const aiScene* scene) {

	for (size_t i = 0; i < node->mNumMeshes; i++) {

		loadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++) {

		loadNode(node->mChildren[i], scene);
	}

}

/**
*	Load the materials for the model. It will go through the assigned path and look for the
*	materials set from the .mtl file that is associated with the model. If it does not find
*	any, a plain texture gets applied to the model.
*
*	@param	scene - The Assimp aiScene
*
*	@see loadTextureA()
*
*/
void Model::loadMaterials(const aiScene* scene) {

	textureList.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++) {

		aiMaterial* material = scene->mMaterials[i];

		textureList[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE)) {

			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {

				unsigned long int idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx + 1);

				std::string texPath = std::string("assets/textures/") + filename;

				textureList[i] = std::make_unique<Material>();
				textureList[i]->getTexture(texPath.c_str());

				if (!textureList[i]->loadTexture()) {

					std::cout << "Failed to load texture at: " << texPath << std::endl;
					textureList[i] = nullptr;
				}
			}
		}

		if (!textureList[i]) {

			textureList[i] = std::make_unique<Material>();
			textureList[i]->getTexture("assets/textures/plain.png"); // In case no texture is found
			textureList[i]->loadTextureA();				// we set a standard texture.
		}
	}
}

/**
*	Inserts all of the models vertices and indices into their vectors. Then the mesh i loaded from
*	external classes to create, set the layout and bind the mesh. Then a Renderer object is passed
*	into a vector of Renderer objects, for later use in rendering the mesh. 
*
*	@param	node  - The Assimp aiNode
*	@param	scene - The Assimp aiScene
* 
*	@see VertexArray(), VertexBuffer(), VertexBufferLayout(), IndexBuffer()
* 
*/
void Model::loadMesh(aiMesh* mesh, const aiScene* scene) {

	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++) {

		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
		
		if (mesh->mTextureCoords[0]) {

			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else {
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}

		vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++) {

		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++) {

			indices.push_back(face.mIndices[j]);
		}
	}

	modelVAO = std::make_shared<VertexArray>();
	modelVAO->bind();
	
	modelVBO = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(GLfloat));
	modelVBO->bind();
	
	modelVBLayout = std::make_unique<VertexBufferLayout>();
	modelVBLayout->Push<float>(3);
	modelVBLayout->Push<float>(2);
	modelVBLayout->Push<float>(3);
	
	modelVAO->addBuffer(*modelVBO, *modelVBLayout);
	modelIBO = std::make_shared<IndexBuffer>(&indices[0], indices.size());

	rendererList.push_back(modelRenderer);

	meshToTex.push_back(mesh->mMaterialIndex);
}



/**
*	Renders the imported mesh by using the Renderer class. The draw method here is regular
*	drawing. This is in case we want to render one, or very few, objects.
*
*	@see	drawElements()
*/
void Model::renderElements() {

	for (size_t i = 0; i < rendererList.size(); i++) {
		unsigned int materialIndex = meshToTex[i];

		if (materialIndex < textureList.size() && textureList[materialIndex]) {

			textureList[materialIndex]->useTexture();
		}

		rendererList[i]->drawElements(modelVAO, modelIBO);
	}
}

/**
*	Renders the imported mesh by using the Renderer class. The draw method here is Instanced
*	drawing. This is in case we want to render many objects, like pellets, which are the same.
*
*	@see	drawInstaned()
*/
void Model::renderInstanced(int numInstanced) {

	for (size_t i = 0; i < rendererList.size(); i++) {

		unsigned int materialIndex = meshToTex[i];

		if (materialIndex < textureList.size() && textureList[materialIndex]) {

			textureList[materialIndex]->useTexture();
		}

		rendererList[i]->drawInstanced(modelVAO, modelIBO, numInstanced);
	}
}

/**
*	Destructor for the model object
*
*/
Model::~Model() {
	
}
