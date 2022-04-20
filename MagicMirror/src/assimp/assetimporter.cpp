#include "assetimporter.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

#include"../opengl/material.h"

#include <vector>
#include <iostream>

#define DIR "res/models/"

namespace assetimporter {


	// Stellt sicher, dass der Dateipfad der Texturen richitg ist
	std::string checkTexturePath(const std::string& aPath, const std::string& dir) {

		int start = aPath.find_last_of('\\');

		if (start == std::string::npos) {
			return dir + aPath;
		}


		return dir + aPath.substr(start + 1);
	}

	//L�dt Texturen eines Materials
	std::vector<Texture> loadTextures(aiMaterial* mat, aiTextureType type, std::string name) {

		std::vector<Texture> textures;
		for (int i = 0; i < mat->GetTextureCount(type); i++) {
			aiString str;
			mat->GetTexture(type, i, &str);

			textures.push_back(Texture(std::string(str.C_Str())));
		}
		return textures;
	}

	// L�dt ein 3D-Modell
	void loadModel(const std::string& dir, 
					const std::string& file, 
					std::vector<Vertex>& vertices, 
					std::vector<uint32_t>& indices, 
					std::vector<std::string>& textures,
					Material& material) {

		std::string fullPath = dir + file;
		const aiScene* scene = aiImportFile(fullPath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

		if (!scene) {
			printf("Failed to load View Model: %s\n", fullPath.c_str());
			printf("Error:\n%s\n", aiGetErrorString());
			return;
		}

		


		for (uint32_t i = 0; i < scene->mNumMeshes; i++) {

			aiMesh* mesh = scene->mMeshes[i];
			
			vertices.reserve(mesh->mNumVertices);

			// Oberfl�chenwerte des Materiales laden
			aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
			aiColor4D specular;
			aiColor4D diffuse;
			aiColor4D ambient;
			float shininess;
			float specularStrength;
			float roughness;

			aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &specular);
			aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
			aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &ambient);
			aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &shininess);
			aiGetMaterialFloat(mat, AI_MATKEY_SPECULAR_FACTOR, &specularStrength);
			aiGetMaterialFloat(mat, AI_MATKEY_ROUGHNESS_FACTOR, &roughness);


			material = Material(
				{ specular.r, specular.g, specular.b, shininess },
				{ diffuse.r, diffuse.g, diffuse.b, diffuse.a },
				{ ambient.r, ambient.g, ambient.b, ambient.a },
				roughness, specularStrength
			);

			// Scheitelpunktinformationen laden
			for (uint32_t j = 0; j < mesh->mNumVertices; j++) {

				aiVector3D pos = mesh->mVertices[j];
				aiVector3D normal = mesh->mNormals[j];
				
				glm::vec2 uv(0.0f);
				if (mesh->mTextureCoords[0]) {
					// Has z component. Figure out for what
					aiVector3D texCoords = mesh->mTextureCoords[0][j];
					uv.x = texCoords.x;
					uv.y = texCoords.y;
				}

				vertices.push_back(
					Vertex(
						glm::vec3(pos.x, pos.y, pos.z), 
						glm::vec3(normal.x, normal.y, normal.y),
						uv)
				);

			}

			indices.reserve(mesh->mNumFaces * 3u);
			for (uint32_t j = 0; j < mesh->mNumFaces; j++) {
				
				//if (mesh->mFaces[j].mNumIndices < 3u) continue;

				assert(mesh->mFaces[j].mNumIndices == 3u);

				indices.push_back(mesh->mFaces[j].mIndices[0u]);
				indices.push_back(mesh->mFaces[j].mIndices[1u]);
				indices.push_back(mesh->mFaces[j].mIndices[2u]);
			
			}

			// Texturpfade auslesen
			if (mesh->mMaterialIndex >= 0) {
				aiMaterial* ai_mat = scene->mMaterials[mesh->mMaterialIndex];

				std::vector<Texture> diffuseMap = loadTextures(ai_mat, aiTextureType_SPECULAR, "diffuse_tex");
				std::vector<Texture> specularMap = loadTextures(ai_mat, aiTextureType_SPECULAR, "specular_tex");

				//textures.insert(textures.end(), diffuseMap.begin(), diffuseMap.end());
				//textures.insert(textures.end(), specularMap.begin(), specularMap.end());

			}


			for (uint32_t j = 0; j < scene->mNumMaterials; j++) {
				const aiMaterial* pMaterial = scene->mMaterials[j];
				if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
					aiString path;

					if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {

						textures.push_back(checkTexturePath(path.C_Str(), dir));
					}
					if (pMaterial->GetTexture(aiTextureType_HEIGHT, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
						textures.push_back(checkTexturePath(path.C_Str(), dir));
					}
				}
			}

		}

		// Belegten Arbeitsspeicher freigeben
		aiReleaseImport(scene);
	}

}