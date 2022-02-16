namespace FATMING_CORE
{
#ifdef WIN32
//for MPDI old version 
#ifdef MPDI_VERSION_1_0
	void	ExportPointDataList(ATG::XMLWriter*e_pXMLWriter,cCueToStartCurveWithTime*e_pCueToStartCurvesWithTime);
#else
	char*	TexBehaviorDataWithImageIndexDataToString(sTexBehaviorDataWithImageIndexData e_sTexBehaviorDataWithImageIndexData,sPuzzleData**e_ppPuzzleData,int e_iNumData);
	void	ExportPointDataList(ATG::XMLWriter*e_pXMLWriter,cCueToStartCurveWithTime*e_pCueToStartCurvesWithTime);
#endif
	void	ExportMPDI(ATG::XMLWriter*e_pXMLWriter,cMPDIList*e_pMPDIList);
//end WIN32
#endif	
}