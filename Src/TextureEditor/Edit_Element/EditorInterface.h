#pragma once

class	cFUSynchronized;

public ref class	cEditorInterface abstract
{
public:
	cEditorInterface(){}
	~cEditorInterface(){ Destroy(); }
	virtual	void	Update(float e_fElpaseTime) = 0;
	virtual	void	Render() = 0;
	virtual	void	Destroy() = 0;
};