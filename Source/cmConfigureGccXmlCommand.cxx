/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile$
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include "cmConfigureGccXmlCommand.h"
#include "cmCacheManager.h"

cmConfigureGccXmlCommand::cmConfigureGccXmlCommand()
{
}

cmConfigureGccXmlCommand::~cmConfigureGccXmlCommand()
{
}

// cmConfigureGccXmlCommand
bool cmConfigureGccXmlCommand::InitialPass(std::vector<std::string>& args)
{
  if(args.size() != 2)
    {
    this->SetError("called with incorrect number of arguments");
    return false;
    }
  
  // If the cache entry already exists, we are done.
  std::string cacheName = args[1];
  const char* cacheValue =
    cmCacheManager::GetInstance()->GetCacheValue(cacheName.c_str());
  if(cacheValue && (std::string(cacheValue) != ""))
    { return true; }
  
  // Get the gccxml support directory location.  This is based on the
  // executable location.
  m_SupportDir = this->GetSupportDirectory(args[0].c_str());
  
#if defined(_WIN32) && !defined(__CYGWIN__)
  // On Windows, we will just look at VcInclude/FLAGS.txt for now.
  if(!this->FindVcIncludeFlags())
    {
    return false;
    }
#else
  // On UNIX, we have to determine which compiler is being used, and
  // attempt to use that compiler's support directory.
#endif
  
  // Add the cache entry with the flags found.
  m_Makefile->AddDefinition(cacheName.c_str(), m_Flags.c_str());
  cmCacheManager::GetInstance()->AddCacheEntry(
    cacheName.c_str(),
    m_Flags.c_str(),
    "Flags to GCC-XML to get it to parse the native compiler's headers.",
    cmCacheManager::STRING);
  
  return true;
}


/**
 * Given the location of the GCC-XML executable, find the root of the
 * support library tree.  Subdirectories of the returned location should
 * contain the compiler-specific support libraries.
 */
std::string cmConfigureGccXmlCommand::GetSupportDirectory(const char* exeLoc)
{
  std::string gccxml = exeLoc;
  m_Makefile->ExpandVariablesInString(gccxml);
  
  std::string dir;
  std::string file;
  // Get the directory (also converts to unix slashes).
  cmSystemTools::SplitProgramPath(gccxml.c_str(), dir, file);
  
#if !defined(_WIN32) || defined(__CYGWIN__)
  // On UNIX platforms, we must replace the "/bin" suffix with
  // "/share/GCC_XML".  If there is no "/bin" suffix, we will assume
  // that the user has put everything in one directory, and not change
  // the path.
  if(dir.substr(dir.length()-4, 4) == "/bin")
    {
    dir = dir.substr(0, dir.length()-4) + "/share/GCC_XML";
    }
#endif
  
  return dir;
}

/**
 * Find the flags needed to use the Visual C++ support library.
 */
bool cmConfigureGccXmlCommand::FindVcIncludeFlags()
{
  std::string fname = m_SupportDir+"/VcInclude/FLAGS.txt";
  std::ifstream flagsFile(fname.c_str());
  
  if(!flagsFile)
    {
    std::string err = "Cannot open GCC-XML flags file \""+fname+"\"";
    this->SetError(err.c_str());
    return false;
    }
  
  // TODO: Replace this with a real implementation.
  char buf[4096];
  flagsFile.getline(buf, 4096);
  if(!flagsFile)
    {
    std::string err = "Error reading from GCC-XML flags file \""+fname+"\"";
    this->SetError(err.c_str());
    return false;
    }

  m_Flags = buf;
  
  return true;
}
