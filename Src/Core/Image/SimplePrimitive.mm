#include "../stdafx.h"

#include "SimplePrimitive.h"
#include "BaseImage.h"
#include "../GLSL/Shader.h"

namespace GLRender
{
	//========comment start======================
	//set glOrtho as glOrtho(vPort[0], vPort[0]+vPort[2], vPort[1]+vPort[3],vPort[1], -1, 1);for normal windows coordinate
	//(0,top) _____
	//		  |	   |
	//		  |	   |
	//		  |____|(right,0)
	//
	//
	//set glOrthoas glOrtho(vPort[0], vPort[0]+vPort[2], vPort[1], vPort[1]+vPort[3], -1, 1);for
	//
	//
	//==============================
	//GLenum	g_enumforCullFace = 0;
	//glDisable(GL_CULL_FACE);
	//basicly just treat as left hand coordinate(forward Z is postive backward is nagtive)
	void glEnable2D(float e_fResolutionWidth,float e_fResolutionHeight,float*e_pfMatrix,bool e_bInvertYAxis)
	{
	   //int vPort[4];
	   //glGetIntegerv(GL_CULL_FACE,&g_enumforCullFace);
	   //glIsEnabled(GL_CULL_FACE);
		//depend on how do y loading the image
	   //if the pixel data is lefr to right or right to left it will effect the UV
	   //glOrtho(vPort[0], vPort[0]+vPort[2], vPort[1], vPort[1]+vPort[3], -1, 1);
		//=====================
		//vertices order
		//       1 ____ 2
		//		  |	   |
		//		  |	   |
		//		 0|____|3
		//=====================
		//texture coordinate
		//(0,1)   _____(1,1)
		//		  |	   |
		//		  |	   |
		//(0,0)	  |____|(1,0)
		//=====================
		//vertices order
		//       0 ____ 3
		//		  |	   |
		//		  |	   |
		//		 1|____|2
		//=====================
		//texture coordinate
		//(0,0)   _____(1,0)
		//		  |	   |
		//		  |	   |
		//(0,1)	  |____|(1,1)
		if( e_bInvertYAxis )
			e_fResolutionHeight = -e_fResolutionHeight;
		cMatrix44	l_matProjection;
		glhOrthof2((float*)l_matProjection.m, 0, e_fResolutionWidth, e_fResolutionHeight, 0, -10000, 10000);
		FATMING_CORE::SetupShaderViewProjectionMatrix(l_matProjection);
		if(e_pfMatrix)
		{
			memcpy(e_pfMatrix,l_matProjection,sizeof(float)*16);
		}
	   glDisable(GL_DEPTH_TEST);
	   glDisable(GL_CULL_FACE);
	   //glDepthMask(GL_FALSE);
	}
	//===========================================
	//end 2d apply
	//===========================================
	void glDisable2D()
	{
	   FATMING_CORE::DisableShaderProgram();
	}
	//here is a sample to draw quad by triangles
	//Vector3	l_v1 = Vector3(100,0,0);
	//Vector3	l_v2 = Vector3(100,100,0);
	//Vector3	l_v3 = Vector3(200,100,0);
	//Vector3	l_v4 = Vector3(200,0,0);

	//glBegin(GL_TRIANGLES);//013,312
	//glTexCoord2f(0,0);	glVertex3fv((float*)&l_v1 );
	//glTexCoord2f(0,1);	glVertex3fv((float*)&l_v2 );
	//glTexCoord2f(1,0);	glVertex3fv((float*)&l_v4 );
	//glTexCoord2f(1,0);	glVertex3fv((float*)&l_v4 );
	//glTexCoord2f(0,1);	glVertex3fv((float*)&l_v2 );
	//glTexCoord2f(1,1);	glVertex3fv((float*)&l_v3 );
	//glEnd()
	//below is how to draw quad by triangle strip
	//glBegin(GL_TRIANGLE_STRIP);//0132
	//glTexCoord2f(0,0);	glVertex3fv((float*)&l_v1 );
	//glTexCoord2f(1,0);	glVertex3fv((float*)&l_v4 );
	//glTexCoord2f(0,1);	glVertex3fv((float*)&l_v2 );
	//glTexCoord2f(1,1);	glVertex3fv((float*)&l_v3 );	
	//glEnd();

	//tricky rotation method
	//how to rotation?
	//for rotation reason we have to substract Width/2 and Hight/2
	//and translate to destination+half of width and half height


	void    GetDrawQuadFVFDataByTriangleStrip(GLint e_iWidth,GLint e_iHeight,float *e_pfTexCoordinate,Vector4 e_vColor,float*e_pfVertices,float*e_pfUV,float*e_pColor,cMatrix44*e_pmat)
	{
		AssignUVDataToTriangleStrip(e_pfTexCoordinate,e_pfUV,false);
		for( int i=0;i<4;++i )
			memcpy(&e_pColor[i*4],&e_vColor,sizeof(Vector4));
		float	l_fHalfWidth = e_iWidth/2.f;
		float	l_fHalfHeight = e_iHeight/2.f;
		Vector3*l_pvPos = (Vector3*)e_pfVertices;
		l_pvPos[0] = Vector3(-l_fHalfWidth,-l_fHalfHeight,0.f);
		l_pvPos[1] = Vector3(l_fHalfWidth,-l_fHalfHeight,0.f);
		l_pvPos[2] = Vector3(-l_fHalfWidth,l_fHalfHeight,0.f);
		l_pvPos[3] = Vector3(l_fHalfWidth,l_fHalfHeight,0.f);
		if( e_pmat )
		{
			for( int i=0;i<4;++i )
			{
				l_pvPos[i] = e_pmat->TransformCoordinate(l_pvPos[i]);
			}
		}
	}

	void    GetDrawQuadFVFDataByTwoTriangles  (GLint e_iWidth,GLint e_iHeight,float *e_pfTexCoordinate,Vector4 e_vColor,float*e_pfVertices,float*e_pfUV,float*e_pColor,cMatrix44*e_pmat)
	{
		AssignUVDataTo2Triangles(e_pfTexCoordinate,e_pfUV,false);
		for( int i=0;i<6;++i )
			memcpy(&e_pColor[i*4],&e_vColor,sizeof(Vector4));
		float	l_fHalfWidth = e_iWidth/2.f;
		float	l_fHalfHeight = e_iHeight/2.f;
		Vector3	l_vPos[4];
		l_vPos[0] = Vector3(-l_fHalfWidth,-l_fHalfHeight,0.f);
		l_vPos[1] = Vector3(l_fHalfWidth,-l_fHalfHeight,0.f);
		l_vPos[2] = Vector3(-l_fHalfWidth,l_fHalfHeight,0.f);
		l_vPos[3] = Vector3(l_fHalfWidth,l_fHalfHeight,0.f);
		if( e_pmat )
		{
			cMatrix44	l_mat = *e_pmat;
			l_mat.m[3][0] += l_fHalfWidth;
			l_mat.m[3][1] += l_fHalfHeight;
			for( int i=0;i<4;++i )
			{
				l_vPos[i] = l_mat.TransformCoordinate(l_vPos[i]);
			}
		}
		//left down
		e_pfVertices[0] = l_vPos[2].x;
		e_pfVertices[1] = l_vPos[2].y;
		e_pfVertices[2] = l_vPos[2].z;
		//right down
		e_pfVertices[3] = l_vPos[3].x;
		e_pfVertices[4] = l_vPos[3].y;
		e_pfVertices[5] = l_vPos[3].z;
		//left upper
		e_pfVertices[6] = l_vPos[0].x;
		e_pfVertices[7] = l_vPos[0].y;
		e_pfVertices[8] = l_vPos[0].z;
		//left upper
		e_pfVertices[9] = l_vPos[0].x;
		e_pfVertices[10] = l_vPos[0].y;
		e_pfVertices[11] = l_vPos[0].z;
		//right down
		e_pfVertices[12] = l_vPos[3].x;
		e_pfVertices[13] = l_vPos[3].y;
		e_pfVertices[14] = l_vPos[3].z;
		//right upper
		e_pfVertices[15] = l_vPos[1].x;
		e_pfVertices[16] = l_vPos[1].y;
		e_pfVertices[17] = l_vPos[1].z;
	}

	void	DrawQuadWithTextureAndColorAndCoordinate(GLfloat e_iX,GLfloat e_iY,GLfloat e_fDepth,GLint e_iWidth,GLint e_iHeight,Vector4 e_vColor,float *e_pfTexCoordinate,float e_fRotationAngle)
	{
		cBaseShader*l_p2DShader = GetCurrentShader();
		UseShaderProgram(DEFAULT_SHADER);
		e_iWidth/=2;
		e_iHeight/=2;
		g_mat2DTransform = cMatrix44::TranslationMatrix(Vector3(e_iX+e_iWidth,e_iY+e_iHeight, 0.f));
		if(e_fRotationAngle!=0.f)
			g_mat2DTransform *= cMatrix44::ZAxisRotationMatrix(D3DXToRadian(e_fRotationAngle));
		FATMING_CORE::SetupShaderWorldMatrix(g_mat2DTransform);
		ASSIGN_2D_UV(e_pfTexCoordinate);
		ASSIGN_2D_VerticesBySize(e_iWidth,e_iHeight,e_fDepth);
		ASSIGN_2D_COLOR(e_vColor);
		MY_GLDRAW_ARRAYS(GL_TRIANGLE_STRIP, 0, 4);
		UseShaderProgram(l_p2DShader);
	}


