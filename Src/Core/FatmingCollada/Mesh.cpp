#include "StdAfx.h"
#include "Mesh.h"
#include "FatmingController.h"


sTechniqueAndTexture::~sTechniqueAndTexture()
{

}
const wchar_t*     cMesh::TypeID( L"cMesh" );

cMesh::cMesh(float*	e_ppfVertexBuffer[TOTAL_FVF],
		UINT	e_uiVertexBufferCount,
		UINT*	e_puiIndexBuffer,
		UINT	e_uiIndexBufferCount)
{
	m_Sphere.vCenter = Vector3::Zero;
	m_Sphere.Radius = 0;
	memset(m_bVerticesBuffer,0,sizeof(bool)*TOTAL_FVF);
	m_uiVertexBufferCount = e_uiVertexBufferCount;
	//fuck,because animation need position info
#ifndef OPENGLES_2_X
	g_bVBOSupported = false;
#else
	g_bVBOSupported = true;
	//g_bVBOSupported = false;
#endif

#ifdef WIN32
	OutputDebugString(L"FUCK!!!\n");
	OutputDebugString(L"glGet GL_ARRAY_BUFFER_BINDING,GL_ELEMENT_ARRAY_BUFFER_BINDING,before get buffer data u have to \
					   glBindBuffer(GL_ARRAY_BUFFER, l_uiBufferID);");
#endif
	if( g_bVBOSupported )
		m_pVBOBuffer = new cVBOBuffer();
	else
		m_pVBOBuffer = 0;
	for(int i=0;i<TOTAL_FVF;++i)
	{
		m_ppfVerticesBuffer[i] = 0;
		if(e_ppfVertexBuffer[i])
		{
			m_uiFVF_Format|=(1<<i);
			if( i == FVF_POS )
			{
				Vector3	l_vMin(FLT_MAX,FLT_MAX,FLT_MAX);
				Vector3	l_vMax(FLT_MIN,FLT_MIN,FLT_MIN);
				Vector3*l_pvAllVertex = (Vector3*)e_ppfVertexBuffer[i];
				if( l_pvAllVertex )
				{
					for(UINT i=0;i<this->m_uiVertexBufferCount;++i)
					{
						if(l_vMin.x>l_pvAllVertex[i].x)	l_vMin.x = l_pvAllVertex[i].x;
						if(l_vMin.y>l_pvAllVertex[i].y)	l_vMin.y = l_pvAllVertex[i].y;
						if(l_vMin.z>l_pvAllVertex[i].z)	l_vMin.z = l_pvAllVertex[i].z;
						if(l_vMax.x<l_pvAllVertex[i].x)	
							l_vMax.x = l_pvAllVertex[i].x;
						if(l_vMax.y<l_pvAllVertex[i].y)	
							l_vMax.y = l_pvAllVertex[i].y;
						if(l_vMax.z<l_pvAllVertex[i].z)	
							l_vMax.z = l_pvAllVertex[i].z;
					}
					m_Sphere.vCenter = (l_vMax-l_vMin)/2;
					m_Sphere.Radius = (l_vMax-l_vMin).Length();
				}
			}
			if( m_pVBOBuffer )
			{
				m_pVBOBuffer->SetupVerticesBuffer(e_ppfVertexBuffer[i],i,g_iFVF_DataStride[i],e_uiVertexBufferCount);
			}
			else
			{
				UINT	l_uiSize = g_iFVF_DataStride[i]*e_uiVertexBufferCount;
				m_ppfVerticesBuffer[i] = e_ppfVertexBuffer[i];
				e_ppfVertexBuffer[i] = 0;
				//m_ppfVerticesBuffer[i] = new float[l_uiSize];
				//memcpy(m_ppfVerticesBuffer[i],e_ppfVertexBuffer[i],sizeof(float)*l_uiSize);
			}
			m_bVerticesBuffer[i] = true;
		}
	}
	assert(m_uiFVF_Format|(1<<0));//it must have position data
	m_uiIndexBufferCount = e_uiIndexBufferCount;
	if( m_pVBOBuffer )
	{
		m_pVBOBuffer->SetupIndicesBuffer((float*)e_puiIndexBuffer,1,m_uiIndexBufferCount);
		m_puiIndexBuffer = 0;
	}
	else
	{
#ifdef __IPHONE
		unsigned short *l_pusData = new unsigned short[e_uiIndexBufferCount];
		for( UINT i=0;i<e_uiIndexBufferCount;++i )
		{
			l_pusData[i] = (unsigned short)e_puiIndexBuffer[i];
		}
		m_puiIndexBuffer = (UINT*)l_pusData;
#else
		m_puiIndexBuffer = new UINT[e_uiIndexBufferCount];
		memcpy(m_puiIndexBuffer,e_puiIndexBuffer,sizeof(UINT)*e_uiIndexBufferCount);	
#endif
	}
}

