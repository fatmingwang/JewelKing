����O�{���i�s�{��h�}�C(Field-Programmable Gate Arrays�FFPGA)�H

FPGA�O�@�إi�H���Ƨ��ܲպA���q���A�i�ϥΪ̶i��s�{���޿�h����A
�S�O�A�Ω󲣫~�}�o�ɥ������_�ܧ�]�p�����ΡA�H���ĥ[�t���~�W���ɶ�
�CFPGA���޿�h�S�ʡA�i�̳]�p�̪��ݭn�[�H���ܡA�ô��ѦU�ذ򥻥\��C
�M�ӡA�@��Ө��AFPGA��DSP�󬰩��Q�A�Ү��Ӫ��\�v�]�����F
FPGA���t�פ]��ASIC��C�A�L�k�����������]�p�A�]��FPGA�`�PDSP��ASIC�f�t�ϥΡA�H���ѧ�n���u�ʡB����ʯ��P�q���ݨD�C



cIOSMIOManager is a top node,it contain driver interface and all device,
driver interface contain all IO data list,



the purpose to update all device and IO is go as follow

//first to call all IOSM data upate to setup singnal
//second to call device update
//
//so the IOSMIOData could set status to back if necessary
//and Device Behavior won't set IOSMIO data incorrectly.

void	cIOSMIOManager::Update(float e_fElpaseTime);