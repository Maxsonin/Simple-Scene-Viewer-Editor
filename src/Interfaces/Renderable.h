#pragma once

class Renderable
{
	virtual void Render() = 0;
	virtual void Update(float deltaTime) = 0;
};
