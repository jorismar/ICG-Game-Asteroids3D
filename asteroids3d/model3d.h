#ifndef JB_MODEL3D_H
#define JB_MODEL3D_H

// Check OS
#ifdef WIN32
	#include <windows.h>
#endif

#include <map>
#include <fstream>
#include <GL/gl.h>
#include <IL/il.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

#include <vector>

#include "err_handler.h"
#include "utils.h"

class Model3D {
	private:
		// Model
		GLuint			 scene_list;
		const aiScene*	 object;
		Assimp::Importer importer;

		// Images / Texture
		std::map<std::string, GLuint*> textures;	// map image filenames to textureIds
		std::string		path;
		double			ar_scale[3];


		// Utils
		bool	hasBoundBox;
		BoundingBox bounding;

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
		bool loadModel(const std::string& path);

		/**
		 * \brief	Load the texture associated with the object Material
		 *
		 * \return	Returns a positive value if sucessfuly, and negative if not.
		 */
		bool loadTexture();

		/**
		 * \brief	Render the model.
		 */
		void render();

		/**
		* \brief Sets the value of the proportional scale.
		*
		* \param	factor	Scale factor.
		*/
		void scale(double factor);

		/**
		* \brief Sets the scale on the individual directions.
		*
		* \param	x	Scale value on the X axis.
		* \param	y	Scale value on the Y axis.
		* \param	z	Scale value on the Z axis.
		*/
		void scale(double x, double y, double z);

		/**
		 * \brief Generates the bounding box of loaded model
		 *
		 * \param	mode	Sets the bounding box range
		 */
		void genBoundingBox(unsigned int mode);

		/**
		* \brief	Gets the generated bounding box
		*
		* \return	Returns the generated bounding box.
		*/
		BoundingBox getBoundingBox();

		/**
		* \brief	Render the bounding box.
		*/
		bool hasBoundingBox();


};

#endif // !JB_MODEL3D_H