	void	DrawMirrorQuadWithTextureAndColorAndCoordinate(GLfloat e_iX,GLfloat e_iY,GLfloat e_fDepth,GLint e_iWidth,GLint e_iHeight,Vector4 e_vColor,float *e_pfTexCoordinate,float e_fRotationAngle)
	{
		cBaseShader*l_p2DShader = GetCurrentShader();
		UseShaderProgram(DEFAULT_SHADER);
		e_iWidth/=2;
		e_iHeight/=2;
		g_mat2DTransform = cMatrix44::TranslationMatrix(Vector3(e_iX+e_iWidth,e_iY+e_iHeight, 0.f));
		if(e_fRotationAngle!=0.f)
			g_mat2DTransform *= cMatrix44::ZAxisRotationMatrix(D3DXToRadian(e_fRotationAngle));
		FATMING_CORE::SetupShaderWorldMatrix(g_mat2DTransform);
		ASSIGN_2D_MIRROR_UV(e_pfTexCoordinate);
		ASSIGN_2D_VerticesBySize(e_iWidth,e_iHeight,e_fDepth);
		ASSIGN_2D_COLOR(e_vColor);
		MY_GLDRAW_ARRAYS(GL_TRIANGLE_STRIP, 0, 4);
		UseShaderProgram(l_p2DShader);
	}

	
	void    DrawQuadWithMatrix(float*e_pfVertices,float*e_pfTextureUV,Vector4 e_vColor,float*e_pfMatrix)
	{
		cBaseShader*l_p2DShader = GetCurrentShader();
		UseShaderProgram(DEFAULT_SHADER);
	   // draw with client side arrays (in real apps you should use cached VBOs which is much better for performance)
		FATMING_CORE::SetupShaderWorldMatrix(e_pfMatrix);
		ASSIGN_2D_COLOR(e_vColor);
		myGlVertexPointer(2,e_pfVertices);
		myGlUVPointer(2,e_pfTextureUV);
		MY_GLDRAW_ARRAYS(GL_TRIANGLE_STRIP, 0, 4);
		UseShaderProgram(l_p2DShader);
	}
	void	Render3DArrow(Vector3 P,Vector3 D,Vector4 e_vColor,float e_fWidth)
	{
		cBaseShader*l_p2DShader = GetCurrentShader();
		UseShaderProgram(NO_TEXTURE_SHADER);
		float angle = atan2(D.y, D.x);
		float	l_fLength = D.Length();
		Vector3	l_vD10Percent = D*0.1f;
		float	l_fAllVertices[] = { 0.f,0.f,0.f,
									 D.x,D.y,D.z,

									 D.x,D.y,D.z,
									 D.x-l_vD10Percent.x,D.y-l_vD10Percent.y,D.z-l_vD10Percent.z,

									 D.x,D.y,D.z,
									 D.x-l_vD10Percent.x,D.y-l_vD10Percent.y,D.z+l_vD10Percent.z,

									 D.x,D.y,D.z,
									 D.x-l_vD10Percent.x,D.y+l_vD10Percent.y,D.z-l_vD10Percent.z,

									 D.x,D.y,D.z,
									 D.x-l_vD10Percent.x,D.y+l_vD10Percent.y,D.z+l_vD10Percent.z,


									 };
		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(3,l_fAllVertices);
		FATMING_CORE::SetupShaderWorldMatrix(cMatrix44::Identity);
		MY_GLDRAW_ARRAYS(GL_LINES, 0, 10);
		UseShaderProgram(l_p2DShader);
	}
	void RenderArrow(Vector3 P,Vector3 D, float radius,Vector4 e_vColor)
	{
		cBaseShader*l_p2DShader = GetCurrentShader();
		UseShaderProgram(NO_TEXTURE_SHADER);
		glLineWidth(radius);
		float angle = atan2(D.y, D.x);
		float	l_fLength = D.Length();
		g_mat2DTransform = cMatrix44::TranslationMatrix(Vector3(P.x, P.y, 0.0f));
		g_mat2DTransform *= cMatrix44::ScaleMatrix(Vector3(l_fLength,l_fLength, 1.0f));
		g_mat2DTransform *= cMatrix44::ZAxisRotationMatrix(D3DXToRadian(angle));
		float	l_fAllVertices[] = { 0.f,0.f,
									 1.f,0.f,
									 1.f,0.f,
									 0.9f,-0.05f,
									 1.f,0.f,
									 0.9f,0.05f
									 };
		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(2,l_fAllVertices);
		FATMING_CORE::SetupShaderWorldMatrix(g_mat2DTransform);
		MY_GLDRAW_ARRAYS(GL_LINES, 0, 6);
		UseShaderProgram(l_p2DShader);
	}

	void	RenderPoint(Vector2	e_vPos,float e_fSize,Vector4 e_vColor)
	{
		cBaseShader*l_p2DShader = GetCurrentShader();
		UseShaderProgram(NO_TEXTURE_SHADER);
		glPointSize(e_fSize);
		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(2,e_vPos);
		FATMING_CORE::SetupShaderWorldMatrix(cMatrix44::Identity);
		MY_GLDRAW_ARRAYS(GL_POINTS, 0, 1);
		UseShaderProgram(l_p2DShader);
	}

	void	RenderPoint(Vector3	e_vPos,float e_fSize,Vector4 e_vColor)
	{
		cBaseShader*l_p2DShader = GetCurrentShader();
		UseShaderProgram(NO_TEXTURE_SHADER);
		glPointSize(e_fSize);
		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(3,e_vPos);
		FATMING_CORE::SetupShaderWorldMatrix(cMatrix44::Identity);
		MY_GLDRAW_ARRAYS(GL_POINTS, 0, 1);
		UseShaderProgram(l_p2DShader);
	}
	void	RenderPoints(Vector3*e_pvPos,int e_iNumPoints,float e_fSize,Vector4 e_vColor,float*e_pfMatrix)
	{
		cBaseShader*l_p2DShader = GetCurrentShader();
		UseShaderProgram(NO_TEXTURE_SHADER);
		glPointSize(e_fSize);
		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(3,e_pvPos);
		FATMING_CORE::SetupShaderWorldMatrix(e_pfMatrix);
		MY_GLDRAW_ARRAYS(GL_POINTS, 0, e_iNumPoints);
		UseShaderProgram(l_p2DShader);
		
	}

