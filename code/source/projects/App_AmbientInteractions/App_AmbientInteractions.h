#pragma once
#include "framework/EliteInterfaces/EIApp.h"
class App_AmbientInteractions final : public IApp
{
public:

	App_AmbientInteractions();
	virtual ~App_AmbientInteractions();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(float deltaTime) const override;

private:

};

