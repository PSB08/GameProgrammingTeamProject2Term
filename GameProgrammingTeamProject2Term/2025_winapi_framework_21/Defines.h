#pragma once
#define DECLARE_SINGLE(className)	\
private:							\
	className() {}					\
public:								\
	static className* GetInst()		\
	{								\
		static className inst;		\
		return &inst;				\
	}		

#define GET_SINGLE(className) className::GetInst()
#define SAFE_DELETE(ptr) if(ptr !=nullptr){	delete ptr;	 ptr = nullptr; }	
#define fDT GET_SINGLE(TimeManager)->GetDT()

#define GET_KEYUP(key) GET_SINGLE(InputManager)->IsUp(key)
#define GET_KEYDOWN(key) GET_SINGLE(InputManager)->IsDown(key)
#define GET_KEY(key) GET_SINGLE(InputManager)->IsPress(key)
#define GET_KEYWIDTH(mainkey, modkey) GET_SINGLE(InputManager)->IsPressWith(mainkey, modkey)
#define GET_MOUSEPOS GET_SINGLE(InputManager)->GetMousePos()

// GDI «Ô∆€
#define RECT_RENDER(hdc, posx, posy, sizex, sizey) Rectangle(hdc, (int)(posx-sizex/2),(int)(posy-sizey/2),(int)(posx+sizex/2),(int)(posy+sizey/2))
#define ELLIPSE_RENDER(hdc, posx, posy, sizex, sizey) Ellipse(hdc, (int)(posx-sizex/2),(int)(posy-sizey/2),(int)(posx+sizex/2),(int)(posy+sizey/2))
#define RECT_MAKE(posx, posy, sizex, sizey) {posx-sizex/2,posy-sizey/2,posx+sizex/2,posy+sizey/2}
