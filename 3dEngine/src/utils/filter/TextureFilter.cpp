#include <GL/glew.h>
#include <stdexcept>

#include "TextureFilter.h"
#include "utils/shader/ShaderManager.h"
#include "utils/display/quad/QuadDisplayerBuilder.h"

namespace urchin
{

	TextureFilter::~TextureFilter()
	{
		ShaderManager::instance()->removeProgram(textureFilterShader);

		glDeleteFramebuffers(1, &fboID);
		glDeleteTextures(1, &textureID);
	}

	void TextureFilter::initialize()
	{
        if(isInitialized)
        {
            throw std::runtime_error("Texture filter is already initialized");
        }

		initializeDisplay();
		initializeTexture();

		isInitialized = true;
	}

	void TextureFilter::initializeDisplay()
	{
		std::locale::global(std::locale("C")); //for float

		texQuadDisplayer = std::make_unique<QuadDisplayerBuilder>()
				->build();

		std::map<std::string, std::string> shaderTokens;
		if(textureFormat==GL_RGB)
		{
			shaderTokens["OUTPUT_TYPE"] = "vec3";
			shaderTokens["SOURCE_TEX_COMPONENTS"] = "rgb";
		}else if(textureFormat==GL_RG)
		{
			shaderTokens["OUTPUT_TYPE"] = "vec2";
			shaderTokens["SOURCE_TEX_COMPONENTS"] = "rg";
		}else if(textureFormat==GL_RED)
		{
			shaderTokens["OUTPUT_TYPE"] = "float";
			shaderTokens["SOURCE_TEX_COMPONENTS"] = "r";
		}else
		{
			throw std::invalid_argument("Unsupported texture format for filter: " + std::to_string(textureFormat));
		}

		this->completeShaderTokens(shaderTokens);

		if(textureType==GL_TEXTURE_2D_ARRAY)
		{
            shaderTokens["MAX_VERTICES"] = std::to_string(3*textureNumberLayer);
            shaderTokens["NUMBER_LAYER"] = std::to_string(textureNumberLayer);

			textureFilterShader = ShaderManager::instance()->createProgram("textureFilter.vert", "textureFilter.geom", getShaderName()+"Array.frag", shaderTokens);
		}else if(textureType==GL_TEXTURE_2D)
		{
			textureFilterShader = ShaderManager::instance()->createProgram("textureFilter.vert", "", getShaderName()+".frag", shaderTokens);
		}else
		{
			throw std::invalid_argument("Unsupported texture type for filter: " + std::to_string(textureType));
		}

		ShaderManager::instance()->bind(textureFilterShader);
		int texLoc = glGetUniformLocation(textureFilterShader, "tex");
		glUniform1i(texLoc, GL_TEXTURE0-GL_TEXTURE0);
		layersToUpdateLoc = glGetUniformLocation(textureFilterShader, "layersToUpdate");
		initializeAdditionalUniforms(textureFilterShader);
	}

	void TextureFilter::initializeTexture()
	{
		glGenFramebuffers(1, &fboID);
		glBindFramebuffer(GL_FRAMEBUFFER, fboID);

		GLenum fboAttachments[1] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, fboAttachments);
		glReadBuffer(GL_NONE);

		glGenTextures(1, &textureID);
		glBindTexture(textureType, textureID);
		glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, textureAccessFilter);
		glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, textureAccessFilter);
		glTexParameterf(textureType, GL_TEXTURE_MAX_ANISOTROPY_EXT, textureAnisotropy);
		glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		if(textureType==GL_TEXTURE_2D_ARRAY)
		{
			glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, textureInternalFormat, textureWidth, textureHeight, textureNumberLayer, 0, textureFormat, GL_FLOAT, nullptr);
		}else if(textureType==GL_TEXTURE_2D)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, textureInternalFormat, textureWidth, textureHeight, 0, textureFormat, GL_FLOAT, nullptr);
		}else
		{
			throw std::invalid_argument("Unsupported texture type for filter: " + std::to_string(textureType));
		}
		glFramebufferTexture(GL_FRAMEBUFFER, fboAttachments[0], textureID, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void TextureFilter::initializeAdditionalUniforms(unsigned int)
	{
		//do nothing: to override
	}

	void TextureFilter::bindAdditionalTextures() const
	{
		//do nothing: to override
	}

	unsigned int TextureFilter::getFboId() const
	{
		return fboID;
	}

	unsigned int TextureFilter::getTextureID() const
	{
		return textureID;
	}

	unsigned int TextureFilter::getTextureWidth() const
	{
		return textureWidth;
	}

	unsigned int TextureFilter::getTextureHeight() const
	{
		return textureHeight;
	}

	unsigned int TextureFilter::getTextureFilterShader() const
	{
		return textureFilterShader;
	}

	std::string TextureFilter::toShaderVectorValues(std::vector<float> &vector) const
	{
		std::string vectorValuesStr;
		for(std::size_t i=0;i<vector.size(); ++i)
		{
			vectorValuesStr += std::to_string(vector[i]);
			if(i!=vector.size()-1)
			{
				vectorValuesStr += ", ";
			}
		}

		return vectorValuesStr;
	}

	/**
	 * @param layersToUpdate Specify the layers which must be affected by the filter (only for GL_TEXTURE_2D_ARRAY).
	 * Lowest bit represent the first layer, the second lowest bit represent the second layer, etc.
	 */
	void TextureFilter::applyOn(unsigned int sourceTextureId, unsigned int layersToUpdate) const
	{
        if(!isInitialized)
        {
            throw std::runtime_error("Texture filter must be initialized before apply.");
        }

		ShaderManager::instance()->bind(textureFilterShader);

		glViewport(0, 0, textureWidth, textureHeight);

		glBindFramebuffer(GL_FRAMEBUFFER, fboID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(textureType, sourceTextureId);
		bindAdditionalTextures();

		if(textureType==GL_TEXTURE_2D_ARRAY)
		{
			glUniform1ui(layersToUpdateLoc, layersToUpdate);
		}

		texQuadDisplayer->display();

		glBindTexture(textureType, 0);
	}
}
