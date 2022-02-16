#ifndef ORTHOGONAL_CAMERA_H
#define	ORTHOGONAL_CAMERA_H

#include "DrawSelectRectangleByMouse.h"
#include "MouseCamera.h"
namespace FATMING_CORE
{
	//most usage way is 2D...because y axis is nagtive againt Cartesian coordinate,so the Z has too invert too.
	//setTransform is not work pretty,because translation X,Y,Z has to multiply negative
	//and Draw Grid is quite tricky!
	class cOrthogonalCamera:public Frame
	{
		float					m_fProjectionMatrix[16];
		cMouseBehave			m_MouseBehave;
		//currently it only works for CameraUpdateByMouse.
		GET_SET_DEC(Vector2,m_CameraPos,GetCameraPos,SetCameraPos);
		//only work for CameraUpdateByMouse?
		GET_SET_DEC(float,m_fScale,GetScale,SetScale);
		GET_SET_DEC(bool,m_bDrawSelectFrame,IsDrawSelectFrame,SetDrawSelectFrame);
		//set left(x) up(y) right(z) down(w),last plane to setup orthogonal
		GET_SET_DEC(Vector4,m_vViewRect,GetViewRect,SetViewRect);
		cDrawSelectRectangleByMouse	m_DrawSelectRectangleByMouse;
		//m_MouseWorldPosition get by ConvertMousePositionToWorldPosition
		//so we could get latest worldposition without recalculate
		GET_SET_DEC(Vector2,m_MouseWorldPosition,GetMouseWorldPos,SetMouseWorldPos);
		//screen resolution,just a original data while rest call it could be set as well
		GET_SET_DEC(Vector2,m_vResolution,GetResolution,SetResolution);
		GET_SET_DEC(float,m_fGridWidth,GetGridWidth,SetGridWidth);
		GET_SET_DEC(float,m_fGridHeight,GetGridHeight,SetGridHeight);
		cMatrix44				m_matVP;//View Projection
	public:
		DEFINE_TYPE_INFO();
		cOrthogonalCamera(Vector2 e_vResolution = Vector2(1920.f,1080.f));
		cOrthogonalCamera(cOrthogonalCamera*e_pOrthogonalCamera);
		CLONE_MYSELF(cOrthogonalCamera);
		~cOrthogonalCamera(){ }
		Vector2					ConvertMousePositionToWorldPosition(Vector2 e_MousePosition,Vector2 e_ViewportSize);
		cMouseBehave*			GetMouseBehave(){ return &m_MouseBehave; }
		//acctualy viewport size,input game wanted resolution
		//it could be different because not whole mornintor support the solution wa wanted.
		//e_bYInvert is because the z is invert so Y is invert too,but in the 3D world we do not expect this...
		//ignore for worldtransform now because I am lazy to test it
		void					Render(bool e_bYInvert = false);
		//
		void    				MoveViewRect(Vector2 e_vDirection);
		//start point will have a offset if offset value is not 0
		void					DrawGrid(float e_fXOffset = 0.f,float e_fYOffset = 0.f,Vector4 e_vColor = Vector4(0.5f,1.f,0.f,1.f),float e_fLineWidth = 2.f);
		void					DrawSelectFrame();
		void					Reset();
		Vector2					GetScreenViewPortSize(){ return  Vector2(m_vViewRect.z-m_vViewRect.x,m_vViewRect.w-m_vViewRect.y); }
		Vector2					GetViewRectCenter(){ return GetScreenViewPortSize()/2+Vector2(m_vViewRect.x,m_vViewRect.y); }
		//ther os no matirx push and pop here,ensure the matrix has settled as well
		//this render only for 2D shader,or fixed pipeline,
		//if u are using another shader please setup VP matrix as well
		void					Render(Vector2 e_vCameraPos);
		//get view projection matrix.
		cMatrix44				GetVPMatrix(){ return m_matVP; }
		void					CameraUpdateByMouse(bool e_bLeftButton,bool e_bRightButton,int e_iWheelDelta,int e_iMouseX,int e_iMouseY,Vector2 e_vViewport);
		//for special axis control,only 2 axis is controlled.
		//only transform matrices
		//if u like to use mouse wheel event in the dot net,
		//make sure you have call Focus function in the mouse move envet to the panel,if u would like to use!!
		void					CameraUpdateByMouse(MyMath::AXIS e_Axis,bool e_bLeftButton,bool e_bRightButton,int e_iWheelDelta,int e_iMouseX,int e_iMouseY,Vector2 e_vViewport);
		float* 					GetProjectionMatrix(){ return m_fProjectionMatrix; }
		//convert expect view rect to real viewport's data
		//e_vRealViewPort should input GameApp's Viewport,but if cGameApp is not actived,ensure what are u doing
		bool					ViewportConvert(Vector4 e_vShowRect,Vector2 e_vRealViewPort,Vector4*e_pResult);
		bool					IsVisible(Vector4 e_vShowRect);
	};
}
#endif