cMesh::cMesh(cMesh*e_pMesh)
{
	if( e_pMesh )
	{
		SetName(e_pMesh->GetName());
		memcpy(m_bVerticesBuffer,e_pMesh->m_bVerticesBuffer,sizeof(bool)*TOTAL_FVF);
		m_pVBOBuffer = e_pMesh->m_pVBOBuffer;
		//m_bFromResource = true;
		CopyListPointer(e_pMesh);
		for(int i=0;i<TOTAL_FVF;++i)
		{
			m_ppfVerticesBuffer[i] = e_pMesh->m_ppfVerticesBuffer[i];
		}
		m_uiFVF_Format = e_pMesh->m_uiFVF_Format;
		assert(m_uiFVF_Format|(1<<0));//it must have position data
		m_uiVertexBufferCount = e_pMesh->m_uiVertexBufferCount;
		m_uiIndexBufferCount = e_pMesh->m_uiIndexBufferCount;
		m_puiIndexBuffer = e_pMesh->m_puiIndexBuffer;
		m_Sphere = e_pMesh->m_Sphere;
	}
	else
	{
		memset(m_bVerticesBuffer,0,sizeof(bool)*TOTAL_FVF);
		m_uiVertexBufferCount = 0;
		m_uiIndexBufferCount = 0;
		m_puiIndexBuffer = 0;
		m_uiFVF_Format = 0;
		m_pVBOBuffer = 0;
		for(int i=0;i<TOTAL_FVF;++i)
			m_ppfVerticesBuffer[i] = 0;
	}
}
cMesh::~cMesh()
{
//if the mesh is load from collada parser,the texture is store in ColladaPaser::cLibraryImages::m_ImageList
	if(!this->m_bFromResource)
	{
		SAFE_DELETE(m_pVBOBuffer);
		for( int i=0;i<TOTAL_FVF;++i )
			SAFE_DELETE(m_ppfVerticesBuffer[i]);
		SAFE_DELETE(m_puiIndexBuffer);
	}
}

void	cMesh::Render()
{//ensure u have create depth framebuffer...
	for(UINT i=0;i<this->m_ObjectList.size();++i)
	{
		if((*this)[i]->GetTechniqueType() == eTL_DIFFUSE)
			(*this)[i]->ApplyImage();
	}
	//SetupShaderWorldMatrix(this->GetWorldTransform()*cMatrix44::ZupToYUp);
	SetupShaderWorldMatrix(cMatrix44::ZupToYUp);
#ifdef _DEBUG
		GLuint ll[TOTAL_FVF];
		memcpy(ll,g_uiAttribArray,sizeof(GLuint)*TOTAL_FVF);
#endif
	if( m_pVBOBuffer )
	{
#ifndef OPENGLES_2_X
		m_pVBOBuffer->Render();
#else
		m_pVBOBuffer->RenderByShader();
#endif
	}
	else
	{
		//DrawCube(Vector3(1,1,1),cMatrix44::Identity,Vector4(1,1,0,1));
#ifndef OPENGLES_2_X
		myGlVertexPointer(3, m_ppfVerticesBuffer[FVF_POS]);
		if( m_bVerticesBuffer[FVF_TEX0] )
			myGlUVPointer(2, m_ppfVerticesBuffer[FVF_TEX0]);
		if( m_bVerticesBuffer[FVF_NORMAL] )
			myGlNormalPointer(3,m_ppfVerticesBuffer[FVF_NORMAL]);
		if( m_bVerticesBuffer[FVF_DIFFUSE] )
			myGlColorPointer(4,m_ppfVerticesBuffer[FVF_DIFFUSE]);
#else
		for( int i=0;i<TOTAL_FVF;++i )
		{
			if( this->m_bVerticesBuffer[i] && g_uiAttribArray[i] != -1 )
			{
				glVertexAttribPointer(g_uiAttribArray[i],g_iFVF_DataStride[i], g_iFVF_DataType[i],0, 0, m_ppfVerticesBuffer[i]);
			}
		}
#endif
		MY_GLDRAW_ELEMENTS(GL_TRIANGLES,m_uiIndexBufferCount, g_iDrawindiceType,m_puiIndexBuffer );
	}
}