	void	RenderPoints(Vector2*e_pvPos,int e_iNumPoints,float e_fSize,Vector4 e_vColor,float*e_pfMatrix)
	{
		cBaseShader*l_p2DShader = GetCurrentShader();
		UseShaderProgram(NO_TEXTURE_SHADER);
		glPointSize(e_fSize);
		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(2,e_pvPos);
		FATMING_CORE::SetupShaderWorldMatrix(e_pfMatrix);
		MY_GLDRAW_ARRAYS(GL_POINTS, 0, e_iNumPoints);
		UseShaderProgram(l_p2DShader);
	}

	//e_iNum as many as line points
	void	RenderLine(float*e_pPosition,int e_iNumPoints,Vector4 e_vColor,int e_iStride,float*e_pfMatrix,float e_fLineSize)
	{
		if( e_iNumPoints<2 )
			return;
		cBaseShader*l_p2DShader = GetCurrentShader();
		UseShaderProgram(NO_TEXTURE_SHADER);
		glLineWidth(e_fLineSize);
		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(e_iStride,e_pPosition);
		FATMING_CORE::SetupShaderWorldMatrix(e_pfMatrix);
		assert((e_iStride == 2 || e_iStride == 3)&&"draw line stride must be 2 or 3");
		MY_GLDRAW_ARRAYS(GL_LINES, 0, e_iNumPoints);
		UseShaderProgram(l_p2DShader);
	}
	void	RenderLine(std::vector<Vector2>* e_pPositionPoint,Vector4 e_vColor,float*e_pfMatrix,bool e_bRenderPoints)
	{
		if( e_pPositionPoint->size() < 2 )
			return;
		//draw line
		std::vector<Vector2>	l_CurvePointVector;
		int	l_iNumPoint = ((int)e_pPositionPoint->size()-1)*2;
		l_CurvePointVector.resize(l_iNumPoint);
		for( int i=0;i<l_iNumPoint/2;++i )
		{
			l_CurvePointVector[i*2] = (*e_pPositionPoint)[i];
			l_CurvePointVector[i*2+1] = (*e_pPositionPoint)[i+1];
		}
		RenderLine((float*)&l_CurvePointVector[0],l_iNumPoint,e_vColor,2,e_pfMatrix);
		if( e_bRenderPoints )
			RenderPoints(&(*e_pPositionPoint)[0],e_pPositionPoint->size(),15,e_vColor,e_pfMatrix);
	}
	void	RenderLine(std::vector<Vector3>* e_pPositionPoint,Vector4 e_vColor,float*e_pfMatrix,bool e_bRenderPoints)
	{
		if( e_pPositionPoint->size() < 2 )
			return;
		//draw line
		std::vector<Vector3>	l_CurvePointVector;
		int	l_iNumPoint = ((int)e_pPositionPoint->size()-1)*2;
		l_CurvePointVector.resize(l_iNumPoint);
		for( int i=0;i<l_iNumPoint/2;++i )
		{
			l_CurvePointVector[i*2] = (*e_pPositionPoint)[i];
			l_CurvePointVector[i*2+1] = (*e_pPositionPoint)[i+1];
		}
		RenderLine((float*)&l_CurvePointVector[0],l_iNumPoint,e_vColor,3,e_pfMatrix);
		if( e_bRenderPoints )
			RenderPoints(&(*e_pPositionPoint)[0],e_pPositionPoint->size(),15,e_vColor,e_pfMatrix);
	}

	void	RenderCircle(cMatrix44 e_mat,float e_fRadius,Vector4 e_vColor,bool e_bDrawPoint,float e_fDensityAngle)
	{
		cBaseShader*l_p2DShader = GetCurrentShader();
		UseShaderProgram(NO_TEXTURE_SHADER);
		if( e_fDensityAngle<1.f )
			e_fDensityAngle = 1.f;
		int	l_iNum = (int)(360.f/e_fDensityAngle);
		float	l_fStepAngle = 1/e_fDensityAngle;
		Vector2	l_vFirestPos = Vector2(sin(l_fStepAngle)*e_fRadius,cos(l_fStepAngle)*e_fRadius);
		g_mat2DTransform = e_mat;
		for( int i=0;i<l_iNum;++i )
		{
			float	l_fX = sin(l_fStepAngle*i)*e_fRadius;
			float	l_fY = cos(l_fStepAngle*i)*e_fRadius;
			Vector2	l_vPos = Vector2(l_fX,l_fY);
			g_fMPDIOptmizeRenderColor[i*4] = l_vFirestPos.x;
			g_fMPDIOptmizeRenderColor[i*4+1] = l_vFirestPos.y;
			g_fMPDIOptmizeRenderColor[i*4+2] = l_vPos.x;
			g_fMPDIOptmizeRenderColor[i*4+3] = l_vPos.y;
			l_vFirestPos = l_vPos;
		}
		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(2,g_fMPDIOptmizeRenderColor);
		FATMING_CORE::SetupShaderWorldMatrix(g_mat2DTransform);
		MY_GLDRAW_ARRAYS(GL_LINES, 0, l_iNum*2);
		if( e_bDrawPoint )
		{
			for( int i=0;i<l_iNum;++i )
			{
				g_fMPDIOptmizeRenderColor[i*2] = sin(l_fStepAngle*i)*e_fRadius;
				g_fMPDIOptmizeRenderColor[i*2+1] = cos(l_fStepAngle*i)*e_fRadius;
			}
			myGlVertexPointer(2,g_fMPDIOptmizeRenderColor);
			MY_GLDRAW_ARRAYS(GL_LINES, 0, l_iNum);
		}
		UseShaderProgram(l_p2DShader);	
	}

