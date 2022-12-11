#include "load_meshes_assimp.hpp"

bool loadAssImp(string path, GLProgram *program, vector<Shape3D *> &meshVector)
{

	Assimp::Importer importer;

	const aiScene *scene = importer.ReadFile(path.c_str(), aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene)
	{
		fprintf(stderr, importer.GetErrorString());
		getchar();
		return false;
	}
	const aiMesh *mesh;

	// Fill vertices positions
	int num_meshes = scene->mNumMeshes; // Numero di oggetti che compongono il modello

	for (unsigned int nm = 0; nm < num_meshes; nm++)
	{
		// Per ogni mesh dell'oggetto
		mesh = scene->mMeshes[nm];

		vector<vec3> vertices;
		// vector<vec2> texCoords;
		vector<vec3> normals;
		vector<uint> indices;
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

		aiColor3D color;
		float value;
		Material mat;
		// Read mtl file vertex data

		if (aiReturn_SUCCESS == material->Get(AI_MATKEY_COLOR_AMBIENT, color))
		{
			mat.ambient = glm::vec3(color.r, color.g, color.b);
		}
		else
		{
			printf("Errore in ambient \n");
		}

		if (aiReturn_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, color))
		{
			mat.diffuse = glm::vec3(color.r, color.g, color.b);
		}
		else
		{
			mat.diffuse = glm::vec3(1.0, 0.2, 0.1);
		}

		if (aiReturn_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, color))
		{
			mat.specular = glm::vec3(color.r, color.g, color.b);
		}
		else
		{
			printf("Errore in specular \n");
			mat.specular = glm::vec3(0.5, 0.5, 0.5);
		}
		if (aiReturn_SUCCESS == material->Get(AI_MATKEY_SHININESS_STRENGTH, value))
		{
			mat.shininess = value;
		}
		else
		{
			// printf("Errore in shininess \n");
			mat.shininess = 50.0f;
		}

		// �http://assimp.sourceforge.net/lib_html/structai_material.html

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{

			aiVector3D pos = mesh->mVertices[i];
			vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
		}

		// Fill vertices texture coordinates

		// for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		// {
		// 	texCoords.push_back(glm::vec2(0.0, 0.0));
		// }

		// Fill vertices normals

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			aiVector3D n = mesh->mNormals[i];
			normals.push_back(glm::vec3(n.x, n.y, n.z));
		}

		// Fill face indices

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			// Assume the model has only triangles.
			indices.push_back(mesh->mFaces[i].mIndices[0]);
			indices.push_back(mesh->mFaces[i].mIndices[1]);
			indices.push_back(mesh->mFaces[i].mIndices[2]);
		}

		meshVector.push_back(Mesh::create(program, vertices, normals, indices, vector<vec2>(), mat, GL_TRIANGLES));
	}
	return true;
}