void	cMesh::AssignSkinningData(float*e_pfWeightsData,float*e_pfInflunceJointsData,UINT e_uiVertexCount)
{
	assert(e_uiVertexCount==m_uiVertexBufferCount);
	assert(m_ppfVerticesBuffer[FVF_SKINNING_WEIGHT] == 0);
	assert(m_ppfVerticesBuffer[FVF_SKINNING_BONE_INDEX] == 0);
	this->m_bVerticesBuffer[FVF_SKINNING_WEIGHT] = true;
	this->m_bVerticesBuffer[FVF_SKINNING_BONE_INDEX] = true;
	if( m_pVBOBuffer )
	{
		m_pVBOBuffer->SetupVerticesBuffer(e_pfWeightsData,FVF_SKINNING_WEIGHT,g_iFVF_DataStride[FVF_SKINNING_WEIGHT],e_uiVertexCount);
		//unsign byte data size is 1
		m_pVBOBuffer->SetupVerticesBuffer(e_pfInflunceJointsData,FVF_SKINNING_BONE_INDEX,g_iFVF_DataStride[FVF_SKINNING_BONE_INDEX],e_uiVertexCount,1);
	}
	else
	{
		m_ppfVerticesBuffer[FVF_SKINNING_WEIGHT] = new float[m_uiVertexBufferCount*MAX_RELEATED_BONE_COUNT];
		m_ppfVerticesBuffer[FVF_SKINNING_BONE_INDEX] = (float*)new char[m_uiVertexBufferCount*MAX_RELEATED_BONE_COUNT];
		memcpy(m_ppfVerticesBuffer[FVF_SKINNING_WEIGHT],e_pfWeightsData,sizeof(float)*m_uiVertexBufferCount*MAX_RELEATED_BONE_COUNT);
		memcpy(m_ppfVerticesBuffer[FVF_SKINNING_BONE_INDEX],e_pfInflunceJointsData,sizeof(char)*m_uiVertexBufferCount*MAX_RELEATED_BONE_COUNT);
//#ifdef _DEBUG
//		for( UINT i=0;i<e_uiVertexCount;++i )
//		{
//			float	l_fWeight = m_ppfVerticesBuffer[FVF_SKINNING_WEIGHT][i*4]+
//				m_ppfVerticesBuffer[FVF_SKINNING_WEIGHT][i*4+1]+
//				m_ppfVerticesBuffer[FVF_SKINNING_WEIGHT][i*4+2]+
//				m_ppfVerticesBuffer[FVF_SKINNING_WEIGHT][i*4+3];
//			assert(l_fWeight>=0.9f&&l_fWeight<=1.1f);
//		}
//#endif
	}
}

void	cMesh::ChangeDiffuseImage(cSimpleGLTexture*e_pSimpleGLTexture)
{
	int	l_iDiffuseIndex = -1;
	for(UINT i=0;i<this->m_ObjectList.size();++i)
	{
		sTechniqueAndTexture*l_pTechniqueAndTexture = (*this)[i];
		if(l_pTechniqueAndTexture->GetTechniqueType() == eTL_DIFFUSE)
		{
			l_iDiffuseIndex = i;
		}
	}
	if( l_iDiffuseIndex != -1 )
		this->RemoveObject(l_iDiffuseIndex);
	sTechniqueAndTexture*l_pTechniqueAndTexture = new sTechniqueAndTexture(e_pSimpleGLTexture,eTL_DIFFUSE);
	bool	l_b = this->AddObject(l_pTechniqueAndTexture);
	assert(l_b);
}