	void	RenderCircle(Vector2 e_vPos,float e_fRadius,Vector4 e_vColor,bool e_bDrawPoint,float e_fDensityAngle)
	{
		cBaseShader*l_p2DShader = GetCurrentShader();
		UseShaderProgram(NO_TEXTURE_SHADER);
		if( e_fDensityAngle<1.f )
			e_fDensityAngle = 1.f;
		int	l_iNum = (int)(360.f/e_fDensityAngle);
		float	l_fStepAngle = 1/e_fDensityAngle;
		Vector2	l_vFirestPos = Vector2(sin(l_fStepAngle)*e_fRadius,cos(l_fStepAngle)*e_fRadius);
		g_mat2DTransform = cMatrix44::TranslationMatrix(Vector3(e_vPos.x,e_vPos.y, 0.0f));
		for( int i=0;i<l_iNum;++i )
		{
			float	l_fX = sin(l_fStepAngle*i)*e_fRadius;
			float	l_fY = cos(l_fStepAngle*i)*e_fRadius;
			Vector2	l_vPos = Vector2(l_fX,l_fY);
			g_fMPDIOptmizeRenderColor[i*4] = l_vFirestPos.x;
			g_fMPDIOptmizeRenderColor[i*4+1] = l_vFirestPos.y;
			g_fMPDIOptmizeRenderColor[i*4+2] = l_vPos.x;
			g_fMPDIOptmizeRenderColor[i*4+3] = l_vPos.y;
			l_vFirestPos = l_vPos;
		}
		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(2,g_fMPDIOptmizeRenderColor);
		FATMING_CORE::SetupShaderWorldMatrix(g_mat2DTransform);
		MY_GLDRAW_ARRAYS(GL_LINES, 0, l_iNum*2);
		if( e_bDrawPoint )
		{
			for( int i=0;i<l_iNum;++i )
			{
				g_fMPDIOptmizeRenderColor[i*2] = sin(l_fStepAngle*i)*e_fRadius;
				g_fMPDIOptmizeRenderColor[i*2+1] = cos(l_fStepAngle*i)*e_fRadius;
			}
			myGlVertexPointer(2,g_fMPDIOptmizeRenderColor);
			MY_GLDRAW_ARRAYS(GL_LINES, 0, l_iNum);
		}
		UseShaderProgram(l_p2DShader);
	}


	void	RenderRectangle(Vector2 e_Pos,float e_fWidth,float e_fHeight,Vector4 e_vColor,float e_fAngle,float e_fLineWidth)
	{
		cBaseShader*l_p2DShader = GetCurrentShader();
		UseShaderProgram(NO_TEXTURE_SHADER);
		glLineWidth(e_fLineWidth);
		float	l_fWidth = e_fWidth/2.f;
		float	l_fHeight = e_fHeight/2.f;
		g_mat2DTransform = cMatrix44::TranslationMatrix(Vector3(e_Pos.x+l_fWidth,e_Pos.y+l_fHeight, 0.f));
		if( e_fAngle !=0.f )
			g_mat2DTransform*=cMatrix44::ZAxisRotationMatrix(D3DXToRadian(e_fAngle));
		float	l_fAllVertices[16] = {-l_fWidth,-l_fHeight,
									  l_fWidth,-l_fHeight,
									  -l_fWidth,-l_fHeight,
									  -l_fWidth,l_fHeight,
									  -l_fWidth,l_fHeight,
									  l_fWidth,l_fHeight,
									  l_fWidth,l_fHeight,
									  l_fWidth,-l_fHeight};
		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(2,l_fAllVertices);
		FATMING_CORE::SetupShaderWorldMatrix(g_mat2DTransform);
		MY_GLDRAW_ARRAYS(GL_LINES, 0, 8);
		UseShaderProgram(l_p2DShader);
	}
	void	RenderRectangle(Vector3 e_Pos,float e_fWidth,float e_fHeight,Vector4 e_vColor,Vector3 e_vAngle,float e_fLineWidth)
	{
		RenderRectangle(Vector2(e_Pos.x,e_Pos.y),e_fWidth,e_fHeight,e_vColor,e_vAngle,e_fLineWidth);
	}
	void	RenderRectangle(Vector2 e_Pos,float e_fWidth,float e_fHeight,Vector4 e_vColor,Vector3 e_vAngle,float e_fLineWidth)
	{
		cBaseShader*l_p2DShader = GetCurrentShader();
		UseShaderProgram(NO_TEXTURE_SHADER);
		glLineWidth(e_fLineWidth);
		float	l_fWidth = e_fWidth/2.f;
		float	l_fHeight = e_fHeight/2.f;
		g_mat2DTransform = cMatrix44::TranslationMatrix(Vector3(e_Pos.x+l_fWidth,e_Pos.y+l_fHeight, 0.f));
		g_mat2DTransform *= cMatrix44::RotationMatrix(e_vAngle);
		float	l_fAllVertices[16] = {-l_fWidth,-l_fHeight,
									  l_fWidth,-l_fHeight,
									  -l_fWidth,-l_fHeight,
									  -l_fWidth,l_fHeight,
									  -l_fWidth,l_fHeight,
									  l_fWidth,l_fHeight,
									  l_fWidth,l_fHeight,
									  l_fWidth,-l_fHeight};

		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(2,l_fAllVertices);
		FATMING_CORE::SetupShaderWorldMatrix(g_mat2DTransform);
		MY_GLDRAW_ARRAYS(GL_LINES, 0, 8);
		UseShaderProgram(l_p2DShader);
			
	}

