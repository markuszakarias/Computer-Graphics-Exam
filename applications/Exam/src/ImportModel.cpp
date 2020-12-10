#include "ImportModel.h"

ImportModel::ImportModel() {

}

ImportModel::~ImportModel() {

}

ImportModel::ImportModel(std::shared_ptr<Window>& mainWindow, glm::vec3 pos) {

	position = pos;

	velocities.push_back(glm::vec3(0.f, 1.f, 0.f));
	velocities.push_back(glm::vec3(0.f, -1.f, 0.f));
	velocities.push_back(glm::vec3(1.f, 0.f, 0.f));
	velocities.push_back(glm::vec3(-1.f, 0.f, 0.f));
	velocities.push_back(glm::vec3(0.f, 0.f, 1.f));
	velocities.push_back(glm::vec3(0.f, 0.f, -1.f));

	generateModel();
}

void ImportModel::generateModel() {

	importModel = std::make_unique<Model>();
	importModel->loadModel("assets/models/Eagle/Eagle02.obj");
}

void ImportModel::updateMovement(GLfloat time) {

	float dt = time;
	
	int i = rand() % 5;

	position += velocities[i] * dt;

	std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;

}

void ImportModel::drawModel(glm::mat4 model, GLuint uniformModel) {

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(this->position));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	importModel->renderElements();
}
