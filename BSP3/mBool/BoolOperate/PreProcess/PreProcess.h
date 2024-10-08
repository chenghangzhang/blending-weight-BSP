#pragma once

class PreProcess {

public:
	PreProcess(vector<Mesh*> p_Meshes, const Tolerance& p_Tolerence)
		: m_PreProcessMeshes(p_Meshes), m_Toler(p_Tolerence)
	{
	
	}

protected: //输入输出变量
	vector<Mesh*> m_PreProcessMeshes;
	Tolerance m_Toler;

public:
	void run();

};
