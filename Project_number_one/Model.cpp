#define _CRT_SECURE_NO_WARNINGS
#include "Model.h"
#include <cstdio>
#include <cstring>
#include <iostream>

Model::Model() {
    displayListID = 0;
    isCompiled = false;
}

Model::~Model() {
    if (displayListID != 0) {
        glDeleteLists(displayListID, 1);
    }
}

// --- دالة التحميل (السريعة) ---
void Model::load(const char* filename) {
    std::cout << "\n[Model] Loading: " << filename << "..." << std::endl;

    FILE* file = fopen(filename, "r");
    if (!file) {
        std::cerr << "[Error] File not found!" << std::endl;
        return;
    }

    std::vector<Vec3> temp_v;
    std::vector<Vec2> temp_vt;
    std::vector<Vec3> temp_vn;

    std::string currentMat = "default";
    meshes[currentMat] = SubMesh();
    meshOrder.push_back(currentMat);

    char line[1024];
    while (fgets(line, 1024, file)) {
        char header[128];
        if (sscanf(line, "%s", header) == EOF) continue;

        if (strcmp(header, "usemtl") == 0) {
            char matName[128];
            sscanf(line, "%*s %s", matName);
            currentMat = std::string(matName);
            if (meshes.find(currentMat) == meshes.end()) {
                meshes[currentMat] = SubMesh();
                meshOrder.push_back(currentMat);
                std::cout << "  > Part: " << currentMat << std::endl;
            }
        }
        else if (strcmp(header, "v") == 0) {
            Vec3 v; sscanf(line, "%*s %f %f %f", &v.x, &v.y, &v.z);
            temp_v.push_back(v);
        }
        else if (strcmp(header, "vt") == 0) {
            Vec2 vt; sscanf(line, "%*s %f %f", &vt.x, &vt.y);
            temp_vt.push_back(vt);
        }
        else if (strcmp(header, "vn") == 0) {
            Vec3 vn; sscanf(line, "%*s %f %f %f", &vn.x, &vn.y, &vn.z);
            temp_vn.push_back(vn);
        }
        else if (strcmp(header, "f") == 0) {
            int v[4], vt[4], vn[4];
            int matches = sscanf(line, "%*s %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
                &v[0], &vt[0], &vn[0], &v[1], &vt[1], &vn[1], &v[2], &vt[2], &vn[2], &v[3], &vt[3], &vn[3]);

            if (matches != 9 && matches != 12) {
                matches = sscanf(line, "%*s %d//%d %d//%d %d//%d %d//%d",
                    &v[0], &vn[0], &v[1], &vn[1], &v[2], &vn[2], &v[3], &vn[3]);
                vt[0] = vt[1] = vt[2] = vt[3] = 0;
            }

            int count = (matches == 12 || matches == 8) ? 4 : 3;
            SubMesh& currentMesh = meshes[currentMat];

            for (int i = 0; i < 3; i++) {
                if (v[i] > 0 && v[i] <= temp_v.size()) currentMesh.vertices.push_back(temp_v[v[i] - 1]);
                if (vt[i] > 0 && vt[i] <= temp_vt.size()) currentMesh.uvs.push_back(temp_vt[vt[i] - 1]);
                if (vn[i] > 0 && vn[i] <= temp_vn.size()) currentMesh.normals.push_back(temp_vn[vn[i] - 1]);
            }
            if (count == 4) {
                int idxs[] = { 0, 2, 3 };
                for (int i : idxs) {
                    if (v[i] > 0 && v[i] <= temp_v.size()) currentMesh.vertices.push_back(temp_v[v[i] - 1]);
                    if (vt[i] > 0 && vt[i] <= temp_vt.size()) currentMesh.uvs.push_back(temp_vt[vt[i] - 1]);
                    if (vn[i] > 0 && vn[i] <= temp_vn.size()) currentMesh.normals.push_back(temp_vn[vn[i] - 1]);
                }
            }
        }
    }
    fclose(file);
}

// --- دالة تعيين التيكستشر ---
void Model::assignTexture(std::string materialName, unsigned int texID) {
    if (meshes.find(materialName) != meshes.end()) {
        meshes[materialName].textureID = texID;
    }
}

// --- دالة تعيين اللون ---
void Model::assignColor(std::string materialName, float r, float g, float b) {
    if (meshes.find(materialName) != meshes.end()) {
        meshes[materialName].r = r;
        meshes[materialName].g = g;
        meshes[materialName].b = b;
    }
}

// --- دالة تجميع القائمة (للتسريع) ---
void Model::compileDisplayList() {
    displayListID = glGenLists(1);
    glNewList(displayListID, GL_COMPILE);

    for (const std::string& name : meshOrder) {
        SubMesh& mesh = meshes[name];
        if (mesh.vertices.empty()) continue;

        if (mesh.textureID != 0) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, mesh.textureID);
            glColor3f(mesh.r, mesh.g, mesh.b);
        }
        else {
            glDisable(GL_TEXTURE_2D);
            glColor3f(mesh.r, mesh.g, mesh.b);
        }

        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < mesh.vertices.size(); i++) {
            if (i < mesh.normals.size()) glNormal3f(mesh.normals[i].x, mesh.normals[i].y, mesh.normals[i].z);
            if (i < mesh.uvs.size()) glTexCoord2f(mesh.uvs[i].x, mesh.uvs[i].y);
            glVertex3f(mesh.vertices[i].x, mesh.vertices[i].y, mesh.vertices[i].z);
        }
        glEnd();

        if (mesh.textureID != 0) glDisable(GL_TEXTURE_2D);
    }

    glEndList();
    isCompiled = true;
}

// --- دالة الرسم ---
void Model::draw() {
    if (!isCompiled) {
        compileDisplayList();
    }
    glCallList(displayListID);
}
