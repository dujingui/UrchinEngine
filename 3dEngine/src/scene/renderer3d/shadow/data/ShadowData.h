#ifndef ENGINE_SHADOWDATA_H
#define ENGINE_SHADOWDATA_H

#include <vector>
#include <set>
#include "UrchinCommon.h"

#include "FrustumShadowData.h"
#include "scene/renderer3d/light/Light.h"

namespace urchin
{

	/**
	* Shadow execution data for a light
	*/
	class ShadowData
	{
		public:
			ShadowData(const Light *const, unsigned int);
			~ShadowData();

			void setFrameBufferObjectID(unsigned int);
			unsigned int getFrameBufferObjectID() const;

			void setDepthTextureID(unsigned int);
			unsigned int getDepthTextureID() const;
			void setShadowMapTextureID(unsigned int);
			unsigned int getShadowMapTextureID() const;

			void setLightViewMatrix(const Matrix4<float> &);
			const Matrix4<float> &getLightViewMatrix() const;

			unsigned int getNbFrustumShadowData() const;
			FrustumShadowData *getFrustumShadowData(unsigned int);
			const FrustumShadowData *getFrustumShadowData(unsigned int) const;

			std::set<Model *> retrieveModels() const;

		private:
			const Light *const light;

			unsigned int fboID; //frame buffer object ID containing shadow map(s)
			unsigned int depthTextureID; //depth texture ID
			unsigned int shadowMapTextureID; //shadow map texture ID (variance shadow map)

			Matrix4<float> lightViewMatrix;
			std::vector<FrustumShadowData *> frustumShadowData;
	};

}

#endif