	void	RenderRectangle(float e_fWidth,float e_fHeight,cMatrix44 e_Mat,Vector4 e_vColor,float e_fLineWidth)
	{
		cBaseShader*l_p2DShader = GetCurrentShader();
		UseShaderProgram(NO_TEXTURE_SHADER);
		glLineWidth(e_fLineWidth);
		float	l_fWidth = e_fWidth/2.f;
		float	l_fHeight = e_fHeight/2.f;
		float	l_fAllVertices[16] = {-l_fWidth,-l_fHeight,
									  l_fWidth,-l_fHeight,
									  -l_fWidth,-l_fHeight,
									  -l_fWidth,l_fHeight,
									  -l_fWidth,l_fHeight,
									  l_fWidth,l_fHeight,
									  l_fWidth,l_fHeight,
									  l_fWidth,-l_fHeight};

		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(2,l_fAllVertices);
		FATMING_CORE::SetupShaderWorldMatrix(e_Mat);
		MY_GLDRAW_ARRAYS(GL_LINES, 0, 8);
		UseShaderProgram(l_p2DShader);
			
	}

	void	RenderFilledRectangle(Vector2 e_Pos,float e_fWidth,float e_fHeight,Vector4 e_vColor,float e_fAngle)
	{
		cBaseShader*l_p2DShader = GetCurrentShader();
		UseShaderProgram(NO_TEXTURE_SHADER);
		e_fWidth/=2;
		e_fHeight/=2;
		g_mat2DTransform = cMatrix44::TranslationMatrix(Vector3(e_Pos.x+e_fWidth,e_Pos.y+e_fHeight, 0.f));
		if(e_fAngle!=0.f)
			g_mat2DTransform *= cMatrix44::ZAxisRotationMatrix(D3DXToRadian(e_fAngle));
		FATMING_CORE::SetupShaderWorldMatrix(g_mat2DTransform);
		ASSIGN_2D_VerticesBySize(e_fWidth,e_fHeight,0.f);
		FATMING_CORE::SetupShaderColor(e_vColor);
		MY_GLDRAW_ARRAYS(GL_TRIANGLE_STRIP, 0, 4);
		UseShaderProgram(l_p2DShader);
	}

	void	RenderRectangle(POINT e_Pos,int e_iWidth,int e_iHeight,Vector4 e_vColor,float e_fAngle,float e_fLineWidth)
	{
		cBaseShader*l_p2DShader = GetCurrentShader();
		UseShaderProgram(NO_TEXTURE_SHADER);
		glLineWidth(e_fLineWidth);
		float	l_fWidth = e_iWidth/2.f;
		float	l_fHeight = e_iHeight/2.f;
		g_mat2DTransform = cMatrix44::TranslationMatrix(Vector3(e_Pos.x+l_fWidth,e_Pos.y+l_fHeight, 0.f));
		if( e_fAngle !=0.f )
			g_mat2DTransform *= cMatrix44::ZAxisRotationMatrix(e_fAngle);
		float	l_fAllVertices[16] = {-l_fWidth,-l_fHeight,
									  l_fWidth,-l_fHeight,
									  -l_fWidth,-l_fHeight,
									  -l_fWidth,l_fHeight,
									  -l_fWidth,l_fHeight,
									  l_fWidth,l_fHeight,
									  l_fWidth,l_fHeight,
									  l_fWidth,-l_fHeight};

		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(2,l_fAllVertices);
		FATMING_CORE::SetupShaderWorldMatrix(g_mat2DTransform);
		MY_GLDRAW_ARRAYS(GL_LINES, 0, 8);
		UseShaderProgram(l_p2DShader);
	}

	void	DrawPlane(cMatrix44 e_mat,float	e_fWidth,float	e_fHeight,float e_fGridWidthDistance,float e_fGridHeightDistance,float e_fLineWidth,Vector4 e_vColor)
	{
		assert(e_fWidth>e_fGridWidthDistance&&e_fHeight>e_fGridHeightDistance);
		int	l_iRow = (int)(e_fWidth/e_fGridWidthDistance)+1;
		int	l_iColumn = (int)(e_fHeight/e_fGridHeightDistance)+1;
		int	l_iTotalCount = (l_iRow+l_iColumn)*2;
		//if( l_iTotalCount>200 )//-.- to big lah.
		//	return;
		cBaseShader*l_p2DShader = GetCurrentShader();
		UseShaderProgram(NO_TEXTURE_SHADER);
		glLineWidth(e_fLineWidth);
		Vector2	l_vLeftUp(-e_fWidth/2,-e_fHeight/2.f);
		Vector2	l_vRightDown(e_fWidth/2,e_fHeight/2.f);
		Vector2	*l_pvVerticesForDrawPlane = (Vector2*)g_fMPDIOptmizeRenderColor;
		//for row
		for( int i=0;i<l_iRow;++i )
		{
			l_pvVerticesForDrawPlane[i*2] = Vector2(l_vLeftUp.x,l_vLeftUp.y+i*e_fGridHeightDistance);
			l_pvVerticesForDrawPlane[i*2+1] = Vector2(l_vRightDown.x,l_vLeftUp.y+i*e_fGridHeightDistance);
		}
		//+2 from l_iTotalCount
		int	l_iTotalRow = l_iRow*2;
		for( int j=0;j<l_iColumn+2;++j )
		{
			l_pvVerticesForDrawPlane[l_iTotalRow+j*2] = Vector2(l_vLeftUp.x+j*e_fGridWidthDistance,l_vLeftUp.y);
			l_pvVerticesForDrawPlane[l_iTotalRow+j*2+1] = Vector2(l_vLeftUp.x+j*e_fGridWidthDistance,l_vRightDown.y);
		}
		FATMING_CORE::SetupShaderColor(e_vColor);
		myGlVertexPointer(2,l_pvVerticesForDrawPlane);
		FATMING_CORE::SetupShaderWorldMatrix(e_mat);
		MY_GLDRAW_ARRAYS(GL_LINES, 0, l_iTotalCount);
		UseShaderProgram(l_p2DShader);
		
	}

