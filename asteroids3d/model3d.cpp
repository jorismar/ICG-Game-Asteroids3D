#include "model3d.h"

Model3D::Model3D() {
	this->scene = NULL;
	this->scene_list = 0;

	this->ar_scale[0] = 1.0;
	this->ar_scale[1] = 1.0;
	this->ar_scale[2] = 1.0;
}

Model3D::~Model3D() {

}

bool Model3D::importFrmFile(const std::string& path) {
	this->obj_path = path;

	std::ifstream filein(this->obj_path.c_str());		// Check if file exists

	ERR(filein.fail(), FILE_NOT_FOUND, false);			// Check and Capture Error

	filein.close();

	this->scene = this->importer.ReadFile(this->obj_path, aiProcessPreset_TargetRealtime_Quality);

	if (!this->scene) {
		std::cout << this->importer.GetErrorString() << std::endl;
		ERR(true, ASSIMP_IMPORTER_ERROR, false);	// Check and Capture Error
	}

	return true;
}

int Model3D::loadTexture() {
	ILboolean success;

	ERR(ilGetInteger(IL_VERSION_NUM) < IL_VERSION, IL_WRONG_VERSION, -1);	// Check and Capture Error

	ilInit();

	ERR(this->scene->HasTextures(), ASSIMP_NOT_HAS_TEXTURE, -2);			// Check and Capture Error

	for (unsigned int i = 0; i < this->scene->mNumMaterials; i++) {
		int texIndex = 0;
		aiReturn texFound = AI_SUCCESS;

		aiString path;

		while (texFound == AI_SUCCESS) {
			texFound = this->scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
			this->textureIdMap[path.data] = NULL; //fill map with textures, pointers still NULL yet
			texIndex++;
		}
	}

	int numTextures = this->textureIdMap.size();
	
	ILuint* imageIds = new ILuint[numTextures];		// array with DevIL image IDs

	//******************** generate DevIL Image IDs ********************
	ilGenImages(numTextures, imageIds); // Generation of numTextures image names 

	this->textureIds = new GLuint[numTextures];		// create and fill array with GL texture ids
	glGenTextures(numTextures, this->textureIds);	// Texture name generation 
											 
	std::map<std::string, GLuint*>::iterator itr = this->textureIdMap.begin(); // get iterator


	size_t pos = this->obj_path.find_last_of("\\/");
	std::string basepath = (std::string::npos == pos) ? "" : this->obj_path.substr(0, pos + 1);

	for (unsigned int i = 0; i < numTextures; i++) {
		std::string filename = (*itr).first;	// get filename
		(*itr).second = &this->textureIds[i];	// save texture id for filename in map
		itr++;									// next texture

		ilBindImage(imageIds[i]);				// Binding of DevIL image name
		std::string path = basepath + filename;	

		success = ilLoadImage((ILstring)path.c_str());

		if (!success) {
			std::cout << "Failed to read the " + path << std::endl;
			return TRUE;
			//return -3;
			//ERR(true, IL_LOAD_IMG_FAIL, -3);
		}

		// Convert every colour component into unsigned byte.If your image contains 
		// alpha channel you can replace IL_RGB with IL_RGBA
		ERR(!ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE), IL_CONVERT_IMG_FAIL, -4);
		
		glBindTexture(GL_TEXTURE_2D, this->textureIds[i]);	// Binding of texture name
		
		// redefine standard texture values
		// We will use linear interpolation for magnification filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// We will use linear interpolation for minifying filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
		// Texture specification
		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
			ilGetData());

		// we also want to be able to deal with odd texture dimensions
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	}

	ilDeleteImages(numTextures, imageIds);		// release memory used by image.

	// Cleanup
	delete[] imageIds;
	imageIds = NULL;

	return TRUE;
}

void Model3D::render() {
	recursive_render(this->scene, this->scene->mRootNode);
}

void Model3D::scale(double x, double y, double z) {
	this->ar_scale[0] = x;
	this->ar_scale[1] = y;
	this->ar_scale[2] = z;
}

//************************************************ PRIVATE FUNCTIONS ************************************************

void Model3D::Color4f(const aiColor4D *color)
{
	glColor4f(color->r, color->g, color->b, color->a);
}

void Model3D::set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

void Model3D::color4_to_float4(const aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

void Model3D::apply_material(const aiMaterial *mtl)
{
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	aiColor4D diffuse;
	aiColor4D specular;
	aiColor4D ambient;
	aiColor4D emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;	// changed: to unsigned

	int texIndex = 0;
	aiString texPath;	//contains filename of texture

	if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath))
	{
		//bind texture
		unsigned int texId = *textureIdMap[texPath.data];
		glBindTexture(GL_TEXTURE_2D, texId);
	}

	this->set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		this->color4_to_float4(&diffuse, c);

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	this->set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		this->color4_to_float4(&specular, c);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	this->set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		this->color4_to_float4(&ambient, c);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	this->set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		this->color4_to_float4(&emission, c);

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	max = 1;
	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
	if ((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		this->set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if (AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if ((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}

void Model3D::recursive_render(const struct aiScene *sc, const struct aiNode* nd) {
	unsigned int i, n, t;
	aiMatrix4x4 matrix1 = nd->mTransformation;

	aiMatrix4x4 matrix2;

	aiMatrix4x4::Scaling(aiVector3D(this->ar_scale[0], this->ar_scale[1], this->ar_scale[2]), matrix2);

	matrix1 = matrix1 * matrix2;

	// update transform
	matrix1.Transpose();
	glPushMatrix();

	glMultMatrixf((float*) &matrix1);

	// draw all meshes assigned to this node
	for (n = 0; n < nd->mNumMeshes; n++) {
		const struct aiMesh* mesh = this->scene->mMeshes[nd->mMeshes[n]];

		apply_material(sc->mMaterials[mesh->mMaterialIndex]);
		/*
		if (mesh->mNormals == NULL)
			glDisable(GL_LIGHTING);
		else
			glEnable(GL_LIGHTING);

		if (mesh->mColors[0] != NULL)
			glEnable(GL_COLOR_MATERIAL);
		else
			glDisable(GL_COLOR_MATERIAL);
		*/
		for (t = 0; t < mesh->mNumFaces; t++) {
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch (face->mNumIndices) {
				case 1:
					face_mode = GL_POINTS;
					break;
				case 2:
					face_mode = GL_LINES;
					break;
				case 3:
					face_mode = GL_TRIANGLES;
					break;
				default:
					face_mode = GL_POLYGON;
					break;
			}

			glBegin(face_mode);
				for (i = 0; i < face->mNumIndices; i++) {						// go through all vertices in face
					int vertexIndex = face->mIndices[i];						// get group index for current index

					if (mesh->mColors[0] != NULL)
						this->Color4f(&mesh->mColors[0][vertexIndex]);

					if (mesh->mNormals != NULL)

						if (mesh->HasTextureCoords(0))		//HasTextureCoords(texture_coordinates_set)
						{
							glTexCoord2f(mesh->mTextureCoords[0][vertexIndex].x, 1 - mesh->mTextureCoords[0][vertexIndex].y); //mTextureCoords[channel][vertex]
						}

					glNormal3fv(&mesh->mNormals[vertexIndex].x);
					glVertex3fv(&mesh->mVertices[vertexIndex].x);
				}
			glEnd();
		}
	}

	// draw all children
	for (n = 0; n < nd->mNumChildren; n++)
		this->recursive_render(sc, nd->mChildren[n]);

	glPopMatrix();
}
