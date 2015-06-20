#ifndef ENGINE_MAPEDITOR_SCENEFREECAMERA_H
#define ENGINE_MAPEDITOR_SCENEFREECAMERA_H

#include <QWidget>
#include "UrchinCommon.h"
#include "Urchin3dEngine.h"

namespace urchin
{

	class SceneFreeCamera : public FreeCamera
	{
		public:
			SceneFreeCamera(float, float, float, const QWidget *);
			virtual ~SceneFreeCamera();

			void moveMouse(int, int);

		private:
			const QWidget *parentWidget;
	};

}

#endif
