#ifndef JB_MODEL3D_H
#define JB_MODEL3D_H

// Check OS
#include <Windows.h>
#include <map>
#include <fstream>
#include <GL/gl.h>
#include <IL/il.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

#include "err_handler.h"

class Model3D {
	private:
		// Model
		GLuint			 scene_list;
		const aiScene*	 scene;
		Assimp::Importer importer;

		// Images / Texture
		GLuint*			 textureIds;
		std::map<std::string, GLuint*> textureIdMap;	// map image filenames to textureIds
		std::string		obj_path;
		double			ar_scale[3];

		void Color4f(const aiColor4D *color);
		void set_float4(float f[4], float a, float b, float c, float d);
		void color4_to_float4(const aiColor4D *c, float f[4]);
		void apply_material(const aiMaterial *mtl);
		void recursive_render(const struct aiScene *sc, const struct aiNode* nd);

	public:
		/**
		* \brief	Constructor
		*/
		Model3D();

		/**
		* \brief	Destructor
		*/
		virtual ~Model3D();

		/**
		 * \brief	Import a 3D model for file.
		 *
		 * \param	path	The path of filename.
		 *
		 * \return	Returns true if the operation was successful. 
		 *			If an error occurs, false is returned and the 
		 *			error code will be updated.
		 */
		bool importFrmFile(const std::string& path);

		int loadTexture();

		void render();

		void scale(double x, double y, double z);
};

#endif // !JB_MODEL3D_H
