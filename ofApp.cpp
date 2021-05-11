#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(3);
	ofEnableDepthTest();

	ofColor color;
	vector<int> hex_list = { 0x247BA0, 0x70C1B3, 0xB2DBBF, 0xF3FFBD, 0xFF1654 };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->base_color_list.push_back(color);
	}

	for (int r = 200; r <= 250; r += 10) {

		auto ico_sphere = ofIcoSpherePrimitive(r, 3);
		this->triangle_list.insert(this->triangle_list.end(), ico_sphere.getMesh().getUniqueFaces().begin(), ico_sphere.getMesh().getUniqueFaces().end());
	}

	for (auto& triangle : this->triangle_list) {

		glm::vec3 avg = (triangle.getVertex(0) + triangle.getVertex(1) + triangle.getVertex(2)) / 3;
		this->location_list.push_back(avg);
		this->param_list.push_back(0);
		this->param_color_list.push_back(ofColor(239));
	}

	int span = 25;
	for (auto& location : this->location_list) {

		vector<int> route_info = vector<int>();
		int index = -1;
		for (auto& other : this->location_list) {

			index++;
			if (location == other) { continue; }

			float distance = glm::distance(location, other);
			if (distance <= span) {

				if (abs(glm::length(location) - glm::length(other)) <= 10) {

					route_info.push_back(index);
				}
			}
		}

		this->route_info_list.push_back(route_info);
	}

	for (int i = 0; i < 100; i++) {

		this->index_list.push_back((int)ofRandom(this->location_list.size()));
		this->index_color_list.push_back(this->base_color_list[ofRandom(this->base_color_list.size())]);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	this->mesh.clear();
	this->frame.clear();

	for (int i = 0; i < this->index_list.size(); i++) {

		int next_index = this->route_info_list[this->index_list[i]][(int)ofRandom(this->route_info_list[this->index_list[i]].size())];
		for (int k = 0; k < this->route_info_list[this->index_list[i]].size(); k++) {

			if (this->param_list[next_index] <= 0) {

				this->param_list[next_index] = 30;
				this->param_color_list[next_index] = this->index_color_list[i];
				this->index_list[i] = next_index;
				break;
			}
		}
	}

	for (int i = 0; i < this->location_list.size(); i++) {

		if (this->param_list[i] > 0) {

			int index = this->mesh.getNumVertices();

			this->mesh.addVertex(this->triangle_list[i].getVertex(0));
			this->mesh.addVertex(this->triangle_list[i].getVertex(1));
			this->mesh.addVertex(this->triangle_list[i].getVertex(2));

			this->frame.addVertex(this->triangle_list[i].getVertex(0));
			this->frame.addVertex(this->triangle_list[i].getVertex(1));
			this->frame.addVertex(this->triangle_list[i].getVertex(2));

			this->mesh.addColor(this->param_color_list[i]);
			this->mesh.addColor(this->param_color_list[i]);
			this->mesh.addColor(this->param_color_list[i]);

			this->frame.addColor(ofColor(39));
			this->frame.addColor(ofColor(39));
			this->frame.addColor(ofColor(39));

			this->mesh.addTriangle(index + 0, index + 1, index + 2);
			this->frame.addTriangle(index + 0, index + 1, index + 2);
		}

		if (this->param_list[i] > 0) { this->param_list[i] -= 1; }
	}

}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(ofGetFrameNum() * 0.37);
	ofRotateY(ofGetFrameNum() * 0.72);

	this->mesh.drawFaces();

	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}