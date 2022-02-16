#ifndef _FAI_CHARACTER_PARAMTER_H_
#define _FAI_CHARACTER_PARAMTER_H_

namespace FATMING_AI
{
	enum	eCharacterAttribute
	{
		eCA_HP = 0,
		eCA_MP,
		eCA_DEF,
		eCA_STR,
		eCA_AGI,
		eCA_DEX,
		eCA_INT,
		eCA_SPEED,
		eCA_TORQUE,
		eCA_UNKNOW,
	};

	static WCHAR*	g_strCharacterAttribute[eCA_UNKNOW+1] = {
		L"HP",
		L"MP",
		L"DEF",
		L"STR",
		L"AGI",
		L"DEX",
		L"INT",
		L"SPEED",
		L"TORQUE",
		L"Unknow!!",
	};

	//it could only has 2 data just for UI show or just for data without ui info depend on the situchation
	struct	sCharacterAttributeAndUIInfo
	{
		eCharacterAttribute	CharacterAttribute;
		float				fValue;
		Vector2				vUIPos;
	};
//<Monkey	Count="3">
//	<Data Attribute="HP" Value="10"  Posiion="0,0" />
//	<Data Attribute="Agile" Value="10"  Posiion="0,100" />
//	<Data Attribute="Defence" Value="10"  Posiion="0,110" />
//</Monkey>
	std::vector<sCharacterAttributeAndUIInfo>	ParseAnimalStatusAndUIInfo(char*e_strFileName);

	sCharacterAttributeAndUIInfo*	GetCharacterAttributeAndUIInfo(std::vector<sCharacterAttributeAndUIInfo>*e_pCharacterAttributeAndUIInfoVector,
		eCharacterAttribute	e_CharacterAttribute,int*e_piReceivedIndex);


	//<Status Name="SmallZombie" HP="100" Strgeth="100" Defence="100" Agile="100" Dexterous="100" Lucky="100" />
    class	cFAICharacterStatus:public NamedTypedObject
    {
    public:
	    int	m_iHP;
	    int	m_iStrgeth;
	    int	m_iDefence;
	    int	m_iAgile;
	    int	m_iDexterous;
        int	m_iLucky;
    public:
        DEFINE_TYPE_INFO();
	    cFAICharacterStatus();
	    cFAICharacterStatus(TiXmlElement*e_pElement);
	    cFAICharacterStatus(cFAICharacterStatus*e_pStatus)
	    {
	        m_iHP = e_pStatus->m_iHP;
	        m_iStrgeth = e_pStatus->m_iStrgeth;
	        m_iDefence = e_pStatus->m_iDefence;
	        m_iAgile = e_pStatus->m_iAgile;
	        m_iDexterous = e_pStatus->m_iDexterous;
            m_iLucky = e_pStatus->m_iLucky;
	    }
		CLONE_MYSELF(cFAICharacterStatus);
	    cFAICharacterStatus*GetStatus(){ return this; }
    };

    //basic character let AI control position and some info.
    class   cFAICharacterInterface:public Frame
    {
		cBound*			m_pBound;
        GET_SET_DEC(float,m_fSpeed,GetSpeed,SetSpeed);
		//if AI is dying,speed is posible let AI run faster
		GET_SET_DEC(float,m_fRunAwaySpeed,GetRunAwaySpeed,SetRunAwaySpeed);
		GET_SET_DEC(float,m_fAnimationSpeed,GetAnimationSpeed,SetAnimationSpeed);
        GET_SET_DEC(cFAICharacterStatus*,m_pAICharacterStatus,GetStatus,SetStatus);
		//for 2D angle
		GETP_SET_DEC(Vector3,m_v2DAngle,GetAngle,SetAngle);
    public:
        DEFINE_TYPE_INFO();
		cFAICharacterInterface(TiXmlElement*e_pElement);
		cFAICharacterInterface(cFAICharacterInterface*e_pAICharacterInterface);
        ~cFAICharacterInterface();
		CLONE_MYSELF(cFAICharacterInterface);
        //below function let AIBehavior to fetch the data as needed.
        Vector3*		GetPosition();
		virtual	void	DebugRender();
	    //some AI need animation data,so here return a NULL but you could override this,to feed data into AI behavior.
        //ex::cFAIMPDIAnimationChangeAIBehavior
		virtual	void	 SetBound(cBound*e_pData);
	    virtual void*    GetAnimationData(){ return 0; }
	    virtual void*    GetWeaponData(){ return 0; }
	    virtual cBound*	 GetBound(){ return m_pBound; }
    };

//end namespace
}

#endif