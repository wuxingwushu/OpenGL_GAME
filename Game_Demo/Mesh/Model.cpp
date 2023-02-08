#include "Model.h"
#include "../Read/stb_image.h"

Model::Model(const string & path)
{
	LoadModel(path);
}

Model::~Model()
{
}

void Model::proessNode(aiNode *node, const aiScene *scene)
{
	for (int i = 0; i < node->mNumMeshes; ++i) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.emplace_back(proessMesh(mesh, scene));
	}

	for (int i = 0; i < node->mNumChildren; ++i) {
		proessNode(node->mChildren[i], scene);
	}
}

Mesh Model::proessMesh(aiMesh * mesh, const aiScene * scene)
{
	vector<Vertex> vertex;
	vector<unsigned int> idxes;
	vector<Texture> textures;

	// mesh->Vertex
	for (int i = 0; i < mesh->mNumVertices; ++i) {
		Vertex buf;
		aiVector3D vector3 = mesh->mVertices[i];
		aiVector3D normal;
		aiVector3D texture;
		buf.positoin = vec3(vector3.x, vector3.y, vector3.z);

		if (mesh->HasNormals()) {
			normal = mesh->mNormals[i];
			buf.normal = vec3(normal.x, normal.y, normal.z);
		}

		if (mesh->mTextureCoords[0]) {
			texture = mesh->mTextureCoords[0][i];
			buf.texCoords = vec2(texture.x, texture.y);
		}
		else {
			buf.texCoords = vec2(0.0f, 0.0f);
		}
		vertex.emplace_back(buf);
	}

	// mesh->indices
	for (int i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; ++j) {
			idxes.emplace_back(face.mIndices[j]);
		}
	}

	// mesh->texture
	aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];

	vector<Texture> diffuse = loadMaterialTexture(mat, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuse.begin(), diffuse.end());

	vector<Texture> specular = loadMaterialTexture(mat, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specular.begin(), specular.end());

	return Mesh(vertex, idxes, textures);

}

vector<Texture> Model::loadMaterialTexture(aiMaterial * mat, aiTextureType type, string type_name)
{
	vector<Texture> texture;
	for (int i = 0; i < mat->GetTextureCount(type); ++i) {
		aiString path;
		mat->GetTexture(type, i, &path);

		bool skip = false;
		for (int i = 0; i < texture_loaded.size(); ++i) {
			if (strcmp(texture_loaded[i].path.data(), path.C_Str()) == 0) {
				texture.emplace_back(texture_loaded[i]);
				skip = true;
				break;
			}
		}

		if (!skip) {
			Texture tex;
			tex.id = GetTextrueID(path.C_Str());
			tex.type = type_name;
			tex.path = path.C_Str();
			texture_loaded.emplace_back(tex);
			texture.emplace_back(tex);
		}
	} 

	return texture;
}

// get texture id.
GLuint Model::GetTextrueID(const char *path)
{
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	stbi_set_flip_vertically_on_load(true);  // 翻转

	string filename = this->directory + '\\' + path;

	cout << filename << endl;

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data) 
	{
		GLenum format = GL_RGB;
		if (nrComponents == 1) format = GL_RED;
		else if (nrComponents == 3) format = GL_RGB;
		else if (nrComponents == 4) format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		/*第一个参数指定了纹理目标(Target)。设置为GL_TEXTURE_2D意味着会生成与当前绑定的纹理对象在同一个目标上的纹理（任何绑定到GL_TEXTURE_1D和GL_TEXTURE_3D的纹理不会受到影响）。
		  第二个参数为纹理指定多级渐远纹理的级别，如果你希望单独手动设置每个多级渐远纹理的级别的话。这里我们填0，也就是基本级别。
		  第三个参数告诉OpenGL我们希望把纹理储存为何种格式。我们的图像只有RGB值，因此我们也把纹理储存为RGB值。
		  第四个和第五个参数设置最终的纹理的宽度和高度。我们之前加载图像的时候储存了它们，所以我们使用对应的变量。
		  下个参数应该总是被设为0（历史遗留的问题）。
		  第七第八个参数定义了源图的格式和数据类型。我们使用RGB值加载这个图像，并把它们储存为char(byte)数组，我们将会传入对应值。
		  最后一个参数是真正的图像数据。*/
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << filename  << " failed teuture id !" << endl;
	}
	stbi_image_free(data);
	return texture_id;
}

void Model::LoadModel(const string path)
{
	Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "failed model flie! error is : " << importer.GetErrorString() << endl;
		return;
	}

	directory = path.substr(0, path.find_last_of("\\"));
	proessNode(scene->mRootNode, scene);
}


void Model::Draw(Shader * myShader)
{
	for (int i = 0; i < meshes.size(); ++i) {
		meshes[i].Draw(myShader);
	}
}