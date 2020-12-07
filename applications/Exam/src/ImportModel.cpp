#include "ImportModel.h"

ImportModel::ImportModel() {

}

ImportModel::~ImportModel() {

}

ImportModel::ImportModel(std::shared_ptr<Window>& mainWindow) {

	generateModel();
}

void ImportModel::generateModel() {

	importModel = std::make_unique<Model>();
	importModel->loadModel("assets/models/ghost.obj");
}

void ImportModel::drawModel(glm::mat4 model, glm::vec3 position, GLuint uniformModel) {

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	importModel->renderElements();
}
