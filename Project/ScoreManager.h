/*----------------------------------------------
Programmer: T.J. Tarazevits (tjt3085@gmail.com)
Date: 2017/12
----------------------------------------------*/
#ifndef __SCOREMANAGER_H_
#define __SCOREMANAGER_H_

#include "Simplex\Simplex.h";

namespace Simplex
{

    //System Class
    class ScoreManager {
        static ScoreManager* m_pInstance; // Singleton pointer
        uint currentScore = 0;
    public:
        /*
        Usage: Gets the singleton pointer
        Arguments: ---
        Output: singleton pointer
        */
        static ScoreManager* GetInstance();
        /*
        Usage: Releases the content of the singleton
        Arguments: ---
        Output: ---
        */
        static void ReleaseInstance(void);
        /*
        USAGE: Sets the score to zero
        ARGUMENTS: ---
        OUTPUT: ---
        */
        void ResetScore();
        /*
        USAGE: Adds the given points to the total score
        ARGUMENTS: ---
        OUTPUT: ---
        */
        void AddScore(uint points);
        /*
        USAGE: Gets the current score
        ARGUMENTS: ---
        OUTPUT: ---
        */
        uint GetScore();
    private:
        /*
        Usage: constructor
        Arguments: ---
        Output: class object instance
        */
        ScoreManager(void);
        /*
        Usage: copy constructor
        Arguments: class object to copy
        Output: class object instance
        */
        ScoreManager(ScoreManager const& a_pOther);
        /*
        Usage: copy assignment operator
        Arguments: class object to copy
        Output: ---
        */
        ScoreManager& operator=(ScoreManager const& a_pOther);
        /*
        Usage: destructor
        Arguments: ---
        Output: ---
        */
        ~ScoreManager(void);
        /*
        Usage: releases the allocated member pointers
        Arguments: ---
        Output: ---
        */
        void Release(void);
        /*
        Usage: initializes the singleton
        Arguments: ---
        Output: ---
        */
        void Init(void);
    };
}
#endif 