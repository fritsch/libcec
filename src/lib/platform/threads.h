#pragma once
/*
 * This file is part of the libCEC(R) library.
 *
 * libCEC(R) is Copyright (C) 2011 Pulse-Eight Limited.  All rights reserved.
 * libCEC(R) is an original work, containing original code.
 *
 * libCEC(R) is a trademark of Pulse-Eight Limited.
 *
 * This program is dual-licensed; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *
 * Alternatively, you can license this library under a commercial license,
 * please contact Pulse-Eight Licensing for more information.
 *
 * For more information contact:
 * Pulse-Eight Licensing       <license@pulse-eight.com>
 *     http://www.pulse-eight.com/
 *     http://www.pulse-eight.net/
 */

#include <stdint.h>

namespace CEC
{
  class IMutex
  {
  public:
    IMutex(bool bRecursive = true) { m_bRecursive = bRecursive ; };
    virtual ~IMutex(void) {};

    virtual bool TryLock(void) = 0;
    virtual bool Lock(void) = 0;
    virtual void Unlock(void) = 0;

  protected:
    bool m_bRecursive;
  };

  class ICondition
  {
  public:
    virtual void Broadcast(void) = 0;
    virtual void Signal(void) = 0;
    virtual bool Wait(IMutex *mutex, uint32_t iTimeout = 0) = 0;

    static void Sleep(uint32_t iTimeout);
  };

  class IThread
  {
  public:
    IThread(void);
    virtual ~IThread(void);

    virtual bool IsRunning(void) const { return m_bRunning; };
    virtual bool CreateThread(bool bWait = true) = 0;
    virtual bool IsStopped(void) const { return m_bStop; };

    virtual bool StopThread(bool bWaitForExit = true);
    virtual bool Sleep(uint32_t iTimeout);

    virtual void *Process(void) = 0;

  protected:
    bool        m_bStop;
    bool        m_bRunning;
    ICondition *m_threadCondition;
    IMutex     *m_threadMutex;
  };

  class CLockObject
  {
  public:
    CLockObject(IMutex *mutex, bool bTryLock = false);
    ~CLockObject(void);

    bool IsLocked(void) const { return m_bLocked; }
    void Leave(void);
    void Lock(void);

  private:
    IMutex *m_mutex;
    bool    m_bLocked;
  };
};