	void	AssignUVDataTo2Triangles(float*e_pSrc,float*e_pDest,bool e_bMirror)
	{
		//for UV
		if( !e_bMirror )
		{
			//left down
			e_pDest[0] = e_pSrc[0];			e_pDest[1] = e_pSrc[3];
			//right down
			e_pDest[2] = e_pSrc[2];			e_pDest[3] = e_pSrc[3];
			//left up
			e_pDest[4] = e_pSrc[0];			e_pDest[5] = e_pSrc[1];
			//left up
			e_pDest[6] = e_pSrc[0];			e_pDest[7] = e_pSrc[1];
			//right down
			e_pDest[8] = e_pSrc[2];			e_pDest[9] = e_pSrc[3];
			//right top
			e_pDest[10] = e_pSrc[2];		e_pDest[11] = e_pSrc[1];
		}
		else
		{
			e_pDest[0] = e_pSrc[2];			e_pDest[1] = e_pSrc[3];
			e_pDest[2] = e_pSrc[0];			e_pDest[3] = e_pSrc[3];
			e_pDest[4] = e_pSrc[2];			e_pDest[5] = e_pSrc[1];

			e_pDest[6] = e_pSrc[2];			e_pDest[7] = e_pSrc[1];
			e_pDest[8] = e_pSrc[0];			e_pDest[9] = e_pSrc[3];
			e_pDest[10] = e_pSrc[0];		e_pDest[11] = e_pSrc[1];
		}
	}

	void	AssignUVDataToTriangleStrip(float*e_pSrc,float*e_pDest,bool e_bMirror)
	{
		if( !e_bMirror )
		{
			e_pDest[0] = e_pSrc[0];			e_pDest[1] = e_pSrc[1];
			e_pDest[2] = e_pSrc[2];			e_pDest[3] = e_pSrc[1];
			e_pDest[4] = e_pSrc[0];			e_pDest[5] = e_pSrc[3];
			e_pDest[6] = e_pSrc[2];			e_pDest[7] = e_pSrc[3];
		}
		else
		{
			e_pDest[0] = e_pSrc[2];			e_pDest[1] = e_pSrc[1];
			e_pDest[2] = e_pSrc[0];			e_pDest[3] = e_pSrc[1];
			e_pDest[4] = e_pSrc[2];			e_pDest[5] = e_pSrc[3];
			e_pDest[6] = e_pSrc[0];			e_pDest[7] = e_pSrc[3];
		}
	}

	void	Assign4VerticesDataTo2Triangles(float*e_pSrc,float*e_pDest,int e_iStrip)
	{
		if( e_iStrip == 3 )
		{
			Vector3*l_pvSrc = (Vector3*)e_pSrc;
			Vector3*l_pvDest = (Vector3*)e_pDest;
			//left down
			l_pvDest[0] = l_pvSrc[2];
			//right down
			l_pvDest[1] = l_pvSrc[3];
			//left up
			l_pvDest[2] = l_pvSrc[0];
			//left up
			l_pvDest[3] = l_pvSrc[0];
			//right down
			l_pvDest[4] = l_pvSrc[3];
			//right top
			l_pvDest[5] = l_pvSrc[1];
		}
		else
		if( e_iStrip == 2 )
		{
			Vector2*l_pvSrc = (Vector2*)e_pSrc;
			Vector2*l_pvDest = (Vector2*)e_pDest;
			//left down
			l_pvDest[0] = l_pvSrc[2];
			//right down
			l_pvDest[1] = l_pvSrc[3];
			//left up
			l_pvDest[2] = l_pvSrc[0];
			//left up
			l_pvDest[3] = l_pvSrc[0];
			//right down
			l_pvDest[4] = l_pvSrc[3];
			//right top
			l_pvDest[5] = l_pvSrc[1];		
		}
	}

	void	DrawVectorProject(Vector3 e_v1,Vector3 e_v2,cMatrix44 e_mat)
	{
		Vector3	l_vD3 = e_v1.Projected(e_v2);
		float	l_f1[6] = {0,0,0,e_v1.x,e_v1.y,e_v1.z};
		float	l_f2[6] = {0,0,0,e_v2.x,e_v2.y,e_v2.z};
		float	l_f3[6] = {0,0,0,l_vD3.x,l_vD3.y,l_vD3.z};
		glEnable2D(640,480,0,true);
		GLRender::RenderLine(l_f1,2,Vector4(1,0,0,1),3,e_mat);
		GLRender::RenderLine(l_f2,2,Vector4(0,1,0,1),3,e_mat);
		GLRender::RenderLine(l_f3,2,Vector4(0,0,1,1),3,e_mat);
		glDisable2D();	
	}

