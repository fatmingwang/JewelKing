/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#include "stdafx.h"
#include "Bullet.h"
/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
cBullet::cBullet( cMPDI*e_pBulletMPDI, cbtShapeCollision* e_pBulletCollision, float e_fSpeed,cMPDI* e_pWeb, cbtShapeCollision* e_pWebCollision )
{
	m_pTargetMonster = 0;
	m_iNumMonsterShoot = 0;
	m_fAngle = 0.f;
	m_vPos = Vector3::Zero;
	m_matMatrix = cMatrix44::Identity;
	m_iPayRate = 1;
	m_fSpeed = e_fSpeed;
	m_pMiniGame = 0;
	//
	m_pBulletMPDI = 0;
	m_pBulletCollision = 0;
	m_pWebMPDI = 0;
	m_pWebCllision = 0;
	//
	if( e_pBulletMPDI )
	{
		m_pBulletMPDI = dynamic_cast<cMPDI*>(e_pBulletMPDI->Clone());
		m_pBulletMPDI->SetPosByImageCenter(Vector3::Zero);
		m_pBulletMPDI->SetExtraTransform(&m_matMatrix);
	} //end if
	if( e_pBulletCollision )
	{
		m_pBulletCollision = dynamic_cast<cbtShapeCollision*>(e_pBulletCollision->Clone());
	} //end if
	if( e_pWeb )
	{
		m_pWebMPDI = new cMPDI(e_pWeb);
	} //end if
	if( e_pWebCollision )
	{
		m_pWebCllision = dynamic_cast<cbtShapeCollision*>(e_pWebCollision->Clone());
	} //end if
	//
	//m_pBulletMPDI->Init();
	//
	this->m_eBulletStatus = eBS_IDLE;
	//
	m_iPowerLevel = 1;
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
cBullet::~cBullet()
{
	Destroy();
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cBullet::SetDirection(float e_fAngle)
{
	//Y is invert
	m_vDirection = AngleToDirection(360-e_fAngle);
	//because Animation angle is 90 plus
	if( m_pBulletMPDI )
	{
		//m_pBulletMPDI->SetAngle(360-e_fAngle+90);
		m_fAngle = 360-e_fAngle+90;
		//m_matMatrix = cMatrix44::ZAxisRotationMatrix(D3DXToRadian(360-e_fAngle+90));
	}
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cBullet::SetDirection(Vector3 e_vDirection)
{
	m_vDirection = e_vDirection.Normalize();
	float	l_fAngle = 180.f-UT::GetAngleBy2Point(0,0,m_vDirection.x,m_vDirection.y);
	if( m_pBulletMPDI )
	{
		m_fAngle = l_fAngle;
	}
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void		cBullet::CastNetFishing()
{
	m_bDoCollided = true;
	if( m_pWebCllision )
	{
		m_pWebCllision->SetTransform(m_pBulletCollision->GetTransform());
	}
	if( m_pBulletMPDI )
	{
		Vector3	l_vPos = m_pBulletMPDI->GetPosByImageCenter();
		Vector3	l_vAngle = m_pBulletMPDI->GetAngle();
		m_pWebMPDI->Init();
		m_pWebMPDI->SetPosByImageCenter(this->m_matMatrix.m[3]);
		m_pWebMPDI->SetAngle(m_fAngle);
	}
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cBullet::WebUpdate(float e_fElpaseTime)
{
	//if( !m_bDoCollided )
	//{
	//	CastNetFishing();
	//}
	if( m_pWebMPDI )
		this->m_pWebMPDI->Update(e_fElpaseTime);
	if( m_pWebMPDI->IsDone() )
	{
		if( m_eBulletStatus == eBS_EXPLOSION_DONE )
			this->m_eBulletStatus = eBS_DONE;
		else
			this->m_eBulletStatus = eBS_EXPLOSION_DONE;
	}
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cBullet::Fire( cPlayerBehaviorBase*e_pPlayer, Vector3 e_vStartPos, float e_fAngle )
{
	SetDirection( e_fAngle );
	m_pPlayer = e_pPlayer;
	Vector2	l_vSize = m_pBulletMPDI->GetDrawSize()/2.f;
	m_vPos = e_vStartPos;
	this->m_eBulletStatus = eBS_MOVING;	
	this->SetTransform();
}

void	cBullet::Fire(cPlayerBehaviorBase*e_pPlayer,Vector3 e_vStartPos,float e_fAngle,cMonster*e_pTargetMonster)
{
	SetDirection( e_fAngle );
	m_pPlayer = e_pPlayer;
	Vector2	l_vSize = m_pBulletMPDI->GetDrawSize()/2.f;
	m_vPos = e_vStartPos;
	this->m_eBulletStatus = eBS_TRACING_MONSTER;
	this->SetTransform();
	this->m_pTargetMonster = e_pTargetMonster;
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cBullet::Fire(cPlayerBehaviorBase*e_pPlayer,Vector3 e_vStartPos,Vector3 e_vDirection)
{
	SetDirection(e_vDirection);
	m_pPlayer = e_pPlayer;
	m_vPos = e_vStartPos;
	this->m_eBulletStatus = eBS_MOVING;
	SetTransform();
}


void	cBullet::SetTransform()
{
	m_matMatrix = cMatrix44::TranslationMatrix(m_vPos)*cMatrix44::ZAxisRotationMatrix(D3DXToRadian(this->m_fAngle));
	if( cBulletManager::m_sbCollideSkip)
		return;
	//Vector2	l_vSize = m_pBulletMPDI->GetDrawSize()/2.f;
	//cMatrix44	l_Collisionmat = cMatrix44::TranslationMatrix(m_vPos)*cMatrix44::ZAxisRotationMatrix(D3DXToRadian(this->m_fAngle));
	//this->m_pBulletCollision->SetTransform(l_Collisionmat);
	this->m_pBulletCollision->SetTransform(m_matMatrix);
}
/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cBullet::BulletUpdate( float e_fElpaseTime )
{
	m_pBulletMPDI->Update( e_fElpaseTime );
	Vector3	l_vMovedDistance = m_vDirection*e_fElpaseTime*m_fSpeed;
	//Vector3	l_vPos = m_pBulletMPDI->GetPosByImageCenter();
	Vector3	l_vPos = *(Vector3*)&this->m_matMatrix.m[3];
	l_vPos += l_vMovedDistance;
	bool	l_bHit = false;
	Vector2	l_vDrawSize = this->m_pBulletMPDI->GetDrawSize()/4.f;
	Vector3	l_vReflectNormal(0,0,0);
	//
	float	l_fOffset = 0.f;
	Vector4	l_vWall(-l_fOffset,-l_fOffset,cGameApp::m_svGameResolution.x+l_fOffset,cGameApp::m_svGameResolution.y+l_fOffset);
	if( l_vPos.x < l_vWall.x )
	{
		l_vReflectNormal.x = 1.f;
		l_bHit = true;
		//l_vPos.x = -l_vPos.x;
		l_vPos.x = l_vWall.x;
	}
	else if( l_vPos.x > l_vWall.z )
	{
		l_vReflectNormal.x = -1.f;
		l_bHit = true;
		//l_vPos.x = cGameApp::m_svGameResolution.x-(l_vPos.x-cGameApp::m_svGameResolution.x);
		l_vPos.x = l_vWall.z;
	}
	//if( l_bHit )
	//{
	//	if( m_vDirection.y >0.001f  )
	//	{
	//		l_vPos.y += l_vDrawSize.x;
	//	}
	//	else
	//	if( m_vDirection.y <-0.001f  )
	//	{
	//		l_vPos.y -= l_vDrawSize.x;
	//	}
	//}
	bool	l_bHitY = false;
	//
	if( l_vPos.y < l_vWall.y )
	{
		l_vReflectNormal.y = 1.f;
		//l_vPos.y = -l_vPos.y;
		l_vPos.y = l_vWall.y;
		l_bHitY = l_bHit = true;
	}
	else if( l_vPos.y > l_vWall.w )
	{
		l_vReflectNormal.y = -1.f;
		//l_vPos.y = cGameApp::m_svGameResolution.y-(l_vPos.y-cGameApp::m_svGameResolution.y);
		l_vPos.y = l_vWall.w;
		l_bHitY = l_bHit = true;
	}
	//if( l_bHitY )
	//{
	//	if( m_vDirection.x >0.001f  )
	//	{
	//		l_vPos.x += l_vDrawSize.x;
	//	}
	//	else
	//	if( m_vDirection.x <-0.001f  )
	//	{
	//		l_vPos.x -= l_vDrawSize.x;
	//	}
	//}
	
	if( l_bHit )
	{
		m_vDirection = Vector3Reflect(m_vDirection,l_vReflectNormal);
		SetDirection(m_vDirection);
	} //end if
	//
	m_vPos = l_vPos;
	SetTransform();
}

void	cBullet::BulletTracingUpdate( float e_fElpaseTime )
{

}
/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cBullet::f_Update_Bullet_MiniGame( float e_fElpaseTime )
{
	m_pBulletMPDI->Update( e_fElpaseTime );
	Vector3	l_vMovedDistance = m_vDirection*e_fElpaseTime*m_fSpeed;
	//Vector3	l_vPos = m_pBulletMPDI->GetPosByImageCenter();
	Vector3	l_vPos = *(Vector3*)&this->m_matMatrix.m[3];
	l_vPos += l_vMovedDistance;
	bool	l_bHit = false;
	Vector3	l_vReflectNormal( 0, 0, 0 );
	//

	if( l_vPos.x < 0 )
	{
//		l_vReflectNormal.x = 1.f;
		l_bHit = true;
//		l_vPos.x = -l_vPos.x;
	}
	else if( l_vPos.x > cGameApp::m_svGameResolution.x )
	{
//		l_vReflectNormal.x = -1.f;
		l_bHit = true;
//		l_vPos.x = cGameApp::m_svGameResolution.x-(l_vPos.x-cGameApp::m_svGameResolution.x);
	} //end if
	//
	if( l_vPos.y < 0 )
	{
//		l_vReflectNormal.y = 1.f;
//		l_vPos.y = -l_vPos.y;
		l_bHit = true;
	}
	else if( l_vPos.y > cGameApp::m_svGameResolution.y )
	{
//		l_vReflectNormal.y = -1.f;
//		l_vPos.y = cGameApp::m_svGameResolution.y-(l_vPos.y-cGameApp::m_svGameResolution.y);
		l_bHit = true;
	} //end if
	//
	if( l_bHit )
	{
		//?s?????????u?A?I???????N?X?h????
		//m_vDirection = Vector3Reflect( m_vDirection,l_vReflectNormal );
		//SetDirection( m_vDirection );
		m_eBulletStatus = eBS_EXPLOSION_DONE; //???l?u???? 79979
		//???v?i?D MiniGame ????????
		m_pMiniGame->f_Fire_ed();
		m_pMiniGame = 0;
	} //end if

	//
	//m_pBulletMPDI->SetPosByImageCenter(l_vPos);
	m_vPos = l_vPos;
	this->SetTransform();
	//
//	if(m_pBulletCollision)
//	{
//		m_pBulletCollision->SetTransform(m_pBulletMPDI->GetTransformByImageCenter());
//	} //end if
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cBullet::Update(float e_fElpaseTime)
{
	if( m_pMiniGame )
	{
		f_Update_Bullet_MiniGame( e_fElpaseTime );
	}
	else
	{
		if(this->m_eBulletStatus == eBS_EXPLOSION)
		{
			WebUpdate(e_fElpaseTime);
		}
		else
		{
			if( this->m_eBulletStatus == eBS_MOVING )
			{
				BulletUpdate(e_fElpaseTime);
			}
			else
			if( this->m_eBulletStatus == eBS_EXPLOSION )
			{
				this->CastNetFishing();
			}
			else
			if( this->m_eBulletStatus == eBS_TRACING_MONSTER )
			{
				BulletTracingUpdate(e_fElpaseTime);
			}
		}
	}
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cBullet::Render(void)
{
	if( m_pMiniGame )
	{
		f_Render_Bullet_MiniGame();
	}
	else
	{
		if( this->m_eBulletStatus == eBS_MOVING )
		{
			m_pBulletMPDI->Render();
		}
		else if( this->m_eBulletStatus == eBS_EXPLOSION )
		{
			this->m_pWebMPDI->Render();
		} //end if
	} //end if
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cBullet::f_Render_Bullet_MiniGame(void)
{
	if( this->m_eBulletStatus == eBS_MOVING )
	{
//		m_pBulletMPDI->Render();
	}
	else if( this->m_eBulletStatus == eBS_EXPLOSION )
	{
//		this->m_pWebMPDI->Render();
	} //end if
}


/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cBullet::Init()
{
	m_pTargetMonster = 0;
	m_fAngle = 0.f;
	m_vPos = Vector3::Zero;
	m_matMatrix = cMatrix44::Identity;
	m_pBulletMPDI->Init();
	this->m_eBulletStatus = eBS_IDLE;
	this->m_bDoCollided = false;
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cBullet::DebugRender()
{
	if(this->m_eBulletStatus == eBS_MOVING)
	{
		if(this->m_pBulletCollision)
		{
			m_pBulletCollision->DebugRender();
		}
	}
	else
	if( this->m_eBulletStatus == eBS_EXPLOSION )
	{
		if(this->m_pWebCllision)
		{
			m_pWebCllision->DebugRender();
		}
	}
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cBullet::Destroy()
{
	SAFE_DELETE(m_pWebMPDI);
	SAFE_DELETE(m_pWebCllision);
	SAFE_DELETE(m_pBulletMPDI);
	SAFE_DELETE(m_pBulletCollision);
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
bool	cBullet::Collide(cbtShapeCollision* e_pbtShapeCollision)
{
	if( this->m_pBulletCollision )
	{
		return m_pBulletCollision->Collide(e_pbtShapeCollision);
	}
	return false;
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
bool		cBullet::f_Collide_Bullet_MiniGame( cbtShapeCollision*e_pbtShapeCollision )
{
	return false;
}


/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */