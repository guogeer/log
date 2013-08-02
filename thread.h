#ifndef _GITCOM_THREAD_H
#define _GTICOM_THREAD_H


#ifdef _WIN32
#include <windows.h>
#include <process.h>
#elif __linux
#include <pthread.h>
#include <unistd.h>
#endif


#ifdef _WIN32
#define xmutex HANDLE
#elif __linux
#define xmutex pthread_mutex_t
#endif

#ifdef _WIN32
#define mutex_init(mtx) mtx = CreateMutex(NULL, FALSE, NULL)
#elif __linux
#define mutex_init(mtx) pthread_mutex_init(& mtx, NULL)
#endif

#ifdef _WIN32
#define mutex_lock(mtx) WaitForSingleObject(mtx, INFINITE)
#elif __linux
#define mutex_lock(mtx) pthread_mutex_lock(& mtx)
#endif

#ifdef _WIN32
#define mutex_unlock(mtx) ReleaseMutex(mtx)
#elif __linux
#define mutex_unlock(mtx) pthread_mutex_unlock(& mtx)
#endif

#ifdef _WIN32
#define thread_t HANDLE
#elif __linux
#define thread_t pthread_t
#endif

#ifdef _WIN32
#define thread_create(tid, fnt) tid=(HANDLE)_beginthreadex(NULL, 0, fnt, NULL, 0, NULL)
#elif __linux
#define thread_create(tid, fnt) pthread_create(&tid, NULL, fnt, NULL)
#endif

#ifdef _WIN32
#define THREAD_RETURN unsigned __stdcall
#elif __linux
#define THREAD_RETURN void *
#endif

#endif