	void	DrawCube(Vector3 e_vSize,cMatrix44 e_mat,Vector4 e_vColor)
	{
				//2,3				5
				//0					1,4
		cBaseShader*l_p2DShader = GetCurrentShader();
		UseShaderProgram(NO_TEXTURE_SHADER);
		FATMING_CORE::SetupShaderWorldMatrix(e_mat);
		FATMING_CORE::SetupShaderColor(e_vColor);
		Vector4	l_vColor[36];
		Vector3	l_vVetrices[] = {
			//
			Vector3(-e_vSize.x, e_vSize.y, e_vSize.z),					// Bottom Left Of The Quad (Top)
			Vector3( e_vSize.x, e_vSize.y, e_vSize.z),					// Bottom Right Of The Quad (Top)
			Vector3(-e_vSize.x, e_vSize.y,-e_vSize.z),					// Top Left Of The Quad (Top)

			Vector3(-e_vSize.x, e_vSize.y,-e_vSize.z),					// Top Left Of The Quad (Top)
			Vector3( e_vSize.x, e_vSize.y, e_vSize.z),					// Bottom Right Of The Quad (Top)
			Vector3( e_vSize.x, e_vSize.y,-e_vSize.z),					// Top Right Of The Quad (Top)
			//
			Vector3(-e_vSize.x,e_vSize.y,e_vSize.z),					// Bottom Left Of The Quad (Bottom)
			Vector3( e_vSize.x,e_vSize.y,e_vSize.z),					// Bottom Right Of The Quad (Bottom)
			Vector3(-e_vSize.x,e_vSize.y, -e_vSize.z),					// Top Left Of The Quad (Bottom)
			Vector3(-e_vSize.x,e_vSize.y, -e_vSize.z),					// Top Left Of The Quad (Bottom)
			Vector3( e_vSize.x,e_vSize.y,e_vSize.z),					// Bottom Right Of The Quad (Bottom)
			Vector3( e_vSize.x,e_vSize.y, -e_vSize.z),					// Top Right Of The Quad (Bottom)
			//
			Vector3(-e_vSize.x,-e_vSize.y, e_vSize.z),					// Bottom Left Of The Quad (Front)
			Vector3( e_vSize.x,-e_vSize.y, e_vSize.z),					// Bottom Right Of The Quad (Front)
			Vector3(-e_vSize.x, e_vSize.y, e_vSize.z),					// Top Left Of The Quad (Front)
			Vector3(-e_vSize.x, e_vSize.y, e_vSize.z),					// Top Left Of The Quad (Front)
			Vector3( e_vSize.x,-e_vSize.y, e_vSize.z),					// Bottom Right Of The Quad (Front)
			Vector3( e_vSize.x, e_vSize.y, e_vSize.z),					// Top Right Of The Quad (Front)
			
			//
			Vector3(-e_vSize.x, -e_vSize.y,-e_vSize.z),					// Bottom Left Of The Quad (Back)
			Vector3( e_vSize.x, -e_vSize.y,-e_vSize.z),					// Bottom Right Of The Quad (Back)
			Vector3(-e_vSize.x,e_vSize.y,-e_vSize.z),					// Top Left Of The Quad (Back)
			Vector3(-e_vSize.x,e_vSize.y,-e_vSize.z),					// Top Left Of The Quad (Back)
			Vector3( e_vSize.x,-e_vSize.y,-e_vSize.z),					// Bottom Right Of The Quad (Back)
			Vector3( e_vSize.x,e_vSize.y,-e_vSize.z),					// Top Right Of The Quad (Back)
			
			//
			Vector3(-e_vSize.x,-e_vSize.y, e_vSize.z),					// Bottom Left Of The Quad (Left)
			Vector3(-e_vSize.x,-e_vSize.y,-e_vSize.z),					// Bottom Right Of The Quad (Left)
			Vector3(-e_vSize.x, e_vSize.y, e_vSize.z),					// Top Left Of The Quad (Left)
			Vector3(-e_vSize.x, e_vSize.y, e_vSize.z),					// Top Left Of The Quad (Left)
			Vector3(-e_vSize.x,-e_vSize.y,-e_vSize.z),					// Bottom Right Of The Quad (Left)
			Vector3(-e_vSize.x, e_vSize.y,-e_vSize.z),					// Top Right Of The Quad (Left)
			
			//
			Vector3( e_vSize.x,-e_vSize.y,-e_vSize.z),					// Bottom Left Of The Quad (Right)
			Vector3( e_vSize.x,-e_vSize.y, e_vSize.z),					// Bottom Right Of The Quad (Right)
			Vector3( e_vSize.x, e_vSize.y,-e_vSize.z),					// Top Left Of The Quad (Right)
			Vector3( e_vSize.x, e_vSize.y,-e_vSize.z),					// Top Left Of The Quad (Right)
			Vector3( e_vSize.x,-e_vSize.y, e_vSize.z),					// Bottom Right Of The Quad (Right)
			Vector3( e_vSize.x, e_vSize.y, e_vSize.z),					// Top Right Of The Quad (Right)
			
		};
		myGlVertexPointer(3,l_vVetrices);
		MY_GLDRAW_ARRAYS(GL_TRIANGLES, 0, 36);
		UseShaderProgram(l_p2DShader);
	}
//end namespace GLRender
};