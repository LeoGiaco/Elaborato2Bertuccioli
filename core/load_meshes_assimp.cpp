#include "load_meshes_assimp.hpp"

bool loadAssImp(const char *path, vector<Shape3D> &meshVector)
{

	Assimp::Importer importer;

	const aiScene *scene = importer.ReadFile(path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene)
	{
		fprintf(stderr, importer.GetErrorString());
		getchar();
		return false;
	}
	const aiMesh *mesh;

	// Fill vertices positions
	int num_meshes = scene->mNumMeshes; // Numero di oggetti che compongono il modello
	meshVector.resize(num_meshes);		// Faccio il resize di meshVector al numero di mesh che compongono l'oggetto

	for (unsigned int nm = 0; nm < num_meshes; nm++)
	{
		// Per ogni mesh dell'oggetto
		mesh = scene->mMeshes[nm];
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
			meshVector[nm].materiale.diffuse = glm::vec3(color.r, color.g, color.b);
		}
		else
		{
			meshVector[nm].materiale.diffuse = glm::vec3(1.0, 0.2, 0.1);
		}

		if (aiReturn_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, color))
		{
			meshVector[nm].materiale.specular = glm::vec3(color.r, color.g, color.b);
		}

		else
		{
			printf("Errore in specular \n");
			meshVector[nm].materiale.specular = glm::vec3(0.5, 0.5, 0.5);
		}
		if (aiReturn_SUCCESS == material->Get(AI_MATKEY_SHININESS_STRENGTH, value))
		{
			meshVector[nm].materiale.shininess = value;
		}
		else
		{
			// printf("Errore in shininess \n");
			meshVector[nm].materiale.shininess = 50.0f;
		}

		// �http://assimp.sourceforge.net/lib_html/structai_material.html

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{

			aiVector3D pos = mesh->mVertices[i];
			meshVector[nm].vertici.push_back(glm::vec3(pos.x, pos.y, pos.z));
		}

		// Fill vertices texture coordinates

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			meshVector[nm].texCoords.push_back(glm::vec2(0.0, 0.0));
			meshVector[nm].colori.push_back(vec4(1.0, 0.0, 0.0, 1.0));
		}

		// Fill vertices normals

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			aiVector3D n = mesh->mNormals[i];
			meshVector[nm].normali.push_back(glm::vec3(n.x, n.y, n.z));
		}

		// Fill face indices

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			// Assume the model has only triangles.
			meshVector[nm].indici.push_back(mesh->mFaces[i].mIndices[0]);
			meshVector[nm].indici.push_back(mesh->mFaces[i].mIndices[1]);
			meshVector[nm].indici.push_back(mesh->mFaces[i].mIndices[2]);
		}
	}
	return true;
}
