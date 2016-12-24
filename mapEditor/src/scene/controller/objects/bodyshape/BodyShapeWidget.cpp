#include <stdexcept>

#include "BodyShapeWidget.h"

namespace urchin
{

	BodyShapeWidget::BodyShapeWidget(const SceneObject *sceneObject) :
			disableShapeEvent(false),
			sceneObject(sceneObject)
	{
		setContentsMargins(0, 0, 0, 0);

		mainLayout = new QGridLayout(this);
		mainLayout->setAlignment(Qt::AlignmentFlag::AlignLeft | Qt::AlignmentFlag::AlignTop);
		mainLayout->setContentsMargins(0, 0, 0, 0);
	}

	BodyShapeWidget::~BodyShapeWidget()
	{

	}

	const SceneObject *BodyShapeWidget::getSceneObject() const
	{
		return sceneObject;
	}

	std::shared_ptr<const CollisionShape3D> BodyShapeWidget::retrieveShape()
	{
		if(shape.get()==nullptr)
		{
			shape = createBodyShape();
		}
		return shape;
	}

	void BodyShapeWidget::setupShapePropertiesFrom(std::shared_ptr<const CollisionShape3D> shape)
	{
		disableShapeEvent = true;

		doSetupShapePropertiesFrom(shape);

		disableShapeEvent = false;
	}

	void BodyShapeWidget::updateBodyShape()
	{
		if(!disableShapeEvent)
		{
			shape = createBodyShape();

			emit bodyShapeChange(shape);
		}
	}
}
