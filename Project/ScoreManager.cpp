#include "ScoreManager.h"
using namespace Simplex;
//  ScoreManager
Simplex::ScoreManager* Simplex::ScoreManager::m_pInstance = nullptr;
Simplex::ScoreManager::ScoreManager() { Init(); }
Simplex::ScoreManager::ScoreManager(ScoreManager const& a_pOther) { }
Simplex::ScoreManager& Simplex::ScoreManager::operator=(ScoreManager const& a_pOther) { return *this; }
Simplex::ScoreManager::~ScoreManager() { Release(); };
void Simplex::ScoreManager::Init(void)
{
    currentScore = 0;
}
void Simplex::ScoreManager::Release(void)
{
    return;
}
Simplex::ScoreManager* Simplex::ScoreManager::GetInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new ScoreManager();
    }
    return m_pInstance;
}
void Simplex::ScoreManager::ReleaseInstance()
{
    if (m_pInstance != nullptr)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}
void Simplex::ScoreManager::AddScore(uint points) {

}
void Simplex::ScoreManager::ResetScore() {

}

uint Simplex::ScoreManager::GetScore() { return currentScore; }