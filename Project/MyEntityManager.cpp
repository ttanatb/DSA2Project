#include "MyEntityManager.h"
using namespace Simplex;
//  MyEntityManager
MyEntityManager* MyEntityManager::m_pInstance = nullptr;
void MyEntityManager::Init(void)
{
	//reset variables
	m_uEntityCount = 0;
	m_entityList.clear();
}
void MyEntityManager::Release(void)
{
	//delete everything in the list
	for (uint i = 0; i < m_uEntityCount; ++i) {
		SafeDelete(m_entityList[i]);
	}

	//reset variables
	m_uEntityCount = 0;
	m_entityList.clear();
}
MyEntityManager* MyEntityManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MyEntityManager();
	}
	return m_pInstance;
}
void MyEntityManager::ReleaseInstance()
{
	SafeDelete(m_pInstance);
}
int Simplex::MyEntityManager::GetEntityIndex(String a_sUniqueID)
{
	//loop through to find the appropriate id
	for (uint i = 0; i < m_entityList.size(); ++i) {
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID) {
			return i;
		}
	}

	//std::cout << a_sUniqueID << " not found" << std::endl;
	return -1;
}
//Accessors
Model* Simplex::MyEntityManager::GetModel(uint a_uIndex)
{
	if (a_uIndex == -1)
		a_uIndex = m_uEntityCount - 1;

	if (a_uIndex < m_uEntityCount)
		return m_entityList[a_uIndex]->GetModel();
	else {
		//print out something
		return nullptr;
	}
}
Model* Simplex::MyEntityManager::GetModel(String a_sUniqueID)
{
	int a_uIndex = GetEntityIndex(a_sUniqueID);
	return GetModel(a_uIndex);
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(uint a_uIndex)
{
	if (a_uIndex == -1)
		a_uIndex = m_uEntityCount - 1;

	if (a_uIndex < m_uEntityCount)
		return m_entityList[a_uIndex]->GetRigidBody();
	else {
		//print out something
		return nullptr;
	}
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(String a_sUniqueID)
{
	int a_uIndex = GetEntityIndex(a_sUniqueID);
	return GetRigidBody(a_uIndex);
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(uint a_uIndex)
{
	if (a_uIndex == -1)
		a_uIndex = m_uEntityCount - 1;

	if (a_uIndex < m_uEntityCount)
		return m_entityList[a_uIndex]->GetModelMatrix();
	else {
		//print out something
		return IDENTITY_M4;
	}
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(String a_sUniqueID)
{
	int a_uIndex = GetEntityIndex(a_sUniqueID);
	return GetModelMatrix(a_uIndex);
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	int a_uIndex = GetEntityIndex(a_sUniqueID);
	SetModelMatrix(a_m4ToWorld, a_uIndex);
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, uint a_uIndex)
{
	if (a_uIndex == -1)
		a_uIndex = m_uEntityCount - 1;

	if (a_uIndex < m_uEntityCount)
		m_entityList[a_uIndex]->SetModelMatrix(a_m4ToWorld);
	else {
		//print out error
	}
}
//The big 3
MyEntityManager::MyEntityManager() { Init(); }
MyEntityManager::MyEntityManager(MyEntityManager const& other) { }
MyEntityManager& MyEntityManager::operator=(MyEntityManager const& other) { return *this; }
MyEntityManager::~MyEntityManager() { Release(); };
// other methods
void Simplex::MyEntityManager::Update(void)
{
	for (uint i = 0; i < m_uEntityCount - 1; i++) {
		for (uint j = i + 1; j < m_uEntityCount; j++) {
			m_entityList[i]->IsColliding(m_entityList[j]);
		}
	}
}
void Simplex::MyEntityManager::AddEntity(String a_sFileName, String a_sUniqueID)
{
	MyEntity* entity = new MyEntity(a_sFileName, a_sUniqueID);
	if (!entity->IsInitialized())
		return;

	m_entityList.push_back(entity);
	m_uEntityCount += 1;
}
void Simplex::MyEntityManager::RemoveEntity(uint a_uIndex)
{
	if (a_uIndex == -1)
		a_uIndex = m_uEntityCount - 1;

	if (a_uIndex < m_uEntityCount)
	{
		std::swap(m_entityList[a_uIndex], m_entityList[a_uIndex - 1]);
		SafeDelete(m_entityList[m_uEntityCount - 1]);
		m_entityList.pop_back();
	}

	//log out some error or something
}
void Simplex::MyEntityManager::RemoveEntity(String a_sUniqueID)
{
	int a_uIndex = GetEntityIndex(a_sUniqueID);
	RemoveEntity(a_uIndex);
}
String Simplex::MyEntityManager::GetUniqueID(uint a_uIndex)
{
	if (a_uIndex == -1)
		a_uIndex = m_uEntityCount - 1;

	if (a_uIndex < m_uEntityCount) {
		return m_entityList[a_uIndex]->GetUniqueID();
	}
	else return "";
}
MyEntity* Simplex::MyEntityManager::GetEntity(uint a_uIndex)
{
	if (a_uIndex == -1)
		a_uIndex = m_uEntityCount - 1;

	if (a_uIndex < m_uEntityCount) {
		return m_entityList[a_uIndex];
	}
	else return nullptr;
}
void Simplex::MyEntityManager::AddEntityToRenderList(uint a_uIndex, bool a_bRigidBody)
{
	if (a_uIndex == -1) {
		for (a_uIndex = 0; a_uIndex < m_uEntityCount; ++a_uIndex) {
			m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
		}
	}
	else if (a_uIndex > m_uEntityCount - 1) {
		m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
	}
}
void Simplex::MyEntityManager::AddEntityToRenderList(String a_sUniqueID, bool a_bRigidBody)
{
	uint a_uIndex = GetEntityIndex(a_sUniqueID);
	AddEntityToRenderList(a_uIndex, a_bRigidBody);
}