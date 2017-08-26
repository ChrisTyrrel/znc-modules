/*
 * Copyright (C) 2017 Chris Tyrrel
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <znc/IRCSock.h>
#include <znc/IRCNetwork.h>
#include <znc/Modules.h>
 
 class CCapMod : public CModule {
 public:
     MODCONSTRUCTOR(CCapMod) {}
 
     bool OnServerCapAvailable(const CString& sCap) override {
         return sCap.Equals("chghost");
     }
 
     void OnServerCapResult(const CString& sCap, bool bSuccess) override {
         if (sCap.Equals("chghost")) {
             if (bSuccess) {m_ServerChghostEnabled = true;}
         }
     }
 
     EModRet OnRaw(CString& sLine) {
         CString sCmd = sLine.Token(1);
         if (sCmd.Equals("CHGHOST")) {
             PutUser(sLine);
         }
         
         return CONTINUE;
     }

     void OnClientCapLs(CClient* client, SCString& caps) override {
         if (m_ServerChghostEnabled) {
            caps.insert("chghost");            
         }
     }
     
     bool IsClientCapSupported(CClient* client, const CString& cap, bool state) override {
         return cap.Equals("chghost");
     }
 
 private:
     bool m_ServerChghostEnabled = false;
};

GLOBALMODULEDEFS(CCapMod, "Module to enable capabilities dynamically.")