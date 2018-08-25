#pragma once

class Drawable
{
public:
	Drawable();
    virtual void render() const = 0;
};
