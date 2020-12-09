#include "InstancedModel.h"

InstancedModel::InstancedModel() {

}

InstancedModel::~InstancedModel() {

}

InstancedModel::InstancedModel(std::shared_ptr<Window>& mainWindow) {

	int x = 0;

	numInstanced = 5;

	for (size_t i = 0; i < numInstanced; i++) {

		glm::vec3 pos(500.0f + x, 100.0f, 500.0f);
		instancedPositions.push_back(pos);
		
		x += 5;
	}

	generateInstanced();
}

void InstancedModel::generateInstanced() {

	instancedModel = std::make_unique<Model>();
	instancedModel->loadModel("assets/models/Tree/PineTree.obj");

	modelMatrices = new glm::mat4[numInstanced];

	for (unsigned int i = 0; i < numInstanced; i++) {

		model = glm::mat4(1.0f);
		model = glm::translate(model, instancedPositions[i]);
		modelMatrices[i] = model;
	}

	instancedModelVAO = std::make_shared<VertexArray>();
	instancedModelVAO = instancedModel->getVertexArray();
	instancedModelVAO->bind();

	instancedModelVBO = std::make_shared<VertexBuffer>(&modelMatrices[0], numInstanced * sizeof(glm::mat4));
	instancedModelVBO->bind();
			 
	instancedModelVAO->addBufferDivisor();
			 
	instancedModelVAO->unbind();
}

void InstancedModel::drawInstanced(std::shared_ptr<Camera>& camera, std::shared_ptr<Shader>& instanced_shader, glm::mat4 projection) {

	uniformView = instanced_shader->getViewLocation();
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));

	uniformProjection = instanced_shader->getProjectionLocation();
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

	instancedModel->renderInstanced(numInstanced);
}
